// Copyright 2020 Fancapital Inc.  All rights reserved.
#pragma once
#ifndef DATATYPE_H
#define DATATYPE_H

#include <list>
#include <vector>
#include <set>
#include <map>
#include <deque>
#include <algorithm>
#include <string.h>
#include <chrono>
#include <memory>
#include <atomic>
#include <thread>
#include <iostream>
#include <chrono>
#include <unordered_map>
#include <boost/atomic.hpp>
#include <boost/thread/thread.hpp>
#include <boost/lockfree/spsc_queue.hpp>
#include <boost/lockfree/queue.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/lock_guard.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#define TRADE_ORDER_REQ 30001
#define TRADE_WITHDRAQ_REQ 30002

#define API_RSP_QRY_ACCOUT 30011
#define API_RSP_QRY_POSITION 30012
#define API_RSP_QRY_ORDER 30013
#define API_RSP_QRY_TRADE 30014

#define TRADE_ORDER_REP 30020
#define TRADE_WITHDRAW_REP 30021
#define INNER_TRADE_ORDER 30022
#define INNER_TRADE_KNOCK 30023

#define FEEDER_TICK 31001
#define FEEDER_ORDER 31002
#define FEEDER_TRADE 31003

#define ONLY_LOG 32001

using namespace std;
using namespace boost::gregorian;

// ------------------------------------------行情--------------------------------------//
struct QTickT {
    int8_t src;
    int8_t dtype;
    int64_t timestamp;
    char code[16];
    char name[16];
    int8_t market;
    double pre_close;
    double upper_limit;
    double lower_limit;
    double bp[10];
    int64_t bv[10];
    double ap[10];
    int64_t av[10];
    int8_t status;
    double new_price;
    int64_t new_volume;
    double new_amount;
    int64_t sum_volume;
    double sum_amount;
    double open;
    double high;
    double low;
    double avg_bid_price;
    double avg_ask_price;
    int64_t new_bid_volume;
    double new_bid_amount;
    int64_t new_ask_volume;
    double new_ask_amount;
    int64_t open_interest;
    double pre_settle;
    int64_t pre_open_interest;
    double close;
    double settle;
    int64_t multiple;
    double price_step;
    int32_t create_date;
    int32_t list_date;
    int32_t expire_date;
    int32_t start_settle_date;
    int32_t end_settle_date;
    int32_t exercise_date;
    double exercise_price;
    int8_t cp_flag;
    char underlying_code[8];
    int64_t sum_bid_volume;
    double sum_bid_amount;
    int64_t sum_ask_volume;
    double sum_ask_amount;
    int64_t bid_order_volume;
    double bid_order_amount;
    int64_t bid_cancel_volume;
    double bid_cancel_amount;
    int64_t ask_order_volume;
    double ask_order_amount;
    int64_t ask_cancel_volume;
    double ask_cancel_amount;
    int64_t new_knock_count;
    int64_t sum_knock_count;
    int64_t local_time;
    int64_t write_time;
    int64_t receive_time;
};

struct QOrderT {
    char code[16];
    int64_t timestamp;
    int64_t order_no;
    int8_t bs_flag;
    int8_t order_type;
    int64_t order_price;
    int64_t order_volume;
    int64_t local_time;
};

struct QKnockT {
    char code[16];
    int64_t timestamp;
    int64_t match_no;
    int8_t bs_flag;
    int64_t ask_order_no;
    int64_t bid_order_no;
    int64_t match_price;
    int64_t match_volume;
    double match_amount;
    int64_t local_time;
};

// ------------------------------------------交易--------------------------------------//

// TradeAsset 交易资金
struct TradeAsset {
    char _id[64]; // 唯一标示：<timestamp>-<fund_id>，timestamp只精确到日期
    int64_t timestamp; // 时间戳，示例：20180728231340100
    int64_t trade_type; // 交易类型：1-现货，2-期货，3-期权
    char fund_id[16]; // 资金账号
    double balance; // 资金余额
    double usable; // 可用资金
    double margin; // [期货/期权] 保证金
    double equity; // [期货/期权] 动态权益（资金余额+保证金）
    double freezed; // 冻结资金
    bool islast; //是否最后一条
};

