// Copyright 2020 Fancapital Inc.  All rights reserved.
#pragma once
#include <string>
#include "datastruct.h"
#include <iostream>

using namespace std;


#ifdef _MSC_VER
#include <io.h>
#include <fcntl.h>
#include <windows.h>
#else
#include <sys/mman.h>
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#endif


class CompatibleUnit{
 public:
    CompatibleUnit() {
    }

    ~CompatibleUnit() {
    }

    static char* InitSharedMemory(const string& file, size_t size, bool write_flag) {
    #ifdef _MSC_VER
        bool master = true;
        HANDLE dumpFileDescriptor = CreateFileA(file.c_str(),
            (master) ? (GENERIC_READ | GENERIC_WRITE) : GENERIC_READ,
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            NULL,
            (master) ? OPEN_ALWAYS : OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL);

        if (dumpFileDescriptor == INVALID_HANDLE_VALUE) {
            cout << ("unable to mmap for page " + file);
            throw("unable to mmap for page " + file);
        }


        HANDLE fileMappingObject = CreateFileMapping(dumpFileDescriptor,
            NULL,
            (master) ? PAGE_READWRITE : PAGE_READONLY,
            0,
            size,
            NULL);

        if (fileMappingObject == nullptr) {
            int nRet = GetLastError();
            cout << ("unable to mmap for page " + file);
            throw("unable to mmap for page " + file);
        }

        char* buffer = reinterpret_cast<char*>(MapViewOfFile(fileMappingObject,
            (master) ? FILE_MAP_ALL_ACCESS : FILE_MAP_READ,
            0,
            0,
            size));

        if (buffer == nullptr) {
            int nRet = GetLastError();
            cout << ("failed to load page " + file + ", MapViewOfFile Error " + std::to_string(nRet));
            throw("failed to load page " + file + ", MapViewOfFile Error " + std::to_string(nRet));
        }

        MemoryHeader* header_ = reinterpret_cast<MemoryHeader*>(buffer);
        if (header_) {
            if (header_->offset.load() == 0) {
                header_->offset.store(sizeof(MemoryHeader));
                header_->length = size;
                header_->next.store(false);
                memset(buffer + sizeof(MemoryHeader), 0, size - sizeof(MemoryHeader));
            }
            //  已写满
            if (header_->next.load() && write_flag) {
                ReleaseSharedMemory(buffer, size);
                buffer = nullptr;
            }
        }

        CloseHandle(fileMappingObject);
        CloseHandle(dumpFileDescriptor);
    #else
        cout << "InitSharedMemory" << endl;
        umask(0000);
        int fd = open(file.c_str(), O_RDWR | O_CREAT, 0666);
//        ftruncate(fd, size);
//        struct stat st;
//        fstat(fd, &st);
//        char* buffer = reinterpret_cast<char*>(mmap(NULL, st.st_size,
//               PROT_READ | PROT_WRITE, MAP_SHARED | MAP_POPULATE | MAP_NONBLOCK, fd, 0));
        
        if (lseek(fd, size - 1, SEEK_SET) == -1) {
            close(fd);
        }
        if (write(fd, "", 1) == -1) {
            close(fd);
        }
        
        char* buffer = (char*)mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

        if (MAP_FAILED == buffer) {
            close(fd);
            cout << "error mapping file to buffer" << endl;
            throw("error mapping file to buffer");
        }
        if (madvise(buffer, size, MADV_WILLNEED) != 0 && mlock(buffer, size) != 0) {
            munmap(buffer, size);
            close(fd);
            cout << "error in madvise or mlock" << endl;
            throw("error in madvise or mlock");
        }

        MemoryHeader* header_ = reinterpret_cast<MemoryHeader*>(buffer);
        if (header_) {
            if (header_->offset.load() == 0) {
                header_->offset.store(sizeof(MemoryHeader));
                header_->length = size;
                header_->next.store(false);
                memset(buffer + sizeof(MemoryHeader), 0, size - sizeof(MemoryHeader));
            }
            //  已写满
            if (header_->next.load() && write_flag) {
                ReleaseSharedMemory(buffer, size);
                buffer = nullptr;
            }
        }
        close(fd);
    #endif
        return buffer;
    }

    // 读的时候不可以Release
    static bool ReleaseSharedMemory(char* address, size_t size) {
        void* buffer = reinterpret_cast<void*>(address);
#ifdef _MSC_VER
        FlushViewOfFile(buffer, 0);
        UnmapViewOfFile(buffer);
#else
        munmap(buffer, size);
#endif
        return true;
    }

    static bool JudgeFileExist(const char* file) {
        bool exit_flag = false;
#ifdef _MSC_VER
        if (fopen(file, "r") != nullptr) {
            exit_flag = true;
        }
#else
        if (access(file, 0) == 0) {
            exit_flag = true;
        }
#endif
        return exit_flag;
    }
};

