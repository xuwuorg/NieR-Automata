#pragma once
#include <windows.h> 

#ifdef XUWU3_EXPORTS
#define xexp __declspec(dllexport)
#else
#define xexp __declspec(dllimport)
#endif

#define TYPEDEF_POINT_TYPE(name, ret)\
typedef ret (__stdcall *pfun##name)

#define INTERFACE_FUN_STATACMENT(name)\
pfun##name m_##name

#define INTERFACE_FUN_POINT_NULL_THROW(name)\
if(m_##name == nullptr)\
    throw 0;\
return m_##name

#define INTERFACE_FUN_POINT_INIT(hmodule, funame)\
{\
    if (m_##funame == nullptr)\
    {\
        m_##funame = (pfun##funame)::GetProcAddress(hmodule, #funame);\
    }\
}


/**
*   每个类的single标记定义
**/

#define SINGLENTH 12
#define TYPEDEF_SINGLE_VALUE unsigned char m_single[SINGLENTH];

#define TYPEDEF_SINGLE_STRING \
char single[SINGLENTH] = "string";\
memory_management::mem_move(m_single, single, SINGLENTH);

#define TYPEDEF_SINGLE_FILE_HELP \
char single[SINGLENTH] = "file_help";\
memory_management::mem_move(m_single, single, SINGLENTH);

#define TYPEDEF_SINGLE_FILE \
char single[SINGLENTH] = "file";\
memory_management::mem_move(m_single, single, SINGLENTH);

#define TYPEDEF_SINGLE_HEAD void init_single();
#define TYPEDEF_SINGLE_FUN(class, single_type) void class::init_single(){single_type;};

/**
*   字符串类型重新定义
**/

namespace xuwuorg
{ 
    typedef enum _ret_status
    {
        ok
        , err
        , malloc_err
    }ret_status;
};

#ifdef _WIN64
typedef ULONGLONG XADDRESS;
#else
typedef DWORD XADDRESS;
#endif