#include <thread>
#include "strategy.h"

Strategy::Strategy() {
}

Strategy::~Strategy() {
}

void Strategy::SetStragety(string strategy_name, int64_t strategy_id) {
    strategy_name_ = strategy_name;
    strategy_id_ = strategy_id;
}

void Strategy::AddReadFile(const string& dir, const string file) {
    if (!reader) {
        reader = yijinjing::JournalReader::create(dir, file, yijinjing::TIME_FROM_FIRST, strategy_name_);
    } else {
        reader->addJournal(dir, file);
    }
}

void Strategy::AddWriteFile(const string& dir, const string file) {
    writer = yijinjing::JournalWriter::create(dir, file, strategy_name_);
}

void Strategy::SubInstrument(std::vector<std::string>& codes) {
        for (auto& it : codes) {
            all_instrument_.insert(it);
        }
}

void Strategy::StartWork() {
    thread_ = std::make_shared<std::thread>(std::bind(&Strategy::Run, this));
    thread_->detach();
    OnInit();
};

void Strategy::Run() {
    yijinjing::FramePtr frame;
    while (true) {
        frame = reader->getNextFrame();
        ProcessMessage(frame);
    }
}

void Strategy::ProcessMessage(yijinjing::FramePtr frame) {
    if (frame.get() != nullptr) {
        short msg_type = frame->getMsgType();
        void* data = frame->getData();
        int64_t msg_time = frame->getNano();
        int len = frame->getDataLength();
        FH_TYPE_LASTFG last_flag = frame->getLastFlag();
        switch(msg_type) {
            case FEEDER_TICK: {
                QTickT* tick = (QTickT*)data;
                if (all_instrument_.empty()) {
                    OnTick(tick);
                } else {
                    if (all_instrument_.find(tick->code) != all_instrument_.end()) {
                        OnTick(tick);
                    }
                }
                break;
            }
            case FEEDER_ORDER: {
                OnLevel2Order((QOrderT*)data);
                break;
            }
            case FEEDER_TRADE: {
                OnLevel2Knock((QKnockT*)data);
                break;
            }
            case TRADE_ORDER_REQ: {
                TradeOrder *order = (TradeOrder *) data;
                std::cout << " code: " << order->code
                          << ", price: " << order->price
                          << ", volume: " << order->volume
                          << ", bs_flag: " << order->bs_flag << std::endl;
                break;
            }
            break;
        }

    }
}




