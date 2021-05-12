#include "pch.h"
#include "xprocess.h" 
#include "xpsapi.h"
#include "xntdll.h"
#include "xnt_process.h"
using namespace xuwuorg;

xexp
bool 
xprocess::new_process(
    const xprocess_param&)
{
    return true;
}

xexp 
HANDLE 
xprocess::open_process(
    const DWORD pid
    , const DWORD desired_access)
{
    CLIENT_ID ClientId;

    ClientId.UniqueThread = NULL;
    ClientId.UniqueProcess = LongToHandle(pid);

    OBJECT_ATTRIBUTES oa;
    InitializeObjectAttributes(
        &oa,
        NULL,
        0,
        NULL,
        NULL);

    HANDLE handle = INVALID_HANDLE_VALUE;
    NTSTATUS status = xntdll::pins()->ZwOpenProcess(
        &handle
        , desired_access
        , &oa
        , &ClientId);

    return handle;
}

xexp 
bool
xprocess::close_process(
    HANDLE handle)
{  
    NTSTATUS status = xntdll::pins()->ZwClose(handle);
    return NT_SUCCESS(status)? true : false;
}

xexp 
DWORD 
xprocess::get_current_pid()
{
    struct _TEB
    {
        NT_TIB NtTib;
        PVOID  EnvironmentPointer;
        CLIENT_ID ClientId;
    };
    
    _TEB* p = nullptr;
#ifdef _WIN64
    p = (_TEB*)__readgsqword(0x30);
#else
    p = (_TEB*)__readfsdword(PcTeb);
#endif 
    DWORD ret = HandleToUlong(p->ClientId.UniqueProcess);
    return ret;
}

xexp
bool
xprocess::kill_process_pid(
    const DWORD pid
    , const DWORD exit_code)
{   
    HANDLE handle = open_process(pid, PROCESS_TERMINATE);
    if (handle != INVALID_HANDLE_VALUE)
    {
        NTSTATUS status =  xntdll::pins()->ZwTerminateProcess(handle, exit_code);
        close_process(handle);

        return NT_SUCCESS(status) ? true : false;
    }
    return false;
}

xexp 
void 
xprocess::exit_process(
    const DWORD exit_code)
{
    kill_process_pid(get_current_pid(), exit_code);
}

/*
    下面是进程工具类
*/

xexp 
size_t 
xprocess_tools::enum_process_all(
    xprocess_tools::xenum_proc_base* call_back)
{
    if (call_back == nullptr)
    {
        return 0;
    }

    unsigned char* p = enum_process_all();
    if (p == nullptr)
    {
        return 0;
    }
      
    SYSTEM_PROCESS_INFORMATION* ppi; 
    ULONG offset = 0; 
    size_t count = 0;

    //枚举当前系统所有进程。
    do 
    {
        ppi = (PSYSTEM_PROCESS_INFORMATION)(p + offset);
        offset += ppi->NextEntryOffset;

        //我只需要pid和进程名字就行了
        xuwuorg::xprocess_tools::xprocess_info pi;
        pi.set_proc_id((DWORD)ppi->UniqueProcessId);
        pi.set_proc_name(xuwuorg::string(
            (wchar_t*)ppi->ImageName.Buffer
            , ppi->ImageName.Length / sizeof(wchar_t)));

        if (pi.get_proc_name().not_empty())
        {
            //把获取当前进程的完整文件路径
            string full_path = get_proc_path_by_pid(pi.get_proc_id());

            pi.set_proc_full_path(full_path);

            if (!call_back->enum_process_callback(pi))
            {
                break;
            }

            count++;
        }
        else {} //如果没有进程名字要他有何用。

    } while (ppi->NextEntryOffset != 0);

    if (p != nullptr)
    {
        delete[] p;
        p = nullptr;
    }

    return count;
} 

unsigned char* 
xprocess_tools::enum_process_all()
{
    NTSTATUS status = STATUS_INFO_LENGTH_MISMATCH;
    ULONG buff_size = 64 * 1024;
    unsigned char* p = nullptr;

    //负责申请能够存储当前系统进程所有信息所需要的内存空间以及获取整个系统进程信息

    for (int index = 0; ((index < 0x1000) && (status == STATUS_INFO_LENGTH_MISMATCH)); index++)
    { 
        p = new unsigned char[buff_size];
        if (p == nullptr)
        {
            return 0;
        }

        status = xuwuorg::xntdll::pins()->ZwQuerySystemInformation(
            SystemProcessInformation,
            p,
            buff_size,
            NULL);
        if (status == STATUS_INFO_LENGTH_MISMATCH)
        {
            delete[] p;
            p = nullptr;
            buff_size += 8192;
        } 
    }

    return p;
}

