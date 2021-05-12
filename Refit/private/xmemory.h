#pragma once
#include "xcommon.h"

namespace xuwuorg
{ 
    namespace memory_management
    {
        xexp bool mem_set(unsigned char*, int, size_t);

        xexp bool mem_zero(void*, size_t);
    }; 
}; 

