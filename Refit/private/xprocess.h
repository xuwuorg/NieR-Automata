/*
    ���ߣ�����
    ���ͣ�http://www.xuwu.org
    mail��xuwuorg@163.com 
*/

#pragma once
#include "xcommon.h"
#include "xstring.h"

/*
    ������
        xprocess.h �Խ��̲��������˷�װʵ���˶�ϵͳ���̵�ö�٣������µĽ��̣��Լ�ɱ����
        �ȹ��ܵķ�װ

        xprocess�����𳣹���̲���API�ӿڵĶ��η�װ
        xprocess_tools�������ȡ������Ϣö�ٵȲ�����
         
    �÷���
        ö�ٽ���
            get_process����ͨ����ͬ��std����ȡ��ǰϵͳ���еĽ�����Ϣ
        
            enum_process()ö�ٽ��̣�����Ҫʵ��xenum_proc_base����Ļص��ӿ�ÿ������һ��
            ���̶��������ص��ӿ�enum_process_callback����ʱ���������������Ҫ���жϣ�
            ������getһ����Ҫ�ȵ����н��̶�ö�����������������
*/

namespace xuwuorg
{    
    namespace xprocess_tools
    {
#define STATUS_INFO_LENGTH_MISMATCH ((NTSTATUS)0xC0000004L)
          
        /*
            ö�ٽ��̵Ľ�����Ϣ��
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
            size_t m_proc_id;           //����PID
            string m_proc_name;         //����·��
            string m_proc_full_path;    //���������ļ�·��
        };
          
        /*
            enum_process����ʱ��Ҫ�Ļص��ӿ��ࡣ����Ҫ�̳�xenum_proc_base�����Լ�ʵ��
            enum_process_callback()�ӿڡ�

            ÿ������һ�����̶��������enum_process_callback�ӿڡ���ʱ��������κ�����
            ������ҵ�������Ҫ�Ľ�����Ϣ���Է���false��ֹͣ������
        */
        class xexp xenum_proc_base
        {
        public:
            xenum_proc_base() {}
            virtual ~xenum_proc_base() {}

            //ö�ٽ��̻ص��ӿڡ�����true����ö����һ��������Ϣ������falseֹͣö�ٽ��̡�
            virtual bool enum_process_callback(const xprocess_info&) = 0;
        };
           

        //ö��ϵͳ���̣�����Ҫ�Լ�ʵ�ּ̳�xenum_proc_base�ӿڵ�����
        xexp size_t enum_process_all(xenum_proc_base*);
          
        
        //��ȡָ���������Ľ�����Ϣ��
        //param1:chrome.exe��
        //param2:����chrome.exe������Ϣ��
        xexp size_t enum_process_from_name(const string&, std::list<xprocess_info>&);   
    }; 
}; 
