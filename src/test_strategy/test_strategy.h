#pragma once
#include <iostream>
#include <fstream>
#include <string.h>
#include <math.h>
#include <sched.h>
#include <unistd.h>
#include <algorithm>
//#include "../journal/JournalReader.h"
//#include "../journal/JournalWriter.h"
//#include "../journal/FrameHeader.h"
//#include "../journal/Frame.hpp"
//#include "../journal/Timer.h"
//#include "../utils/datastruct.h"

#include "../strategy/strategy.h"

//using yijinjing::JournalReaderPtr;
//using yijinjing::JournalWriterPtr;
//using namespace yijinjing;

// 一个reader, 读feeder 与 broker
// 一个writer, 写broker
class TestStrategy : public Strategy {
public:
    TestStrategy();
    virtual ~TestStrategy();

    virtual void OnInit();
    virtual void OnTick(QTickT* data);
    virtual void OnLevel2Order(QOrderT* data);
    virtual void OnLevel2Knock(QKnockT* data);

    virtual void OnRspQueryAccout(QueryTradeAssetRep* rsp);
    virtual void OnRspQueryPosition(QueryTradePositionRep* rsp);
    virtual void OnRspQueryOrder(QueryTradeOrderRep* rsp);
    virtual void OnRspQueryTrade(QueryTradeKnockRep* rsp);
    virtual void OnRspOrder(TradeOrderRep* rsp);
    virtual void OnRspWithdraw(TradeWithdrawRep* rsp);
    virtual void OnRtnTradeKnock(TradeKnock* rtn);
};

