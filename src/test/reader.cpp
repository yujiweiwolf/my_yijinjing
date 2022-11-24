
#include <iostream>
#include <fstream>
#include <string.h>
#include <math.h>
#include <sched.h>
#include <unistd.h>
#include <algorithm>
#include "../journal/JournalReader.h"
#include "../journal/FrameHeader.h"
#include "../journal/Frame.hpp"
#include "../journal/Timer.h"
#include "../utils/datastruct.h"

using yijinjing::JournalReaderPtr;
using namespace yijinjing;

#define KUNGFU_JOURNAL_FOLDER "./data/"  /**< where we put journal files */

bool cpu_set_affinity(int cpu_id){
#if defined __linux__
    cpu_set_t mask;
    CPU_ZERO(&mask);
    CPU_SET(cpu_id, &mask);
    return 0 == sched_setaffinity(0, sizeof(mask), &mask);
#else
    return false;
#endif
}

// step 10 100 1000
void StatisticsTimeSpread(vector<int64_t>& all_diff, int step) {
    for (int i = 1; i < 100; i++) {
        int total_num = 0;
        for (auto& it : all_diff) {
            if (it >= (i - 1) * step && it < i * step) {
                total_num++;
            }
        }
        if (total_num) {
            double ratio = total_num * 1.0 / all_diff.size() * 100.00;
            std::cout << "[statistics, size:" << all_diff.size() << ", range, from " << (i - 1) * step << " ns to " << i * step
                   << " ns, num: " << total_num << ", ratio: " << ratio << "%]" << std::endl;
        }
    }
}

int main(){
    int cpu_id_ = 16;
    cpu_set_affinity(cpu_id_);
    std::vector<string> empty;
    JournalReaderPtr reader = yijinjing::JournalReader::create(KUNGFU_JOURNAL_FOLDER, "feeder", yijinjing::TIME_FROM_FIRST, "Client_R");
    reader->addJournal(KUNGFU_JOURNAL_FOLDER, "trade");
    bool flag = reader->seekTimeJournalByName("trade", yijinjing::TIME_FROM_FIRST);
    yijinjing::FramePtr frame;
    int index = 0;
    while (true) {
        frame = reader->getNextFrame();
        if (frame.get() != nullptr)
        {
            short msg_type = frame->getMsgType();
            void* data = frame->getData();
            int64_t msg_time = frame->getNano();
            int len = frame->getDataLength();
            FH_TYPE_LASTFG last_flag = frame->getLastFlag();
            index++;
            std::cout << "index: " << index
                    << ", msg_type: " << msg_type << std::endl;
            if (msg_type == FEEDER_TICK) {
                QTickT *tick = (QTickT *) data;
                std::cout << " code: " << tick->code
                          << ", new_price: " << tick->new_price
                          << ", new_volume: " << tick->new_volume << std::endl;
            } else if (msg_type == TRADE_ORDER_REQ) {
                TradeOrder *order = (TradeOrder *) data;
                std::cout << " code: " << order->code
                          << ", price: " << order->price
                          << ", volume: " << order->volume
                          << ", bs_flag: " << order->bs_flag << std::endl;
            }
        }
    }


//    std::vector<yijinjing::FrameHeader*> out;
//    int index = 0;
//    vector<int64_t> all_diff;
//    vector<QTickT> all_tick;
//    while (true) {
//        out.clear();
//        reader->ReadData(out);
////        if (out.size() > 0) {
////            std::cout << "out size: " << out.size() << std::endl;
////        }
//        for (auto& it : out) {
//            yijinjing::Frame frame((void*)it);
//            short msg_type = frame.getMsgType();
//            int date_length = frame.getDataLength();
//            void* data = frame.getData();
//            index++;
//            std::cout << "index: " << index
//                    << ", msg_type: " << msg_type << std::endl;
//              // << ", msg_type: " << msg_type << ", data is " << std::string((char*)data, date_length) << std::endl
//            if (msg_type == 101) {
//                QTickT* tick = (QTickT*)data;
//                std::cout << " code: " << tick->code
//                          << ", new_price: " << tick->new_price
//                          << ", new_volume: " << tick->new_volume << std::endl;
//;
////                int64_t recei_time = getNanoTime();
////                int64_t diff = recei_time- tick->write_time;
////                all_diff.push_back(diff);
////                all_tick.push_back(*tick);
////
////                if (all_diff.size() == 10000) {
////                    std::sort(all_diff.begin(), all_diff.end(), [](int64_t a, int64_t b) {return a > b; });
////                    int num = 0;
////                    for (auto& it : all_diff) {
////                        std::cout << "after sort diff: " << it  << ", num: " << ++num << std::endl;
////                    }
////                    std::cout << "over" << ", index: " << index << std::endl;
////                    StatisticsTimeSpread(all_diff, 1000);
////
//////                    for (auto &it : all_tick) {
//////                        std::cout << " code: " << it.code
//////                                  << ", new_price: " << it.new_price
//////                                  << ", new_volume: " << it.new_volume
//////                                  << ", local_time: " << it.local_time
//////                                  << ", recei_time: " << it.recei_time
//////                                  << ", diff: " << (it.recei_time - it.local_time) << std::endl;
//////                    }
//////                    std::cout << " ============================= index:" << index << std::endl;
////                    all_diff.clear();
////                    all_tick.clear();
////                }
//            } else if (msg_type == 777) {
//                TradeOrder *order = (TradeOrder *) data;
//                std::cout << " code: " << order->code
//                          << ", price: " << order->price
//                          << ", volume: " << order->volume
//                          << ", bs_flag: " << order->bs_flag << std::endl;
//            }
//        }
//    }
}
