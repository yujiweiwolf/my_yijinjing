// Copyright 2020 Fancapital Inc.  All rights reserved.
#pragma once
#include "define.h"
#include "./datastruct.h"
#include "./commonunit.h"
#include "./compatible_unit.h"

#define DATALENGTH 1024 * 1024
#define FEEDERLENGTH 1024 * 1024
#define MAX_VECTOR_SIZE 100

class BaseBuffer {
 public:
    BaseBuffer() {
    }

    virtual ~BaseBuffer() {
    }

    void SetSharedMemory() {
        shared_memory_flag_ = true;
    }

    MemoryHeader* GetMemoryHeader() {
        return header_;
    }

    bool IsCrossBoundary(long length) {
        return (header_->offset.load() + static_cast<long>(sizeof(MemoryMessage)) + length > header_->length);
    }
    
    bool IsInitNextMemory(long length) {
        return (header_->offset.load() + static_cast<long>(sizeof(MemoryMessage)) + length + left_size_ > header_->length);
    }

    template <typename T>
    MemoryMessage* AllocateMessage(int type) {
        if (!init_next_shm_falg_ && IsInitNextMemory(sizeof(T))) {
            init_next_shm_falg_ = true;
            thread worker(&BaseBuffer::InitNextMemoryHeader, this);
            worker.detach();
        }
        
        if (IsCrossBoundary(sizeof(T))) {
            LOG_INFO << "cross";
            ResetMemoryHeader();
        }
        MemoryMessage* current_message = reinterpret_cast<MemoryMessage*>(
                                        (reinterpret_cast<char*>(header_) + header_->offset.load()));
        current_message->field = reinterpret_cast<char*>(current_message) + sizeof(MemoryMessage);
        current_message->header.length = sizeof(T);
        current_message->header.type = type;
        // printf("open_data header <%x> current_message<%x>, offset<%d>  flag <%d>\n",
        // header_, current_message, header_->offset.load(), flag);
        return current_message;
    }

    template<typename T>
    void CloseAllocate() {
        header_->offset.add(sizeof(MemoryMessage) + sizeof(T));
        //printf("CloseAllocate header <%p>, offset<%ld>\n", header_, header_->offset.load());
    }

    // 直接写入(日志等)
    void AllocateBuffer(const std::string& info, int type, bool multithread_flag) {
        if (IsCrossBoundary(sizeof(info.length()))) {
            ResetMemoryHeader();
        }
        // 多线程写需要加锁
        // SpinLock spinlock(lock_flag_);

        MemoryMessage* current_message = nullptr;
        long offset = header_->offset.load();
        current_message = reinterpret_cast<MemoryMessage*>(reinterpret_cast<char*>(header_) + offset);
        header_->offset.add(static_cast<long>(info.length() + sizeof(MemoryMessage)));
        memcpy(reinterpret_cast<char*>(current_message) + sizeof(MemoryMessage), info.c_str(), info.length());
        current_message->header.length = static_cast<int>(info.size());
        current_message->header.type = type;
    }    

    virtual void InitMemoryHeader() {
    }
    
    virtual void InitNextMemoryHeader() {
    }

    virtual void ResetMemoryHeader() {
    }    
    

 protected:
    // 文件的序列号
    int file_index_ = 0;
    // 标志是共享内存还是普通内存
    bool shared_memory_flag_ = false;
    MemoryHeader* header_ = nullptr;
    std::atomic_flag lock_flag_ = ATOMIC_FLAG_INIT;
    // 当前的共享内存还剩8M时, 开辟一线程, 分配下一块
    bool init_next_shm_falg_ = false;
    long left_size_ = 1024 * 1024 * 8;
    MemoryHeader* next_header_ = nullptr;
};

class ContinuousShM : public BaseBuffer {
 public:
    ContinuousShM() {
        file_index_ = 0;
        size_ = 0;
    }

    virtual~ContinuousShM() {
    }

    void Init(const char* dir, const char* file, long size, SM_MODEL model) {
        dir_ = dir;
        file_ = file;
        size_ = size;
        if (model == SM_MODEL::QUICK || model == SM_MODEL::START) {
            write_flag_ = true;
        }
        InitMemoryHeader();
    }

    virtual void InitMemoryHeader() {
        while (!header_) {
            std::string full_file = fmt::format("{0}/{1}_{2}_{3}", dir_.c_str(), file_.c_str(),
                                                CommonUnit::GetTradingDay(), file_index_++);
            header_ = reinterpret_cast<MemoryHeader*>(CompatibleUnit::InitSharedMemory(full_file.c_str(),
                                                      size_, write_flag_));
        }
    }

    virtual void ResetMemoryHeader() {        
        header_->next.store(true);
        init_next_shm_falg_ = false;
        header_ = next_header_;
        next_header_ = nullptr;
    }

    void InitNextMemoryHeader() {
        std::string full_file = fmt::format("{0}/{1}_{2}_{3}", dir_, file_, CommonUnit::GetTradingDay(), file_index_++);
        next_header_ = reinterpret_cast<MemoryHeader*>(CompatibleUnit::InitSharedMemory(full_file.c_str(),
                                                  size_, write_flag_));
        next_header_->msg_count.store(0);
        next_header_->offset.store(sizeof(MemoryHeader));
        next_header_->next.store(false);
        LOG_INFO << "next";
    } 

 private:
    string dir_;
    string file_;
    long size_;
    // 读、写标志，分别很大
    bool write_flag_ = false;
};

class CommonShM {
public:
    // 此前的共享内存读完时，返回true
    static bool ReadShmData(MemoryHeader* read_header, std::vector<MemoryMessage*> & out, long& read_offset) {
        bool finish_flag = false;
        while (read_offset < read_header->offset.load()) {
            //MemoryMessage* message = GetMemoryMessage(read_header, read_offset);
            MemoryMessage* message = reinterpret_cast<MemoryMessage*> (reinterpret_cast<char*> (read_header) + read_offset);
            message->field = reinterpret_cast<char*> (reinterpret_cast<char*> (read_header) + read_offset + sizeof (MemoryMessage));            
            read_offset += (message->header.length + sizeof(MemoryMessage));
            out.push_back(message);
            // 设置vector的最大容量是10000。初始化读当天的数据量会很大
            if (out.size() >= MAX_VECTOR_SIZE) {
                break;
            }
        }

        if (read_header->next.load() && read_offset == read_header->offset.load()) {
            finish_flag = true;
        }
        //printf("ReadShmData <%p> size<%d> offset<%ld>\n", read_header, out.size(), read_offset);
        return finish_flag;
    }
};
