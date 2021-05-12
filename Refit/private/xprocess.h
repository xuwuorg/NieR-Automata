/*
    作者：阿虚
    博客：http://www.xuwu.org
    mail：xuwuorg@163.com 
*/

#pragma once
#include "xcommon.h"
#include "xstring.h"

/*
    概述：
        xprocess.h 对进程操作进行了封装实现了对系统进程的枚举，创建新的进程，以及杀进程
        等功能的封装

        xprocess：负责常规进程操作API接口的二次封装
        xprocess_tools：负责获取进程信息枚举等操作。
         
    用法：
        枚举进程
            get_process可以通过不同的std来获取当前系统所有的进程信息
        
            enum_process()枚举进程，你需要实现xenum_proc_base基类的回调接口每遍历到一个
            进程都将会进入回调接口enum_process_callback。此时你可以立即做你想要的判断，
            而非向get一样需要等到所有进程都枚举整理完才能做处理。
*/

namespace xuwuorg
{    
    namespace xprocess_tools
    {
#define STATUS_INFO_LENGTH_MISMATCH ((NTSTATUS)0xC0000004L)
          
        /*
            枚举进程的进行信息类
        */
        class xexp xprocess_info
        {
        public:
            xprocess_info() {};
            inline void set_proc_id(size_t pid) { m_proc_id = pid; };
            inline void set_proc_name(const string& proc_name) { m_proc_name = proc_name; };
            inline void set_proc_full_path(const string& proc_full_path) { m_proc_full_path = proc_full_path; };

            inline size_t get_proc_id() const { return m_proc_id; };
            inline string get_proc_name() const { return m_proc_name; };
            inline string get_proc_full_path() const { return m_proc_full_path; };

        private:
            size_t m_proc_id;           //进程PID
            string m_proc_name;         //进程路径
            string m_proc_full_path;    //进程完整文件路径
        };
          
        /*
            enum_process进程时需要的回调接口类。你需要继承xenum_proc_base并且自己实现
            enum_process_callback()接口。

            每遍历到一个进程都将会调用enum_process_callback接口。此时你可以做任何事情
            如果你找到了你想要的进程信息可以返回false来停止遍历。
        */
        class xexp xenum_proc_base
        {
        public:
            xenum_proc_base() {}
            virtual ~xenum_proc_base() {}

            //枚举进程回调接口。返回true继续枚举下一个进程信息。返回false停止枚举进程。
            virtual bool enum_process_callback(const xprocess_info&) = 0;
        };
           

        //枚举系统进程，就需要自己实现继承xenum_proc_base接口的子类
        xexp size_t enum_process_all(xenum_proc_base*);
          
        
        //获取指定进程明的进程信息。
        //param1:chrome.exe。
        //param2:所有chrome.exe进程信息。
        xexp size_t enum_process_from_name(const string&, std::list<xprocess_info>&);   
    }; 
}; 
