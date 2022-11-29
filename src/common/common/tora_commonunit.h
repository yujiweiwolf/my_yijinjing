// Copyright 2020 Fancapital Inc.  All rights reserved.
#pragma once
#include <string>
#include "commonunit.h"

#ifdef HUAXINTORA
#include "TORATstpUserApiStruct.h"

class ToraCommonUnit {
    public:
    ToraCommonUnit() {
    }

    ~ToraCommonUnit() {
    }

    static int64_t GetOrderTimestamp(int64_t trading_day, int time) {
        return (trading_day * 1000000000LL + time);
    }

    static int64_t GetTradeTimestamp(int64_t trading_day, int time, char exchangeid) {
        if (exchangeid == '1') {
            return (trading_day * 1000000000LL + time * 10LL);
        }
        else {
            return (trading_day * 1000000000LL + time);
        }
    }

    static int64_t GetTickTimestamp(int64_t trading_day, int time, char exchangeid) {
        if (exchangeid == '1') {
            return (trading_day * 1000000000LL + time * 1000LL);
        }
        else {
            return (trading_day * 1000000000LL + time);
        }
    }

    /// Tora 市价 '1', 限价 '2',本方最优 '3'
    static int8_t GetOrderType(const char& order_type) {
        int8_t std_order_type = 0;
        switch (order_type) {
        case '1':
            std_order_type = kQOrderTypeMarket;
            break;
        case '2':
            std_order_type = kQOrderTypeLimit;
            break;
        case '3':
            std_order_type = kQOrderTypeSelfMarket;
            break;
        default:
            // __error << "unknown order_type: " << order_type;
            break;
        }
        return std_order_type;
    }

    /// Tora 买 '1'， 卖 '2'，借入 '3'，借出 '4'
    static int8_t GetOrderSide(const char& side_type) {
        int8_t std_bs_flag = 0;
        switch (side_type) {
        case '1':
            std_bs_flag = kBsFlagBuy;
            break;
        case '2':
            std_bs_flag = kBsFlagSell;
            break;
        default:
            // __error << "unknown side_type: " << side_type;
            break;
        }
        return std_bs_flag;
    }

    // ExecType 成交 '1', 撤销 '2',未知 'N', TradeBSFlag 外盘,主动买 'B', 内盘,主动卖 'S', 未知 'N'
    static int8_t GetTradeSide(const char& exec_type, const char& trade_flag) {
        int8_t std_bs_flag = 0;
        if (exec_type == '2') {
            std_bs_flag = kBsFlagWithdraw;
        }
        else {
            switch (trade_flag) {
            case 'B':
                std_bs_flag = kBsFlagBuy;
                break;
            case 'S':
                std_bs_flag = kBsFlagSell;
                break;
            default:
                break;
            }
        }
        return std_bs_flag;
    }

    // #define TORA_TSTP_EXD_SSE '1'  TORA_TSTP_EXD_SZSE '2' TORA_TSTP_EXD_HK '3'
    static int Market2Std(TTORATstpExchangeIDType& exchange_id) {
        int market = 0;
        switch (exchange_id) {
        case '1':
            market = kMarketSH;
            break;
        case '2':
            market = kMarketSZ;
            break;
        case '3':
            market = kMarketHK;
            break;
        default:
            break;
        }
        return market;
    }

    /// Tora 开盘前 '0',集合竞价 '1', 连续交易 '2', 休市 '3', 停牌 '4', 长期停牌 '5', 波动性中断 '6',熔断可恢复 '7'
    /// 熔断不可恢复 '8', 闭市 '9', 其它 'a', 收盘集合竞价 'b', 集中撮合(盘后定价)'c', 连续交易(盘后定价)'d'
   /// 闭市(盘后定价)'e',开盘前(盘后定价)'f'
    static int8_t GetSecurityStatus(const char& security_status, const double& upper_limit, const double& lower_limit,
        const double& bp1, const double& ap1) {
        int8_t std_status = kStateOK;
        switch (security_status) {
        case '4':
        case '5':
        case '6':
            std_status = kStateSuspension;
            break;
        case '7':
        case '8':
            std_status = kStateBreak;
            break;
        default:
            if (upper_limit > 0 && CommonUnit::DB_GREAT_EQ(bp1, upper_limit)) {
                std_status = kStateLimitUp;
            }
            else if (lower_limit > 0 && ap1 > 0 && CommonUnit::DB_LESS_EQ(ap1, lower_limit)) {
                std_status = kStateLimitDown;
            }
            break;
        }
        return std_status;
    }

