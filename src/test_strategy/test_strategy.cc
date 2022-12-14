
#include "test_strategy.h"

namespace yijinjing {
    TestStrategy::TestStrategy() {
    }

    TestStrategy::~TestStrategy() {
    }

    void TestStrategy::OnInit() {
        std::cout << "TestStrategy::OnInit" << std::endl;
    }

    void TestStrategy::OnTick(QTickT *tick) {
        std::cout << " code: " << tick->code
                  << ", new_price: " << tick->new_price
                  << ", new_volume: " << tick->new_volume << std::endl;
    }

    void TestStrategy::OnRspQueryAccout(QueryTradeAssetRep *rsp) {

    }

    void TestStrategy::OnRspQueryPosition(QueryTradePositionRep *rsp) {

    }

    void TestStrategy::OnRspQueryTrade(QueryTradeKnockRep *rsp) {

    }

    void TestStrategy::OnRspOrder(TradeOrderMessage *rsp) {

    }

    void TestStrategy::OnRspWithdraw(TradeWithdrawMessage *rsp) {

    }

    void TestStrategy::OnRtnTradeKnock(TradeKnock *rtn) {

    }
}