// TradePosition 交易持仓
struct TradePosition {
    char _id[64]; // 唯一标示：<timestamp>-<fund_id>-<code>，timestamp只精确到日期
    int64_t timestamp; // 时间戳，示例：20180728231340100
    int64_t trade_type; // 交易类型：1-现货，2-期货，3-期权
    char fund_id[16]; // 资金账号
    int64_t hedge_flag; // [期货] 套保标记：1-投机，2-套利，3-套保
    int64_t market; // 市场代码：0-未知，1-上海，2-深圳，3-上海B，4-深圳B，5-中金所，6-上期所，7-大商所，8-郑商所，9-特转A，10-特转B，11-三板
    char code[16]; // 证券代码
    char name[16]; // 证券名称
    int64_t buy_volume; // 买入数量
    double buy_market_value; // 买入市值
    int64_t buy_can_close; // 买入可平仓数量
    int64_t sell_volume; // 卖出数量
    double sell_market_value; // 卖出市值
    int64_t sell_can_close; // 卖出可平仓数量
    int64_t buy_pre_volume; // 买入昨持仓
    int64_t sell_pre_volume; // 卖出昨持仓
    bool islast; //是否最后一条
};

// TradeOrder 交易委托
struct TradeOrder {
    char _id[64]; // 唯一标示：<timestamp>-<fund_id>-<inner_order_no>
    int64_t timestamp; // 时间戳，示例：20180728231340100
    int64_t trade_type; // 交易类型：1-现货，2-期货，3-期权
    char fund_id[16]; // 资金账号
    char username[16]; // 用户名
    char inner_order_no[64]; // 内部委托合同号（唯一标示）
    char order_no[64]; // 委托合同号
    char batch_no[64]; // 批次号
    int64_t market; // 市场代码：0-未知，1-上海，2-深圳，3-上海B，4-深圳B，5-中金所，6-上期所，7-大商所，8-郑商所，9-特转A，10-特转B，11-三板
    char code[16]; // 证券代码
    char name[16]; // 证券名称
    int64_t hedge_flag; // [期货] 套保标记：1-投机, 2-套利, 3-套保
    int64_t bs_flag; // 买卖方向，1-买, 2-卖, 3-申购, 4-赎回
    int64_t oc_flag; // [期货/期权] 开平仓标记，1：开仓，2：平仓
    int64_t volume; // 委托数量
    double price; // 委托价格
    int64_t price_type; // 报单类型：0-限价单，1-涨停价，2-卖五价，3-卖四价，4-卖三价，5-卖二价，6-卖一价，7-买一价，8-买二价，9-买三价，10-买四价，11-买五价，12-跌停价
    int64_t state; // 委托状态：0：未报，1：待报，(2：已报)，3：已报待撤，4：部成待撤，(5：部撤)，(6：已撤)，(7：部成)，(8：已成)，(9：废单)
    int64_t match_volume; // 成交数量
    double match_price; // 成交均价
    double match_amount; // 成交金额
    int64_t withdraw_volume; // 撤单数量
    char policy_type[16]; // 策略类型
    char policy_id[16]; // 策略编号
    char tags[16]; // 标签，示例：{"version": "v1.0.0", "mac": "EF:00:01:02:DA:AB"}
    char msg[64]; // 消息
    int64_t order_ref; // 本地报单编号
    int64_t strategy_id; // 策略ID
    int64_t sub_strategy_id; // 子策略ID
    int64_t write_time;
    int64_t receive_time;
    bool islast; //是否最后一条
};

// TradeKnock 交易成交
struct TradeKnock {
    char _id[64]; // 唯一标示：<timestamp>-<fund_id>-<inner_match_no>
    int64_t timestamp; // 时间戳，示例：20180728231340100
    int64_t trade_type; // 交易类型：1-现货，2-期货，3-期权
    char fund_id[16]; // 资金账号
    char username[16]; // 用户名
    char inner_match_no[32]; // 内部成交合同号（唯一标示）
    char match_no[32]; // 成交合同号
    int64_t market; // 市场代码：0-未知，1-上海，2-深圳，3-上海B，4-深圳B，5-中金所，6-上期所，7-大商所，8-郑商所，9-特转A，10-特转B，11-三板
    char code[16]; // 证券代码
    char name[16]; // 证券名称
    char order_no[64]; // 委托合同号
    char batch_no[32]; // 批次号
    int64_t hedge_flag; // [期货] 套保标记：1-投机，2-套利，3-套保
    int64_t bs_flag; // 买卖方向：1:买入，2：卖出
    int64_t oc_flag; // [期货/期权] 开平仓标记，1：开仓，2：平仓
    int64_t match_type; // 成交类型：1：买卖成功，2：撤单成功, 3：买卖废单，4：撤单废单
    int64_t match_volume; // 成交数量
    double match_price; // 成交价格
    double match_amount; // 成交金额
    char msg[64]; // 备注
    int64_t order_ref; // 本地报单编号
    int64_t strategy_id; // 策略ID
    int64_t sub_strategy_id; // 子策略ID
    int64_t local_time;
    bool islast; //是否最后一条
};

