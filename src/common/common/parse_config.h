#include "base_json.h"
//// 解析config.json文件的函数
class ParseConfig : public ParseJson {
public:
    ParseConfig(const char* file) : ParseJson(file) {

    }

    virtual ~ParseConfig() {

    }

    string log_directory() {
        string log_directory = root.get<string>("log_directory");
        return log_directory;
    }

    string log_file_name() {
        string log_file_name = root.get<string>("log_file_name");
        return log_file_name;
    }

    string sm_path() {
        string sm_path = root.get<string>("sm_path");
        return sm_path;
    }

    long sm_size() {
        long sm_size = root.get<long>("sm_size");
        return sm_size;
    }

    //void GetInstruments(std::vector<string>& vec_instruments) {
    //    string Temp_Instrument = root.get<string>("instrumnet");
    //    boost::split(vec_instruments, Temp_Instrument, boost::is_any_of("|"), boost::token_compress_on);
    //}

    void GetMdInfos(std::vector<ConfigInfo>& vec_mds) {
        items = root.get_child("market");
        for (boost::property_tree::ptree::iterator it = items.begin(); it != items.end(); ++it) {
            if (it->second.get<bool>("enabled")) {
                ConfigInfo Temp;
                memset(&Temp, 0, sizeof(Temp));
                Temp.apitype = CommonUnit::GetApiType(it->second.get<string>("apiname").c_str());
                strcpy(Temp.brokenid, it->second.get<string>("brokenid").c_str());
                strcpy(Temp.investorid, it->second.get<string>("investorid").c_str());
                strcpy(Temp.password, it->second.get<string>("password").c_str());
                strcpy(Temp.sourceaddr, it->second.get<string>("address").c_str());
                strcpy(Temp.reserve, it->second.get<string>("reserve").c_str());
                Temp.port = it->second.get<int>("port");
                Temp.sm_size = it->second.get<double>("sm_size");
                vec_mds.push_back(Temp);
            }
        }
    }

    void GetTradeInfos(std::vector<ConfigInfo>& vec_trades) {
        items = root.get_child("trade");
        for (boost::property_tree::ptree::iterator it = items.begin(); it != items.end(); ++it) {
            if (it->second.get<bool>("enabled")) {
                ConfigInfo Temp;
                memset(&Temp, 0, sizeof(Temp));
                Temp.apitype = CommonUnit::GetApiType(it->second.get<string>("apiname").c_str());
                strcpy(Temp.brokenid, it->second.get<string>("brokenid").c_str());
                strcpy(Temp.investorid, it->second.get<string>("investorid").c_str());
                strcpy(Temp.password, it->second.get<string>("password").c_str());
                strcpy(Temp.sourceaddr, it->second.get<string>("address").c_str());
                strcpy(Temp.reserve, it->second.get<string>("reserve").c_str());
                Temp.port = it->second.get<int>("port");
                Temp.sm_size = it->second.get<double>("sm_size");
                vec_trades.push_back(Temp);
            }
        }
    }

    void GetStrategys(std::vector<StrategyInfo>& strategys) {
        items = root.get_child("strategy");
        for (boost::property_tree::ptree::iterator it = items.begin(); it != items.end(); ++it) {
            if (it->second.get<bool>("enabled")) {
                StrategyInfo Temp;
                memset(&Temp, 0, sizeof(Temp));
                strcpy(Temp.strategy_name, it->second.get<string>("strategyname").c_str());
                strcpy(Temp.config_file, it->second.get<string>("configfile").c_str());
                Temp.strategy_id = it->second.get<int>("strategyid");
                Temp.sm_size = it->second.get<double>("sm_size");
                Temp.broker_type = CommonUnit::GetApiType(it->second.get<string>("broker").c_str());
                Temp.feeder_type = CommonUnit::GetApiType(it->second.get<string>("feeder").c_str());
                strategys.push_back(Temp);
            }
        }
    }
};

//ParseConfig::ParseConfig(const char* file) : ParseJson(file) {
//
//}
