
#include <iostream>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include "../journal/JournalWriter.h"
#include "../journal/FrameHeader.h"
#include "../journal/Timer.h"
#include "../journal/PageProvider.h"
#include "../journal/PageUtil.h"
#include "../utils/datastruct.h"

using yijinjing::JournalWriterPtr;
using namespace yijinjing;
const string kVersion = "v1.0.1";

#define KUNGFU_JOURNAL_FOLDER "./data/"  /**< where we put journal files */
#define COUNT 10000

void Test_Write_Memcpy(int interval) {
    JournalWriterPtr feeder_writer_ = yijinjing::JournalWriter::create(KUNGFU_JOURNAL_FOLDER, "feeder", "Client");
    int count = COUNT;
    QTickT tick;
    int tick_length = sizeof(tick);
    for(int j = 1; j <= count; ++j) {
            memset(&tick, 0, sizeof(tick));
            tick.receive_time = getNanoTime();
            memset(&tick, 0, tick_length);
            sprintf(tick.code, "%06d.SH", j);
            tick.new_price = j * 1.0;
            tick.new_volume = j * 100;
            tick.sum_amount = 100000.0;
            tick.sum_volume = 1000;
            for (int i = 0; i < 10; i++) {
                tick.bp[i] = j * 1.0 - 0.01 * (i + 1);
                tick.bv[i] = j * 100 - 1;
                tick.ap[i] = j * 1.0 + 0.01 * (i + 1);
                tick.av[i] = j * 100 + 1;
            }
            tick.timestamp = 20221116094539650 + j * 1000;
            tick.market = 1;
            tick.pre_close = j * 1.0;
            tick.upper_limit = 10.0;
            tick.lower_limit = 1.2;
            tick.open = j * 1.0 + 0.1;
            tick.high = j * 1.0 + 0.2;
            tick.low = j * 1.0 + 0.3;
            tick.open = j * 1.0 + 0.4;
            tick.close = j * 1.0 + 0.5;
            tick.avg_bid_price = j * 1.0 + 0.5;
            tick.avg_ask_price = j * 1.0 + 0.5;
            tick.write_time = getNanoTime();
            feeder_writer_->write_frame(&tick, tick_length, FEEDER_TICK, 0);
//            std::cout << "write index: " << j
//                      << " code: " << tick.code
//                      << ", new_price: " << tick.new_price
//                      << ", new_volume: " << tick.new_volume
//                      << ", sum_amount: " << tick.sum_amount
//                      << ", sum_volume: " << tick.sum_volume
//                      << ", bp: " << tick.bp[0]
//                      << ", bv: " << tick.bv[0]
//                      << ", ap: " << tick.ap[0]
//                      << ", av: " << tick.av[0]
//                      << ", write_time: " << tick.write_time
//                      << std::endl;
        if (interval > 0 && j % interval == 0) {
            usleep(1);
        }
    }
}

void Test_Write_Allocate(int interval) {
    JournalWriterPtr feeder_writer_ = yijinjing::JournalWriter::create(KUNGFU_JOURNAL_FOLDER, "feeder", "Client");
    int count = COUNT;

    for(int j = 1; j <= count; ++j) {
        void *buffer = feeder_writer_->GetFrame();
        int64_t nano = getNanoTime();
        Frame frame(buffer);
        frame.setNano(nano);

        QTickT *tick = (QTickT *) ((char *) buffer + BASIC_FRAME_HEADER_LENGTH);
        tick->receive_time = getNanoTime();
        sprintf(tick->code, "%06d.SH", j);
        tick->new_price = j * 1.0;
        tick->new_volume = j * 100;
        tick->sum_amount = 100000.0;
        tick->sum_volume = 1000;
        for (int i = 0; i < 10; i++) {
            tick->bp[i] = j * 1.0 - 0.01 * (i + 1);
            tick->bv[i] = j * 100 - 1;
            tick->ap[i] = j * 1.0 + 0.01 * (i + 1);
            tick->av[i] = j * 100 + 1;
        }
        tick->timestamp = 20221116094539650 + j * 1000;
        tick->market = 1;
        tick->pre_close = j * 1.0;
        tick->upper_limit = 10.0;
        tick->lower_limit = 1.2;
        tick->open = j * 1.0 + 0.1;
        tick->high = j * 1.0 + 0.2;
        tick->low = j * 1.0 + 0.3;
        tick->open = j * 1.0 + 0.4;
        tick->close = j * 1.0 + 0.5;
        tick->avg_bid_price = j * 1.0 + 0.5;
        tick->avg_ask_price = j * 1.0 + 0.5;
        tick->write_time = getNanoTime();

        feeder_writer_->passFrame(frame, sizeof(QTickT), FEEDER_TICK, 0);
//        std::cout << "write index: " << j
//                  << " code: " << tick->code
//                  << ", new_price: " << tick->new_price
//                  << ", new_volume: " << tick->new_volume
//                  << ", sum_amount: " << tick->sum_amount
//                  << ", sum_volume: " << tick->sum_volume
//                  << ", bp: " << tick->bp[0]
//                  << ", bv: " << tick->bv[0]
//                  << ", ap: " << tick->ap[0]
//                  << ", av: " << tick->av[0]
//                  << ", write_time: " << tick->write_time
//                  << std::endl;
        if (interval > 0 && j % interval == 0) {
            usleep(1);
        }
    }
}


int main(int argc, char *argv[]){
    int model = atoi(argv[1]);
    int interval = atoi(argv[2]);
    if (model == 1) {
        Test_Write_Memcpy(interval);
    } else {
        Test_Write_Allocate(interval);
    }
    return 0;
}
