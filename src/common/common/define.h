#ifndef DEFINE_COMMON_H_
#define DEFINE_COMMON_H_

#include <sstream>
#include <string>
#include <memory>

using namespace std;

constexpr double kEpsilon = 0.000001;
constexpr int64_t kQScale = 10000; // 行情浮点数扩大倍率
constexpr char kSep = '\t';

constexpr int kSecurityTypeUnknown = 0; // 证券类型：未知
constexpr int kSecurityTypeStock = 101; // 证券类型：股票
constexpr int kSecurityTypeFund = 102; // 证券类型：基金
constexpr int kSecurityTypeBond = 103; // 证券类型：债券
constexpr int kSecurityTypeRepo = 104; // 证券类型：逆回购
constexpr int kSecurityTypeIndex = 105; // 证券类型：指数
constexpr int kSecurityTypeFuture = 200; // 证券类型：期货
constexpr int kSecurityTypeOption = 300; // 证券类型：期权

// --------------------------------------------------
constexpr int kMarketSH = 1; // 市场代码：上海
constexpr int kMarketSZ = 2; // 市场代码：深圳
constexpr int kMarketCFFEX = 5; // 市场代码：中金所
constexpr int kMarketSHFE = 6; // 市场代码：上期所
constexpr int kMarketDCE = 7; // 市场代码：大商所
constexpr int kMarketCZCE = 8; // 市场代码：郑商所
constexpr int kMarketHK = 9; // 市场代码：港交所
constexpr int kMarketINE = 21; // 市场代码：上海国际能源交易中心
constexpr int kMarketSGE = 22; // 市场代码：上海黄金交易所
constexpr int kMarketCSI = 23; // 市场代码：中证指数

const string kSuffixSH = ".SH"; // 代码后缀：上海
const string kSuffixSZ = ".SZ"; // 代码后缀：深圳
const string kSuffixCFFEX = ".CFFEX"; // 代码后缀：中金所
const string kSuffixSHFE = ".SHFE"; // 市场后缀：上期所
const string kSuffixDCE = ".DCE"; // 市场后缀：大商所
const string kSuffixCZCE = ".CZCE"; // 市场后缀：郑商所
const string kSuffixHK = ".HK"; // 市场后缀：港交所
const string kSuffixINE = ".INE"; // 市场后缀：上海国际能源交易中心
const string kSuffixSGE = ".SGE"; // 市场后缀：上海黄金交易所
const string kSuffixCSI = ".CSI"; // 市场后缀：中证指数


// --------------------------------------------------
// [0] 禁止现金替代：不允许使用现金替代
constexpr int kCashReplaceDisabled = 0;
// [1] 允许现金替代：优先使用股票，股票不足才使用现金替代
constexpr int kCashReplaceEnabled = 1;
// [2] 必须现金替代：必须使用现金替代，比如股票停牌的情况下。
constexpr int kCashReplaceMust = 2;
// [3] （跨市场）退补现金替代：基金公司代买卖股票，现金替代金额多退少补。
constexpr int kCashReplaceCrossMarketEnabled = 3;
// [4] （跨市场）必须现金替代：必须使用现金替代
constexpr int kCashReplaceCrossMarketMust = 4;
// [5] 非沪深退补现金替代（跨境）：基金公司代买卖股票，现金替代金额多退少补。
constexpr int kCashReplaceCrossBorderEnabled = 5;
// [6] 非沪深必须现金替代（跨境）：必须使用现金替代
constexpr int kCashReplaceCrossBorderMust = 6;
// --------------------------------------------------

// --------------------------------------------------
// 行情
// --------------------------------------------------
const string kPrefixQStock = "S"; // 股票行情代码前缀
const string kPrefixQIndex = "I"; // 指数行情代码前缀
const string kPrefixQFuture = "F"; // 期货行情代码前缀
const string kPrefixQOption = "O"; // 期权行情代码前缀
const string kPrefixQOrder = "D"; // 逐笔委托行情前缀
const string kPrefixQQueue = "Q"; // 委托队列前缀
const string kPrefixQKnock = "T"; // 逐笔成交行情前缀
const string kPrefixQEtfIopv = "V"; // ETF净值行情代码前缀
const string kPrefixQGradedFundIopv = "C"; // 分级基金净值前缀
const string kPrefixQOptionVolatility = "P"; // 期权隐含波动率前缀

constexpr char kFBPrefixQTick = 'a'; // FB协议行情快照前缀
constexpr char kFBPrefixQOrder = 'b'; // FB协议逐笔委托前缀
constexpr char kFBPrefixQKnock = 'c'; // FB协议逐笔成交前缀
constexpr char kFBPrefixQQueue = 'd'; // FB协议委托队列前缀
constexpr char kFBPrefixQEtfIopv = 'e'; // FB协议ETF净值前缀

