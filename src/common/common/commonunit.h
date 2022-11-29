// Copyright 2020 Fancapital Inc.  All rights reserved.
#pragma once
#include "./datastruct.h"
#define EPSILON 0.000001

struct CPUStat {
    int64_t user;
    int64_t nice;
    int64_t system;
    int64_t idle;
    int64_t iowait;
    int64_t irq;
    int64_t softirq;
};

class CommonUnit {
 public:
     CommonUnit() {
     }

    ~CommonUnit() {
    }

    //static int GetIdleCPUID();
    
    static void BindCPU();

    static string SafeGBKToUTF8(const string& str);

    static int64_t NowTimestamp();

    static std::string GetTradingDay();

    static int64_t GetTradeDay();

    static int64_t GetLocalTimeStamp();

    static string Market2Suffix(const int64_t& market);

    static string Trim(const string& context);

    static string DropCodeSuffix(const string& code);

    static API_TYPE GetApiType(const string& api);

    static string GetProduct(std::string contract);

    static int64_t Market2Std(string porduct);

    static inline double i2f(const int64_t& value) {
        return value / 10000.0;
    }

    static inline int64_t f2i(const double& value) {
        return value >= 0 ? (int64_t)(value * 10000 + 0.5) : (int64_t)(value * 10000 - 0.5);
    }

    static int64_t CalculateNamoTimeSpread(const std::chrono::time_point<std::chrono::high_resolution_clock>& chrono_time);
    
    static bool DB_EQ(double a, double b) {
        return fabs(a - b) < EPSILON;
    }
    static bool DB_GREAT(double a, double b) {
        return (a - b) > EPSILON;
    }
    static bool DB_GREAT_EQ(double a, double b) {
        return (a - b) > EPSILON || DB_EQ(a, b);
    }
    static bool DB_LESS(double a, double b) {
        return (b - a) > EPSILON;
    }
    static bool DB_LESS_EQ(double a, double b) {
        return (b - a) > EPSILON || DB_EQ(a, b);
    }

    static int64_t start_time;
};


