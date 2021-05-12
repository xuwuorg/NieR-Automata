#pragma once
#include "xcommon.h"
#include "xstring.h"


namespace xuwuorg
{ 
    namespace xmodule
    {
        class xexp xmodule_info
        {
        public:
            xmodule_info() {
                m_hmodule = NULL;
                m_base_address = nullptr;
                m_base_size = 0;
                m_full_name = L"";
            }
            virtual ~xmodule_info() {}

            inline void set_module(HMODULE hmodule) {
                m_hmodule = hmodule; 
                return;
            }

            inline void set_base_address(PVOID base_address) {
                m_base_address = base_address;
                return;
            }

            inline void set_module_size(DWORD base_size) {
                m_base_size = base_size;
                return;
            }

            inline void set_full_name(const string& full_name){
                m_full_name = full_name;
                return;
            }

            inline HMODULE get_module() { return m_hmodule; }

            inline PVOID get_base_address() { return m_base_address; }

            inline DWORD get_module_size() { return m_base_size; }

            inline string get_full_name() { return m_full_name; }

        private:
            HMODULE m_hmodule;
            PVOID m_base_address;
            DWORD m_base_size;
            string m_full_name; 
        };

        xexp size_t enum_process_module(const DWORD, std::list<xmodule_info>&);
    };
}; 