constexpr char kSrcQTickLevel2 = 0x00; // 行情清洗数据源：Level2行情快照
constexpr char kSrcQTickLevel1 = 0x01; // 行情清洗数据源：Level1行情快照
constexpr char kSrcQTickHolo = 0x02; // 行情清洗数据源：全息行情快照
constexpr char kSrcQOrder = 0x03; // 行情清洗数据源：逐笔委托
constexpr char kSrcQKnock = 0x04; // 行情清洗数据源：逐笔成交
constexpr char kSrcQQueue = 0x05; // 行情清洗数据源：委托队列

constexpr char kDTypeStock = 0x01; // 行情数据类型：股票
constexpr char kDTypeFuture = 0x02; // 行情数据类型：期货
constexpr char kDTypeOption = 0x03; // 行情数据类型：期权
constexpr char kDTypeIndex = 0x04; // 行情数据类型：指数
constexpr char kDTypeOrder = 0x05; // 行情数据类型：逐笔委托
constexpr char kDTypeKnock = 0x06; // 行情数据类型：逐笔成交
constexpr char kDTypeQueue = 0x07; // 行情数据类型：委托队列

constexpr int kStateOK = 0; // 正常
constexpr int kStateSuspension = 1; // 停牌
constexpr int kStateLimitUp = 2; // 涨停
constexpr int kStateLimitDown = 3; // 停跌
constexpr int kStateBreak = 4; // 熔断

constexpr char kQOrderTypeLimit = 0x00; // 逐笔委托类型：限价单
constexpr char kQOrderTypeMarket = 0x01; // 逐笔委托类型：市价单（对手价）
constexpr char kQOrderTypeSelfMarket = 0x02; // 逐笔委托类型：本方最优市价单

// --------------------------------------------------
// 交易
// --------------------------------------------------

// TradeTypeSpot 交易类型：现货
constexpr int kTradeTypeSpot = 1;
// TradeTypeFuture 交易类型：期货
constexpr int kTradeTypeFuture = 2;
// TradeTypeOption 交易类型：期权
constexpr int kTradeTypeOption = 3;
// TradeTypeCoin 交易类型：数字货币
constexpr int kTradeTypeCoin = 101;


constexpr int kOrderNotSend = 0; // 委托状态：未报
constexpr int kOrderCreating = 1; // 委托状态：待报
constexpr int kOrderCreated = 2; // 委托状态：已报
constexpr int kOrderCanceling = 3; // 委托状态：已报撤单
constexpr int kOrderPartlyCanceled = 5;  // 委托状态：部分撤单
constexpr int kOrderFullyCanceled = 6;  // 委托状态：已撤单
constexpr int kOrderPartlyKnocked = 7;  // 委托状态：部分成交
constexpr int kOrderFullyKnocked = 8;  // 委托状态：已成交
constexpr int kOrderFailed = 9;  // 委托状态：废单

constexpr int kHedgeFlagSpeculate = 1; // 套保标记：投机
constexpr int kHedgeFlagArbitrage = 2; // 套保标记：套利
constexpr int kHedgeFlagHedge = 3; // 套保标记：套保

constexpr int kBsFlagWithdraw = -1; // 买卖标记：撤单
constexpr int kBsFlagBuy = 1; // 买卖标记：买
constexpr int kBsFlagSell = 2; // 买卖标记：卖
constexpr int kBsFlagCreate = 3; // 买卖标记：申购
constexpr int kBsFlagRedeem = 4; // 买卖标记：赎回

constexpr int kOcFlagAuto = 0; // 开平仓标记：自动开平仓（只存在报单请求中，不存在于最终委托开平仓标记中）
constexpr int kOcFlagOpen = 1; // 开平仓标记：开仓
constexpr int kOcFlagClose = 2; // 开平仓标记：平仓
constexpr int kOcFlagForceClose = 3; // 开平仓标记：强平
constexpr int kOcFlagCloseToday = 4; // 开平仓标记：平今
constexpr int kOcFlagCloseYesterday = 5; // 开平仓标记：平昨
constexpr int kOcFlagForceOff = 6; // 开平仓标记：强减
constexpr int kOcFlagLocalForceClose = 7; // 开平仓标记：本地强平

constexpr int kCpFlagCall = 1; // 认购
constexpr int kCpFlagPut = 2; // 认沽

constexpr int kMatchTypeOK = 1; // 成交类型：买卖申赎成功
constexpr int kMatchTypeWithdrawOK = 2; // 成交类型：撤单成功
constexpr int kMatchTypeFailed = 3; // 成交类型：废单
constexpr int kMatchTypeWithdrawFailed = 4; // 成交类型：撤单废单


enum API_TYPE {
    CTP = 0,
    XELE,
    TORA,
    SIMULATE,
    NONE
};

enum SM_MODEL {
    // 从当前的位置写，且从当前的位置读
    QUICK = 1,
    // 从当前的位置写，且从头开始读
    START,
    // 从头开始读
    RESTART
};

#endif //DEFINE_COMMON_H_