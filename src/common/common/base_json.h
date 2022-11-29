#pragma once
#include <iostream>
#include <string>
#include <boost/algorithm/string.hpp>
#include "datastruct.h"

class ParseJson {
public:
    ParseJson(const char* file) {
        config_file_ = file;
        boost::property_tree::read_json<boost::property_tree::ptree>(config_file_.c_str(), root);
    }

    virtual ~ParseJson() {
    }

 public:
    string config_file_;
    boost::property_tree::ptree root;
    boost::property_tree::ptree items;
};
