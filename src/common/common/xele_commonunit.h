// Copyright 2020 Fancapital Inc.  All rights reserved.
#pragma once
#include <string>
#include "commonunit.h"

#ifdef MANTAXELE
#include "CXeleFtdcUserApiDataType.h"

class XeleCommonUnit {
    public:
    XeleCommonUnit() {
    }

    ~XeleCommonUnit() {
    }

    static int64_t OrderStatus2Std(TXeleFtdcOrderStatusType status) {
        int64_t std_order_state = 0;
        switch (status) {
        case XELE_FTDC_OST_AllTraded:  /// 全部成交
            std_order_state = kOrderFullyKnocked;
            break;
        case XELE_FTDC_OST_PartTradedQueueing:  /// 部分成交还在队列中
            std_order_state = kOrderPartlyKnocked;
            break;
        case XELE_FTDC_OST_PartTradedNotQueueing:  /// 部成部撤(部分成交不在队列中)
            std_order_state = kOrderPartlyCanceled;
            break;
        case XELE_FTDC_OST_Canceled:  /// 已撤
            std_order_state = kOrderFullyCanceled;
            break;
        case XELE_FTDC_OST_NoTradeQueueing:  /// 未成交还在队列中
            std_order_state = kOrderCreated;
            break;
        case XELE_FTDC_OST_NoTradeNotQueueing:  /// 废单(未成交不在队列中)
            std_order_state = kOrderFailed;
            break;
        default:
            break;
        }
        return std_order_state;
    }

    static int64_t GetOrderTimeStamp(TXeleFtdcDateType InsertDate, TXeleFtdcTimeType InsertTime) {
        string s_time = InsertTime;
        boost::algorithm::replace_all(s_time, ":", "");
        int64_t i_time = atoll(s_time.c_str()) * 1000LL;
        int64_t timestamp = atoll(InsertDate) * 1000000000LL + i_time;
        return timestamp;
    }

    static int64_t GetTradeTimeStamp(int64_t date, TXeleFtdcTimeType TradeTime) {
        string s_time = TradeTime;
        boost::algorithm::replace_all(s_time, ":", "");
        int64_t i_time = atoll(s_time.c_str()) * 1000LL;
        int64_t timestamp = date * 1000000000LL + i_time;
        return timestamp;
    }

    static int64_t BsFlag2Std(TXeleFtdcDirectionType direction) {
        int64_t std_bs_flag = 0;
        switch (direction) {
        case XELE_FTDC_D_Buy:  // 多头
            std_bs_flag = kBsFlagBuy;
            break;
        case XELE_FTDC_D_Sell:  // 空头
            std_bs_flag = kBsFlagSell;
            break;
        default:
            break;
        }
        return std_bs_flag;
    }

    static int64_t OcFlag2Std(char flag) {
        int64_t std_oc_flag = 0;
        switch (flag) {
        case XELE_FTDC_OF_Open:  /// 开仓
            std_oc_flag = kOcFlagOpen;
            break;
        case XELE_FTDC_OF_Close:  /// 平仓
            std_oc_flag = kOcFlagClose;
            break;
        case XELE_FTDC_OF_ForceClose:  /// 强平
            std_oc_flag = kOcFlagForceClose;
            break;
        case XELE_FTDC_OF_CloseToday:  /// 平今
            std_oc_flag = kOcFlagCloseToday;
            break;
        case XELE_FTDC_OF_CloseYesterday:  /// 平昨
            std_oc_flag = kOcFlagCloseYesterday;
            break;
        default:
            break;
        }
        return std_oc_flag;
    }

    static int64_t HedgeFlag2Std(TXeleFtdcHedgeFlagType type) {
        int64_t std_hedge_flag = 0;
        switch (type) {
        case XELE_FTDC_HF_Speculation:
            std_hedge_flag = kHedgeFlagSpeculate;
            break;
        case XELE_FTDC_HF_Arbitrage:
            std_hedge_flag = kHedgeFlagArbitrage;
            break;
        case XELE_FTDC_HF_Hedge:
            std_hedge_flag = kHedgeFlagHedge;
            break;
        default:
            break;
        }
        return std_hedge_flag;
    }

    static char Std2BsFlag(int64_t direction) {
        char bs_flag = 'a';
        switch (direction) {
        case kBsFlagBuy:
            bs_flag = XELE_FTDC_D_Buy;
            break;
        case kBsFlagSell:
            bs_flag = XELE_FTDC_D_Sell;
            break;
        default:
            break;
        }
        return bs_flag;
    }

    static char Std2OcFlag(int64_t flag) {
        char oc_flag = 'a';
        switch (flag) {
        case kOcFlagOpen:
            oc_flag = XELE_FTDC_OF_Open;
            break;
        case kOcFlagClose:
            oc_flag = XELE_FTDC_OF_Close;
            break;
        case kOcFlagForceClose:
            oc_flag = XELE_FTDC_OF_ForceClose;
            break;
        case kOcFlagCloseToday:
            oc_flag = XELE_FTDC_OF_CloseToday;
            break;
        case kOcFlagCloseYesterday:
            oc_flag = XELE_FTDC_OF_CloseYesterday;
            break;
        default:
            break;
        }
        return oc_flag;
    }

    static char HedgeFlag2Std(int64_t flag) {
        char hedge_flag = 'a';
        switch (flag) {
        case kHedgeFlagSpeculate:
            hedge_flag = XELE_FTDC_HF_Speculation;
            break;
        case kHedgeFlagArbitrage:
            hedge_flag = XELE_FTDC_HF_Arbitrage;
            break;
        case kHedgeFlagHedge:
            hedge_flag = XELE_FTDC_HF_Hedge;
            break;
        default:
            break;
        }
        return hedge_flag;
    }
};

#endif // MANTAXELE