// QueryTradeAssetReq 查询资金请求
struct QueryTradeAssetReq {
    char request_id[64]; // 请求编号
    int64_t request_time; // 请求时间戳，20160121091501001
    int64_t request_ttl; // 请求生命周期（单位：毫秒）
    int64_t trade_type; // 交易类型：1-现货，2-期货，3-期权
    char fund_id[16]; // 资金账号
};

// QueryTradeAssetRep 查询资金响应
struct QueryTradeAssetRep {
    char request_id[64]; // 请求编号
    char error[64]; // 错误消息
    TradeAsset item;
};

// QueryTradePositionReq 查询持仓请求
struct QueryTradePositionReq {
    char request_id[64]; // 请求编号
    int64_t request_time; // 请求时间戳，20160121091501001
    int64_t request_ttl; // 请求生命周期（单位：毫秒）
    int64_t trade_type; // 交易类型：1-现货，2-期货，3-期权
    char fund_id[16]; // 资金账号
    char cursor[32]; // 查询游标
};

// QueryTradePositionRep 查询持仓响应
struct QueryTradePositionRep {
    char request_id[64]; // 请求编号
    char error[64]; // 错误消息
    char next_cursor[32]; // 后续数据的游标地址，为空表示查询结束
    TradePosition item;
};

// QueryTradeOrderReq 查询委托请求
struct QueryTradeOrderReq {
    char request_id[64]; // 请求编号
    int64_t request_time; // 请求时间戳，20160121091501001
    int64_t request_ttl; // 请求生命周期（单位：毫秒）
    int64_t trade_type; // 交易类型：1-现货，2-期货，3-期权
    char fund_id[16]; // 资金账号
    char cursor[32]; // 查询游标
};

// QueryTradeOrderRep 查询委托响应
struct QueryTradeOrderRep {
    char request_id[64]; // 请求编号
    char error[64]; // 错误消息
    char next_cursor[32]; // 后续数据的游标地址，为空表示查询结束
    TradeOrder item;
};

// QueryTradeKnockReq 查询成交请求
struct QueryTradeKnockReq {
    char request_id[64]; // 请求编号
    int64_t request_time; // 请求时间戳，20160121091501001
    int64_t request_ttl; // 请求生命周期（单位：毫秒）
    int64_t trade_type; // 交易类型：1-现货，2-期货，3-期权
    char fund_id[16]; // 资金账号
    char cursor[32]; // 查询游标
};

// QueryTradeKnockRep 查询成交响应
struct QueryTradeKnockRep {
    char request_id[64]; // 请求编号
    char error[64]; // 错误消息
    char next_cursor[32]; // 后续数据的游标地址，为空表示查询结束
    TradeKnock item;
};

// TradeOrderReq 委托请求
struct TradeOrderReq {
    char request_id[64]; // 请求编号
    int64_t request_time; // 请求时间戳，20160121091501001
    int64_t request_ttl; // 请求生命周期（单位：毫秒）
    char token[32]; // 访问令牌
    TradeOrder item; // 必填字段：fund_id,code,bs_flag,oc_flag,volume,price, 选填字段：username,price_type,policy_type,policy_id,tags
};

// TradeOrderRep 委托响应
struct TradeOrderRep {
    char request_id[64]; // 请求编号
    char error[64]; // 错误消息，不为空表示所有委托都报单失败
    char batch_no[64]; // 委托批次号，单笔委托为空
    char order_no[64]; // 所有委托的合同号列表，与委托一一对应，废单的order_no为空字符串
    //char errors[256]; // 所有委托的废单原因列表，与委托一一对应，成功委托的error为空字符串
    char code[16]; // 证券代码
    int64_t order_ref; // 本地报单编号
    int64_t strategy_id; // 策略ID
    int64_t sub_strategy_id; // 子策略ID
    int64_t local_time;
};

