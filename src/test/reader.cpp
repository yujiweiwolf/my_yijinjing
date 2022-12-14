
#include <iostream>
#include <fstream>
#include <string.h>
#include <math.h>
#include <algorithm>
#include "../journal/Timer.h"
#include "../journal/JournalReader.h"
#include "../common/datastruct.h"

using yijinjing::JournalReaderPtr;
using namespace yijinjing;

#define KUNGFU_JOURNAL_FOLDER "../data"  /**< where we put journal files */
#define COUNT 10000

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
    // bool flag = reader->seekTimeJournalByName("trade", yijinjing::TIME_FROM_FIRST);
    yijinjing::FramePtr frame;
    int index = 0;
    while (true) {
        frame = reader->getNextFrame();
        if (frame.get() != nullptr)
        {
            short msg_type = frame->getMsgType();
            void* data = frame->getData();
//            int64_t msg_time = frame->getNano();
            int len = frame->getDataLength();
//            FH_TYPE_LASTFG last_flag = frame->getLastFlag();
            index++;
            std::cout << "index: " << index
                    << ", msg_type: " << msg_type
                    << ", len: " << len
                    << std::endl;
            if (msg_type == FEEDER_TICK) {
                QTickT *tick = (QTickT *) data;
                std::cout << " code: " << tick->code
                          << ", new_price: " << tick->new_price
                          << ", new_volume: " << tick->new_volume
                          << ", sum_amount: " << tick->sum_amount
                          << ", sum_volume: " << tick->sum_volume
                          << ", bp: " << tick->bp[0]
                          << ", bv: " << tick->bv[0]
                          << ", ap: " << tick->ap[0]
                          << ", av: " << tick->av[0]
                          << std::endl;
            } else if (msg_type == TRADE_ORDER_REQ) {
                TradeOrder *order = (TradeOrder *) data;
                std::cout << " code: " << order->code
                          << ", price: " << order->price
                          << ", volume: " << order->volume
                          << ", bs_flag: " << order->bs_flag << std::endl;
            } else if (msg_type == ONLY_LOG) {
                std::cout << std::string((char *) data, len) << std::endl;
            }
        }
    }
    return 0;


// 压力测试的统计
    {
        yijinjing::FramePtr frame;
        int index = 0;
        vector<int64_t> all_diff;
        vector<QTickT> all_tick;
        while (true) {
            frame = reader->getNextFrame();
            if (frame.get() != nullptr)
            {
                short msg_type = frame->getMsgType();
                void* data = frame->getData();
                index++;
    //            printf("index: %d, msg_type: %d, head address: %p\n", index, msg_type, frame->get_address());
                if (msg_type == FEEDER_TICK) {
                    QTickT* tick = (QTickT*)data;
                    int64_t recei_time = getNanoTime();
                    int64_t diff = recei_time- tick->write_time;
                    all_diff.push_back(diff);

    //                all_tick.push_back(*tick);
    //
    //                std::cout << " code: " << tick->code
    //                          << ", new_price: " << tick->new_price
    //                          << ", new_volume: " << tick->new_volume
    //                          << ", sum_amount: " << tick->sum_amount
    //                          << ", sum_volume: " << tick->sum_volume
    //                          << ", bp: " << tick->bp[0]
    //                          << ", bv: " << tick->bv[0]
    //                          << ", ap: " << tick->ap[0]
    //                          << ", av: " << tick->av[0]
    //                          << ", frame header nano: " << frame->getNano()
    //                          << ", write_time: " << tick->write_time
    //                          << ", recei_time: " << recei_time
    //                          << ", diff: " << diff
    //                          << std::endl;

                    if (all_diff.size() == COUNT) {
                        std::sort(all_diff.begin(), all_diff.end(), [](int64_t a, int64_t b) { return a > b; });
                        int num = 0;
                        for (auto &it: all_diff) {
                            std::cout << "after sort diff: " << it << ", num: " << ++num << std::endl;
                        }
                        std::cout << "over" << ", index: " << index << std::endl;
                        StatisticsTimeSpread(all_diff, 1000);

    //                    for (auto &it : all_tick) {
    //                        std::cout << " code: " << it.code
    //                                  << ", new_price: " << it.new_price
    //                                  << ", new_volume: " << it.new_volume
    //                                << ", sum_amount: " << it.sum_amount
    //                                << ", sum_volume: " << it.sum_volume
    //                                << ", bp: " << it.bp[0]
    //                                << ", bv: " << it.bv[0]
    //                                << ", ap: " << it.ap[0]
    //                                << ", av: " << it.av[0]
    //                                << ", write_time: " << it.write_time
    //                                << std::endl;
    //                    }
                        all_tick.clear();

                        std::cout << " ============================= index:" << index << std::endl;
                        all_diff.clear();
                    }
                }
            }
        }
    }
}
