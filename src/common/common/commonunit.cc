// Copyright 2020 Fancapital Inc.  All rights reserved.

#include "./commonunit.h"

#ifdef _MSC_VER

#else
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <sys/time.h>
#include <cstdlib>
#include <vector>
#include <sys/select.h>
#include <errno.h>
#include "iconv.h"
#include <string>
#include <sys/select.h>
#include <signal.h>
#include <errno.h>
#include <dirent.h>
#include <map>
#include <fstream>
#include <sstream>
#include <unistd.h>

string iconv(const string& fromcode, const string& tocode, const string& str, const size_t& buf_size) {
    // @TODO 转换海量数据时有问题，比如一个完整的电子书
    if (str.empty()) {
        return "";
    }
    iconv_t cd = iconv_open(tocode.c_str(), fromcode.c_str()); // create conversion descriptor
    if (cd == (iconv_t) - 1) {
        // xthrow() << "invoke iconv_open failed";
    }
    size_t dest_size = buf_size <= 0 ? str.size() * 5 : buf_size;
    string dest(dest_size, '\0'); // GBK: 2字节，UTF8: 1~3字节
    char* inbuf = (char*) str.c_str();
    size_t inbytesleft = str.size();
    char* outbuf = (char*) dest.c_str();
    size_t outbytesleft = dest.size();
#ifdef _WIN32
    size_t size = ::iconv(cd, (const char* *) &inbuf, (size_t *) & inbytesleft, (char* *) &outbuf, (size_t *) & outbytesleft);
#else
    size_t size = ::iconv(cd, (char* *) &inbuf, (size_t *) & inbytesleft, (char* *) &outbuf, (size_t *) & outbytesleft);
#endif
    if (size == (size_t) (-1)) {
        iconv_close(cd);
        //xthrow() << "invoke iconv failed: str=[" << str << "]";
    }
    if (iconv_close(cd) != 0) {
        //xthrow() << "invoke iconv_close failed";
    }
    string ret(dest.c_str());
    return ret;
}

string gbk_to_utf8(const string& str) {
    return iconv("GBK", "UTF-8", str, str.size()*2);
}

static bool GetAllCpuState(map<string, CPUStat>& cpuid_stat_map) {
    ifstream ifile("/proc/stat", ios::in);
    if (!ifile.is_open()) {
        ifile.close();
        return false;
    }
    string line_str;
    while (getline(ifile, line_str)) {
        if (line_str.find("cpu ") == line_str.npos && line_str.find("cpu") != line_str.npos) {
            stringstream ss(line_str);
            string str;
            vector<string> r;
            while (getline(ss, str, ' ')) {
                r.push_back(str);
            }
            CPUStat stat;
            memset(&stat, 0, sizeof(stat));
            int index = 1;
            stat.user = stol(r[index++]);
            stat.nice = stol(r[index++]);
            stat.system = stol(r[index++]);
            stat.idle = stol(r[index++]);
            stat.iowait = stol(r[index++]);
            stat.irq = stol(r[index++]);
            stat.softirq = stol(r[index++]);
            cpuid_stat_map[r[0]] = stat;
        }
    }
    ifile.close();
    return true;
}
#endif


int GetIdleCPUID() {
#ifdef _MSC_VER
    return 0;
#else
    map<string, CPUStat> first_cpuid_stat_map;
    map<string, CPUStat> second_cpuid_stat_map;
    if (!GetAllCpuState(first_cpuid_stat_map)) {
        return 0;
    }
    sleep(1);
    if (!GetAllCpuState(second_cpuid_stat_map)) {
        return 0;
    }

    map<int64_t, int64_t> usage_cpuid_map;
    int64_t cpu_total1 = 0;
    int64_t cpu_use1 = 0;
    int64_t cpu_total2 = 0;
    int64_t cpu_use2 = 0;
    int64_t usage = 0;
    for (auto it : first_cpuid_stat_map) {
        cpu_total1 = it.second.user + it.second.nice + it.second.system + it.second.idle + it.second.iowait +
                     it.second.irq + it.second.softirq;
        cpu_use1 = it.second.user + it.second.nice + it.second.system + it.second.irq + it.second.softirq;
        CPUStat& stat = second_cpuid_stat_map[it.first];
        cpu_total2 = stat.user + stat.nice + stat.system + stat.idle + stat.iowait + stat.irq + stat.softirq;
        cpu_use2 = stat.user + stat.nice + stat.system + stat.irq + stat.softirq;
        usage = (cpu_use2 - cpu_use1) * 100 / (cpu_total2 - cpu_total1);

        usage_cpuid_map[(int64_t)usage] = stoi(it.first.substr(3));
        printf("<%s> Usage<%ld>\n", it.first.c_str(), usage);
    }
    return usage_cpuid_map.begin()->second;
#endif
}

