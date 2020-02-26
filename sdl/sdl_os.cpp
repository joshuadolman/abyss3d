#if OS_WINDOWS
    inline void* 
    OS_AllocateMemory(void* address, s64 memorySize)
    {
        return VirtualAlloc(address, memorySize, MEM_COMMIT | MEM_RESERVE, 0);
    }

    inline void* 
    OS_AllocateMemory(s64 memorySize)
    {
        return VirtualAlloc(0, memorySize, MEM_COMMIT | MEM_RESERVE, 0);
    }

    inline b32
    OS_FreeMemory(void* address)
    {
        return VirtualFree(address, memorySize, MEM_DECOMMIT | MEM_RELEASE);
    }

    inline b32
    OS_FreeMemory(void* address)
    {
        return VirtualFree(address, 0, MEM_DECOMMIT | MEM_RELEASE);
    }

#elif OS_LINUX
    inline void* 
    OS_AllocateMemory(void* address, s64 memorySize)
    {
        return mmap(address, memorySize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    }

    inline void*
    OS_AllocateMemory(s64 memorySize)
    {
        return mmap(0, memorySize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    }

    inline b32
    OS_FreeMemory(void* address, s64 memorySize)
    {
        if(munmap(address, memorySize) == 0)
            return true;
        else
            return false;
    }
#endif