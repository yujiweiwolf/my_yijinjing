
    #include "test_strategy.h"

    TestStrategy::TestStrategy() {
    }

    TestStrategy::~TestStrategy() {
    }

    void TestStrategy::OnInit() {
        std::cout << "TestStrategy::OnInit" << std::endl;
    }

    void TestStrategy::OnTick(QTickT* tick) {
        std::cout << " code: " << tick->code
                  << ", new_price: " << tick->new_price
                  << ", new_volume: " << tick->new_volume << std::endl;
    }

    void TestStrategy::OnLevel2Order(QOrderT* data) {

    }

    void TestStrategy::OnLevel2Knock(QKnockT* data) {

    }

    void TestStrategy::OnRspQueryAccout(QueryTradeAssetRep* rsp) {

    }

    void TestStrategy::OnRspQueryPosition(QueryTradePositionRep* rsp) {

    }

    void TestStrategy::OnRspQueryOrder(QueryTradeOrderRep* rsp) {

    }

    void TestStrategy::OnRspQueryTrade(QueryTradeKnockRep* rsp) {

    }

    void TestStrategy::OnRspOrder(TradeOrderRep* rsp) {

    }

    void TestStrategy::OnRspWithdraw(TradeWithdrawRep* rsp) {

    }

    void TestStrategy::OnRtnTradeKnock(TradeKnock* rtn) {

    }





