#pragma once
#include <iostream>
#include <fstream>
#include <string.h>
#include <math.h>
#include <sched.h>
#include <unistd.h>
#include <algorithm>
#include "../journal/JournalReader.h"
#include "../journal/JournalWriter.h"
#include "../journal/FrameHeader.h"
#include "../journal/Frame.hpp"
#include "../journal/Timer.h"
#include "../utils/datastruct.h"

using yijinjing::JournalReaderPtr;
using yijinjing::JournalWriterPtr;
using namespace yijinjing;

// 一个reader, 读feeder 与 broker
// 一个writer, 写broker
class Strategy {
public:
    Strategy();
    virtual ~Strategy();
    void SetStragety(string strategy_name, int64_t strategy_id);
    void StartWork();

    template<typename T>
    void WriteData(T* data, int msg_type) {
        writer->write_frame((void*)data, sizeof(T), msg_type, 0);
    }
    void SubInstrument(std::vector<std::string>& codes);
    void AddReadFile(const string& dir, const string file);
    void AddWriteFile(const string& dir, const string file);

    virtual void OnInit() {};
    virtual void OnTick(QTickT* data) {};
    virtual void OnLevel2Order(QOrderT* data) {};
    virtual void OnLevel2Knock(QKnockT* data) {};

    virtual void OnRspQueryAccout(QueryTradeAssetRep* rsp) {};
    virtual void OnRspQueryPosition(QueryTradePositionRep* rsp) {};
    virtual void OnRspQueryOrder(QueryTradeOrderRep* rsp) {};
    virtual void OnRspQueryTrade(QueryTradeKnockRep* rsp) {};

    virtual void OnRspOrder(TradeOrderRep* rsp) {};
    virtual void OnRspWithdraw(TradeWithdrawRep* rsp) {};
    virtual void OnRtnTradeKnock(TradeKnock* rtn) {};

private:
    void Run();

    void ProcessMessage(yijinjing::FramePtr frame);

private:
    JournalReaderPtr reader;
    JournalWriterPtr writer;
    string strategy_name_;
    int64_t strategy_id_; // 策略ID
    std::shared_ptr<std::thread> thread_;
    std::set<std::string> all_instrument_;
};



