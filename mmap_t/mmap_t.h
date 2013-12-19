#include <sys/stat.h>
#include <sys/mman.h>
#include <fnctl.h>
#include <unistd.h>
#include <stdio>



struct mmap_t
{
    size_t size;
    void* ptr;

    mmap_t(const char* path, bool write_mode = false, int flags = MAP_FILE|MAP_PRIVATE)
    {
        int OPEN_MODE = O_RDONLY;
        int PROT = PROT_READ;
        if(write_mode) {
            OPEN_MODE = O_RDWR;
            PROT |= PROT_WRITE;
        }

        int f = open(path, OPEN_MODE);
        struct stat statbuf;
        fstat(f, &statbuf);
        ptr = mmap(0, statbuf.st_size, PROT, flags, f, 0);
        size = statbuf.st_size;
        close(f);  
    }


    ~mmap_t()
    {
        munmap(ptr, size);
    }


    operator bool() const 
    { 
        return (reinterpret_cast<uintptr_t>(ptr) != -1);
    }
};

