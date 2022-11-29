// Copyright 2020 Fancapital Inc.  All rights reserved.
#pragma once
#include <vector>
#include <string>
#include <functional>
#include "./datastruct.h"

class Event {
 public:
    Event() {}
    virtual ~Event() {}
    virtual void Notify(QTickT* tick) {}
};

// 无差别的Event
class FeederEvent : public Event {
 public:
    FeederEvent() {}
    virtual ~FeederEvent() {}

    void SetFunc(const std::function<void(QTickT*)>& func) {
        func_ = func;
    }

    void Notify(QTickT* tick) {
        func_(tick);
    }
 private:
    std::function<void(QTickT*)> func_ = nullptr;
};

// 合约Event，可以一个StrategyUnit, 可以多个StrategyUnit
class InstrumentEvent : public Event {
 public:
    InstrumentEvent() {}
    virtual ~InstrumentEvent() {}

    void SetEventInstrument(const char* instrument) {
        instrument_ = instrument;
    }

    void AddFunc(const std::function<void(QTickT*)>& func) {
        vec_func_.push_back(func);
        //cout << __FUNCTION__ << ", " << instrument_ << ", size: " << vec_func_.size() << endl;
    }

    void Notify(QTickT* tick) {
        if (instrument_.compare(tick->code) == 0) {
            for (auto& it : vec_func_) {
                it(tick);
            }
        }
    }
 private:
    string instrument_;
    vector<std::function<void(QTickT*)>> vec_func_;
};
