
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



int main(int argc, char *argv[]){

    int interval = 10;
    JournalWriterPtr trade_writer_ = yijinjing::JournalWriter::create(KUNGFU_JOURNAL_FOLDER, "trade", "Client");
    JournalWriterPtr feeder_writer_ = yijinjing::JournalWriter::create(KUNGFU_JOURNAL_FOLDER, "feeder", "Client");

    int count = 50000;
    count = JOURNAL_PAGE_SIZE * 2 / (sizeof(QTickT) + sizeof(TradeOrder)) + 1000;
    QTickT tick;
    int tick_length = sizeof(tick);

    TradeOrder order;
    int order_length = sizeof(order);
    for(int j = 1; j <= count; ++j) {
        if (j % 2 == 0) {
            tick.receive_time = getNanoTime();
            memset(&tick, 0, tick_length);
            sprintf(tick.code, "%06d.SH", j);
            tick.new_price = j * 1.0;
            tick.new_volume = j * 100;
            tick.sum_amount = 100000.0;
            tick.sum_volume = 1000;
            for (int i = 0; i < 10; i++) {
                tick.bp[i] = j * 1.0 - 0.01 * i;
                tick.bv[i] = j * 100 - 1;
                tick.ap[i] = j * 1.0 + 0.01 * i;
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
        } else {
            order.receive_time = getNanoTime();
            memset(&order, 0, order_length);
            sprintf(order.code, "%06d.SH", j);
            order.volume = j * 100;
            order.price = j * 1.0;
            order.bs_flag = (j % 3 == 0) ? 1 : 2;
            order.order_ref = j;
            order.write_time = getNanoTime();
            trade_writer_->write_frame(&order, order_length, TRADE_ORDER_REQ, 0);
        }

        if (j % 10 == 0) {
            usleep(1);
        }
    }
}
