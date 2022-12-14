
#include <iostream>
#include <fstream>
#include <string.h>
#include <math.h>
#include <sched.h>
#include <unistd.h>
#include <algorithm>

#include "test_strategy.h"

int main(){
    yijinjing::TestStrategy test;
    test.SetStragety("test_stratey", 99);

//    std::vector<std::string> sub_codes;
//    sub_codes.push_back("IC2203.CFFEX");
//    sub_codes.push_back("IF2203.CFFEX");
//    sub_codes.push_back("IH2203.CFFEX");
//    test.SubInstrument(sub_codes);

    //
    test.AddReadFile("./data/", "trade");
    test.AddReadFile("./data/", "feeder");
    // reader->seekTimeJournalByName("trade", yijinjing::TIME_FROM_FIRST);

    test.AddWriteFile("./data/", "test_stratey");
    test.StartWork();
    getchar();
}