    //-------------------------交易---------------------------------------//
    static string ConvertCharacters(const TTORATstpErrorMsgType msg) {
        //return SafeGBKToUTF8(msg);
        return msg;
    }

    static string GetErrorMsg(TTORATstpErrorIDType code, TTORATstpErrorMsgType msg) {
        stringstream ss;
        ss << code << "-";
        if (msg) {
            ss << ConvertCharacters(msg);
        }
        return ss.str();
    }

    static string GetApiError(int rc) {
        stringstream ss;
        string ret;
        switch (rc) {
        case 0:
            ret = "0-ok";
            break;
        case -1:
            ret = "-1-network error";
            break;
        case -2:
        case -3:
            ss << rc << "-flow control error";
            ret = ss.str();
            break;
        default:
            ss << rc << "-unknown error";
            ret = ss.str();
            break;
        }
        return ret;
    }

    static int64_t GetTimeStamp(TTORATstpDateType date, TTORATstpTimeType time) {
        string s_date = date;
        boost::algorithm::replace_all(s_date, "-", "");
        int64_t i_date = atoll(s_date.c_str());
        string s_time = time;
        boost::algorithm::replace_all(s_time, ":", "");
        int64_t i_time = atoll(s_time.c_str()) * 1000;
        int64_t timestamp = i_date * 1000000000LL + i_time % 1000000000LL;
        return timestamp;
    }

    static int64_t BsFlag2Std(TTORATstpDirectionType type) {
        int64_t std_bs_flag = 0;
        switch (type) {
        case TORA_TSTP_D_Buy:  // 买
            std_bs_flag = kBsFlagBuy;
            break;
        case TORA_TSTP_D_Sell:  // 卖
            std_bs_flag = kBsFlagSell;
            break;
        case TORA_TSTP_D_ETFPur:  // 申购
            std_bs_flag = kBsFlagCreate;
            break;
        case TORA_TSTP_D_ETFRed:  // 赎回
            std_bs_flag = kBsFlagRedeem;
            break;
        default:
            //__warn << "not parse bs type: " << type;
            break;
        }
        return std_bs_flag;
    }

    static int64_t OrderStatus2Std(TTORATstpOrderStatusType order_state, TTORATstpOrderSubmitStatusType submit_state) {
        // 委托状态：0：未报，1：待报，(2：已报)，3：已报待撤，4：部成待撤，(5：部撤)，(6：已撤)，(7：部成)，(8：已成)，(9：废单)
        int64_t std_order_state = 0;
        switch (order_state) {
        case TORA_TSTP_OST_AllTraded:  /// 全部成交
            std_order_state = kOrderFullyKnocked;
            break;
        case TORA_TSTP_OST_PartTradedQueueing:  /// 部分成交还在队列中
            std_order_state = kOrderPartlyKnocked;
            break;
        case TORA_TSTP_OST_PartTradedNotQueueing:  /// 部分成交不在队列中，即“部成部撤”
            std_order_state = kOrderPartlyCanceled;
            break;
        case TORA_TSTP_OST_NoTradeQueueing:  /// 未成交还在队列中
            std_order_state = kOrderCreated;
            break;
        case TORA_TSTP_OST_NoTradeNotQueueing:  /// 未成交不在队列中
            std_order_state = kOrderFailed;
            break;
        case TORA_TSTP_OST_Canceled:  /// 撤单
             // 撤单已经被拒绝, 撤单未提交，报单已经被拒绝(应该是被交易所拒绝)
            if (submit_state == TORA_TSTP_OSS_CancelRejected || submit_state == TORA_TSTP_OSS_CancelUnSubmitted || submit_state == TORA_TSTP_OSS_InsertRejected) {
                std_order_state = kOrderFailed;
            }
            else {
                std_order_state = kOrderFullyCanceled;
            }
            break;
        case TORA_TSTP_OST_Unknown:  /// 未知
            std_order_state = kOrderNotSend;
            break;
        case TORA_TSTP_OST_NotTouched:  /// 尚未触发
            std_order_state = kOrderNotSend;
            break;
        case TORA_TSTP_OST_Touched:  /// 已触发
            std_order_state = kOrderCreated;
            break;
        case TORA_TSTP_OST_Cached:  /// 预埋
            std_order_state = kOrderNotSend;
            break;
        default:
            //__warn << "not parse order_state: " << order_state << ", submit_state: " << submit_state;
            break;
        }
        return std_order_state;
    }