// TradeWithdrawReq 撤单请求
struct TradeWithdrawReq {
    char request_id[64]; // 请求编号
    char token[32]; // 访问令牌
    int64_t trade_type; // 交易类型：1-现货，2-期货，3-期权
    char fund_id[16]; // 资金账号
    char order_no[64]; // 委托合同号（单笔撤单时有效）
    char batch_no[64]; // 委托批次号（批量撤单时有效）
    bool all; // 是否对整个资金账户所有挂单委托进行撤单（账户全撤时有效）
    int64_t market; // 市场代码
    int64_t strategy_id; // 策略ID
    int64_t sub_strategy_id; // 子策略ID
    int64_t local_time;
};

// TradeWithdrawRep 撤单响应
struct TradeWithdrawRep {
    char request_id[64]; // 请求编号
    char error[64]; // 错误消息
    char order_no[64]; // 委托合同号
    char batch_no[64]; // 委托批次号
    char msg[64]; // 撤单信息（全撤时返回撤单委托数量等信息）
    char code[16]; // 证券代码
    int64_t order_ref; // 本地报单编号
    int64_t strategy_id; // 策略ID
    int64_t sub_strategy_id; // 子策略ID
    int64_t local_time;
};

struct ErrorMsg {
    int error_id;
    char error_msg[128];
};

struct RspErrorOrder {
    TradeOrderReq req;
    ErrorMsg err;
};

struct RspErrorWithdraw {
    TradeWithdrawReq req;
    ErrorMsg err;
};

struct InstrumentInfo {
    char code_[16];
    char name_[16];
    int market_;
    int type_;
    int status_;
    int min_market_order_volume;
    int max_market_order_volume;
    int min_limit_order_volume;
    int max_limit_order_volume;
    int volume_multiple;
    double price_tick;

    InstrumentInfo() {

    }

    InstrumentInfo(const char* code, const char* name, int market, int type, int status) : market_(market),
           type_(type), status_(status){
        strncpy(code_, code, strlen(code));
        strncpy(name_, name, strlen(name));
    }
};

struct StrategyID {
    int64_t strategy_id; // 策略ID
    int64_t sub_strategy_id; // 子策略ID
    bool order_rep_flag;    // local_order_ref 的回报
    StrategyID(int64_t strategy_id1, int64_t sub_strategy_id1, bool order_rep) : strategy_id(strategy_id1),
              sub_strategy_id(sub_strategy_id1), order_rep_flag(order_rep) {
        // order_rep_flag = false;
    }
};

//typedef std::shared_ptr<QueryTradeAssetReq> QueryTradeAssetReqPtr;
//typedef std::shared_ptr<QueryTradeAssetRep> QueryTradeAssetRepPtr;
//typedef std::shared_ptr<QueryTradePositionReq> QueryTradePositionReqPtr;
//typedef std::shared_ptr<QueryTradePositionRep> QueryTradePositionRepPtr;
//typedef std::shared_ptr<QueryTradeOrderReq> QueryTradeOrderReqPtr;
//typedef std::shared_ptr<QueryTradeOrderRep> QueryTradeOrderRepPtr;
//typedef std::shared_ptr<QueryTradeKnockReq> QueryTradeKnockReqPtr;
//typedef std::shared_ptr<QueryTradeKnockRep> QueryTradeKnockRepPtr;
//typedef std::shared_ptr<TradeOrderReq> TradeOrderReqPtr;
//typedef std::shared_ptr<TradeOrderRep> TradeOrderRepPtr;
//typedef std::shared_ptr<TradeWithdrawReq> TradeWithdrawReqPtr;
//typedef std::shared_ptr<TradeWithdrawRep> TradeWithdrawRepPtr;
//typedef std::shared_ptr<InstrumentInfo> QStockPtr;


struct SpinLock {
    explicit SpinLock(std::atomic_flag& flag) : m_flag(flag) {
        while (m_flag.test_and_set(std::memory_order_acquire)) {
        }
    }

    ~SpinLock() {
        m_flag.clear(std::memory_order_release);
    }

 private:
    std::atomic_flag& m_flag;
};

#endif /* DATATYPE_H */

