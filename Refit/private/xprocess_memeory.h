#pragma once
#include "xcommon.h"

namespace xuwuorg
{   

#define XPROC_MEM_READ 1
#define XPROC_MEM_WRITE 2 

    class xexp xprocess_memory_base
    {
    public:
        xprocess_memory_base(); 
        virtual ~xprocess_memory_base();

        void close();

        virtual bool open_process(DWORD) = 0;
           
    protected:
        bool open(DWORD, DWORD);

        DWORD m_pid;
        HANDLE m_handle;
    };

    class xexp xprocess_offset
    {
    public:
        xprocess_offset()
            : m_base(0)
            , m_base_size(0)
        {};
        virtual ~xprocess_offset() 
        {
            m_base = 0;
            m_base_size = 0;
        };
         
        inline void set_base_address(XADDRESS p) { m_base = p; };
        inline XADDRESS get_base_address() { return m_base; };

        inline void set_base_size(DWORD size) { m_base_size = size; };
        inline DWORD get_base_size() { return m_base_size; };

    public:
        XADDRESS m_base;
        DWORD m_base_size;
    };
     
    class xexp xprocess_memory_read 
        : virtual public xprocess_memory_base
        , virtual public xprocess_offset
    {
    public:
        xprocess_memory_read();
        virtual ~xprocess_memory_read();
        
        virtual bool open_process(DWORD);

        bool read(XADDRESS, unsigned char*, SIZE_T, SIZE_T* = nullptr);
        bool read(XADDRESS, unsigned char**, SIZE_T, SIZE_T* = nullptr);
        bool read_offset(XADDRESS, unsigned char*, SIZE_T, SIZE_T* = nullptr);
        bool read_offset(XADDRESS, unsigned char**, SIZE_T, SIZE_T* = nullptr);
    };

    class xexp xprocess_memory_write
        : virtual public xprocess_memory_base
        , virtual public xprocess_offset
    {
    public:
        xprocess_memory_write();
        virtual ~xprocess_memory_write();

        virtual bool open_process(DWORD);

        bool write(XADDRESS, unsigned char*, SIZE_T, SIZE_T* = nullptr);
        bool write_offset(XADDRESS, unsigned char*, SIZE_T, SIZE_T* = nullptr);
    };

    class xexp xprocess_memeory
        : public xprocess_memory_read
        , public xprocess_memory_write
    {
    public:
        xprocess_memeory();  
        virtual ~xprocess_memeory();
        virtual bool open_process(DWORD);
    };
}; 