class xenum_process_callback : public xprocess_tools::xenum_proc_base
{
public:
    xenum_process_callback(
        const string& fp
        , std::list<xprocess_tools::xprocess_info>* p
        , std::map<DWORD, string>* pmap)
        : m_proc_name(fp)
        , m_list(p)
        , m_map(pmap)
    {}
    virtual ~xenum_process_callback() {}

    virtual bool enum_process_callback(
        const xprocess_tools::xprocess_info& pi)
    {
        if (m_list != nullptr)
        {
            if (m_proc_name.empty())
            {
                m_list->push_back(pi);
            }
            else if (m_proc_name == pi.get_proc_name())
            {
                m_list->push_back(pi);
            }
        }

        if (m_map != nullptr)
        {
            if (pi.get_proc_full_path().empty())
            {
                m_map->insert(std::pair<DWORD, string>(
                    pi.get_proc_id()
                    , pi.get_proc_name()));
            }
            else
            {
                m_map->insert(std::pair<DWORD, string>(
                    pi.get_proc_id()
                    , pi.get_proc_full_path()));
            } 
        }
        return true;
    }

private:
    string m_proc_name;
    std::list<xprocess_tools::xprocess_info>* m_list; 
    std::map<DWORD, string>* m_map;
};

xexp
size_t 
xprocess_tools::enum_process_all(
    std::list<xprocess_info>& pi)
{
    xenum_process_callback* p = new xenum_process_callback(L"", &pi, nullptr);
    if (p == nullptr)
    {
        return 0;
    }

    enum_process_all(p);
    return pi.size();
}

xexp 
size_t 
xprocess_tools::enum_process_from_name(
    const string& proc_name
    , std::list<xprocess_info>& pi)
{
    xenum_process_callback* p = new xenum_process_callback(proc_name, &pi, nullptr);
    if (p == nullptr)
    {
        return 0;
    }

    enum_process_all(p);
    
    delete p;
    return pi.size();
} 

xexp
size_t 
xprocess_tools::enum_process_all(
    std::map<DWORD, string>& pi)
{
    xenum_process_callback* p = new xenum_process_callback(L"", nullptr, &pi);
    if (p == nullptr)
    {
        return 0;
    }

    enum_process_all(p);
    return pi.size();
}

xexp
bool xprocess_tools::is_run_process(
    const string& proc_name)
{
    std::list<xprocess_info> pi; 
    return enum_process_from_name(proc_name, pi)? true : false;
}

xexp
size_t 
xprocess_tools::get_proc_pid_by_path(
    const string& proc_name
    , std::list<DWORD>& pid_list)
{
    std::list<xprocess_info> pi;
    enum_process_from_name(proc_name, pi);

    auto it = pi.begin();
    for (it; it != pi.end(); it++)
    {
        pid_list.push_back(it->get_proc_id());
    }

    return pid_list.size();
}

xexp
string 
xprocess_tools::get_proc_path_by_pid(
    DWORD pid)
{ 
    string full_path;

    HANDLE handle = xprocess::open_process(
        pid
        , PROCESS_QUERY_INFORMATION | PROCESS_VM_READ);
    if (handle != INVALID_HANDLE_VALUE)
    {
        wchar_t szProcessName[512] = { 0 };
        xpsapi::pins()->GetModuleFileNameExW(
            handle
            , NULL
            , szProcessName
            , sizeof(szProcessName));
    
        xntdll::pins()->ZwClose(handle); 

        full_path = szProcessName;
    }

    return full_path;
}
 
xexp 
size_t 
xprocess_tools::get_proc_modules(
    DWORD pid
    , std::list<string>& pm)
{
    PRTL_DEBUG_INFORMATION p = xntdll::pins()->RtlCreateQueryDebugBuffer(0, FALSE);

    NTSTATUS status = xntdll::pins()->RtlQueryProcessDebugInformation(
        (HANDLE)LongToHandle(pid)
        , RTL_QUERY_PROCESS_NONINVASIVE | RTL_QUERY_PROCESS_MODULES32 | RTL_QUERY_PROCESS_MODULES
        , p);
    if (NT_SUCCESS(status))
    {
        for (ULONG i = 0; i < p->Modules->NumberOfModules; i++)
        {
            pm.push_back(p->Modules->Modules[i].FullPathName);
        }
    }

    xntdll::pins()->RtlDestroyQueryDebugBuffer(p);

    return pm.size();
}