    static int64_t OrderPriceType2Std(TTORATstpOrderPriceTypeType price_type) {
        int64_t ret = 0;
        switch (price_type) {
        case TORA_TSTP_OPT_AnyPrice:
            ret = 1;  /// 任意价
            break;
        case TORA_TSTP_OPT_LimitPrice:
            ret = 2;   /// 限价
            break;
        case TORA_TSTP_OPT_BestPrice:
            ret = 3;   /// 最优价
            break;
        case TORA_TSTP_OPT_FixPrice:
            ret = 8;  /// 盘后定价
            break;
        case TORA_TSTP_OPT_FiveLevelPrice:
            ret = 9;  /// 五档价
            break;
        case TORA_TSTP_OPT_HomeBestPrice:
            ret = 10;  /// 本方最优
            break;
        default:
            //__warn << "not parse order price type: " << price_type;
            break;
        }
        return ret;
    }

    static int64_t GetMarket(const string& code) {
        int64_t std_market = 0;
        string suffix = code.substr(code.length() - 3);
        if (suffix == kSuffixSH) {
            std_market = kMarketSH;
        }
        else if (suffix == kSuffixSZ) {
            std_market = kMarketSZ;
        }
        else if (suffix == kSuffixHK) {
            std_market = kMarketHK;
        }
        else {
            // __warn << "not parse code: " << code;
        }
        return std_market;
    }

    static TTORATstpExchangeIDType StdMarket2Tora(int exchange_id) {
        TTORATstpExchangeIDType market = TORA_TSTP_EXD_COMM;
        switch (exchange_id) {
        case kMarketSH:
            market = TORA_TSTP_EXD_SSE;
            break;
        case kMarketSZ:
            market = TORA_TSTP_EXD_SZSE;
            break;
        case kMarketHK:
            market = TORA_TSTP_EXD_HK;
            break;
        default:
            // __warn << "not parse std market: " << exchange_id;
            break;
        }
        return market;
    }

    static TTORATstpDirectionType StdBsFlag2Tora(int64_t bs_flag) {
        TTORATstpDirectionType tora_bs_flag = '0';
        switch (bs_flag) {
        case kBsFlagBuy:
            tora_bs_flag = TORA_TSTP_D_Buy;
            break;
        case kBsFlagSell:
            tora_bs_flag = TORA_TSTP_D_Sell;
            break;
        case kBsFlagCreate:
            tora_bs_flag = TORA_TSTP_D_ETFPur;
            break;
        case kBsFlagRedeem:
            tora_bs_flag = TORA_TSTP_D_ETFRed;
            break;
        default:
            // __warn << "not parse std bs flag: " << bs_flag;
            break;
        }
        return tora_bs_flag;
    }

    static int SecurityStatus2Std(TTORATstpSecurityStatusType type) {
        int status = 0;
        switch (type) {
        case 0x0000:
            status = kStateOK;
            break;
        case 0x0001:
            status = kStateSuspension;
            break;
        case 0x0040:
            status = kStateSuspension;
            break;
        default:
            break;
        }
        return status;
    }
};

#endif // HUAXINFEEDERTORA