int64_t CommonUnit::start_time;

void CommonUnit::BindCPU() {
#ifdef _MSC_VER

#else
    int cpu_id = GetIdleCPUID();
    printf("==========BindCPU <%d>============\n", cpu_id);
    cpu_set_t mask;
    CPU_ZERO(&mask);
    CPU_SET(cpu_id, &mask);
    if (sched_setaffinity(0, sizeof (mask), &mask) == -1) {
        printf("Set Affinity Fail");
    }
    return;
#endif
}

int64_t CommonUnit::NowTimestamp() {
    return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
}

string CommonUnit::GetTradingDay() {
    auto tt = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    struct tm* ptm = localtime(&tt);
    char date[64] = "";
    sprintf(date, "%d%02d%02d", ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday);
    return std::string(date);
}

int64_t CommonUnit::GetTradeDay() {
    date today = day_clock::local_day();
    int year = today.year();
    int month = today.month();
    int day = today.day();
    return (year * 10000LL + month * 100LL + day);
}

int64_t CommonUnit::GetLocalTimeStamp() {
    auto tt = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    struct tm* ptm = localtime(&tt);
    int64_t day = (ptm->tm_year + 1900) * 10000LL + (ptm->tm_mon + 1) * 100LL + ptm->tm_mday;
    int64_t stamp = ptm->tm_hour * 10000LL + ptm->tm_min * 100LL + ptm->tm_sec;
    auto nowLocalTimeCount = std::chrono::system_clock::now().time_since_epoch();
    std::chrono::milliseconds now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(nowLocalTimeCount);
    return (day * 1000000000LL + stamp * 1000LL + now_ms.count() % 1000);
}

string CommonUnit::Market2Suffix(const int64_t& market) {
    string suffix;
    switch (market) {
    case kMarketSH:
        suffix = kSuffixSH;
        break;
    case kMarketSZ:
        suffix = kSuffixSZ;
        break;
    case kMarketCFFEX:
        suffix = kSuffixCFFEX;
        break;
    case kMarketSHFE:
        suffix = kSuffixSHFE;
        break;
    case kMarketDCE:
        suffix = kSuffixSHFE;
        break;
    case kMarketCZCE:
        suffix = kSuffixCZCE;
        break;
    case kMarketHK:
        suffix = kSuffixHK;
        break;
    case kMarketINE:
        suffix = kSuffixINE;
        break;
    case kMarketSGE:
        suffix = kSuffixSGE;
        break;
    case kMarketCSI:
        suffix = kSuffixCSI;
        break;
    default:
        break;
    }
    return suffix;
}

string CommonUnit::Trim(const string& context) {
    return context;
}

string CommonUnit::DropCodeSuffix(const string& code) {
    std::size_t pos = code.find(".");
    string instrument;
    if (pos != code.npos) {
        instrument = code.substr(0, pos);
    }
    return instrument;
}

string CommonUnit::SafeGBKToUTF8(const string& str) {
#ifdef _MSC_VER
    return str;
#else
    return gbk_to_utf8(str);
#endif
}

API_TYPE CommonUnit::GetApiType(const string& api) {
::    API_TYPE type = NONE;
    if (api == "ctp") {
        type = CTP;
    } else if (api == "tora") {
        type = TORA;
    } else if (api == "xele") {
        type = XELE;
    } else if (api == "simulate") {
        type = SIMULATE;
    }
    return type;
}

string CommonUnit::GetProduct(std::string contract) {
    std::string product = "";
    for (auto& it : contract) {
        if (it >= '0' && it <= '9') {
            break;
        }
        else {
            if (it >= 'a' && it <= 'z') {
                product += (it - 32);
            }
            else {
                product += it;
            }
        }
    }
    return product;
}

int64_t CommonUnit::Market2Std(string porduct) {
    int64_t market = 0;
    if (porduct == "SC") {
        market = kMarketINE;
    }
    else if (porduct == "IF" || porduct == "IH" || porduct == "IC" ||
        porduct == "T" || porduct == "TF" || porduct == "TL" || porduct == "TS") {
        market = kMarketCFFEX;
    }
    else {
        market = kMarketSHFE;
    }
    return market;
}

int64_t CommonUnit::CalculateNamoTimeSpread(const std::chrono::time_point<std::chrono::high_resolution_clock>& chrono_time) {
    std::chrono::time_point<std::chrono::high_resolution_clock> chrono_now = std::chrono::high_resolution_clock::now();
    auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(chrono_now - chrono_time);
    int64_t time_spread = ns.count();
    return time_spread;
}


