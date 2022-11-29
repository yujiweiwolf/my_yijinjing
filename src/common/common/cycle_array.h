// Copyright 2020 Fancapital Inc.  All rights reserved.
#pragma once
#include <vector>
#include "./datastruct.h"

template <typename T>
class CycleArray {
 public:
    CycleArray() {
        datalength_ = sizeof(T);
        write_index_.store(0);
    }

    ~CycleArray() {
    }

    void WriteData(T* data) {
        int mod = write_index_.load() & (FEEDERLENGTH - 1);
        memcpy(dataArray_ + mod, data, datalength_);
        write_index_.add(1);
    }

    T* GetCycleHeader() {
        return dataArray_;
    }

    void GetMemoryMessage(std::vector<T*>& vec_msg) {
        while (read_index_ < write_index_.load()) {
            int mod = read_index_ & (FEEDERLENGTH - 1);
            vec_msg.push_back(dataArray_ + mod);
            read_index_++;
        }
    }

 public:
    int datalength_;
    T dataArray_[FEEDERLENGTH];
    boost::atomic_int write_index_;
    int read_index_ = 0;
};
