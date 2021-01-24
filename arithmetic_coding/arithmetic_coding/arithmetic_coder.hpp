#pragma once
#include <iostream>
#include "bitstream.hpp"
#include "model.hpp"


class arithmetic_coder {
public:
    arithmetic_coder() = default;

    using code_value = long;

    void set_model(std::shared_ptr<model>& model) {
        model_ = model;
    }

protected:
    const static int Code_value_bits = 16;
    const static int Top_value = ((long)1 << Code_value_bits) - 1;

    const static int First_qtr = Top_value / 4 + 1;
    const static int Half      = 2 * First_qtr;
    const static int Third_qtr = 3 * First_qtr;

    code_value low_ = 0, high_ = Top_value;

    std::shared_ptr<model> model_ = std::make_shared<adaptive_model>();

    int char_to_index(int ch) const {
        return model_->char_to_index(ch);
    }

    int index_to_char(int symbol) const {
        return model_->index_to_char(symbol);
    }

    int cum_freq(int symbol) const {
        return model_->cum_freq(symbol);
    }

    int eof_symbol() const {
        return model_->eof_symbol();
    }

    void update_model(int symbol) {
        return model_->update_model(symbol);
    }
};


class arithmetic_encoder final : public arithmetic_coder {
public:
    arithmetic_encoder() = delete;
    explicit arithmetic_encoder(obitstream& b_out) : arithmetic_coder(), obitstream_(b_out) {}

    void encode(std::istream& istream) {
        while (true) {
            int ch = istream.get();
            if (istream.eof()) {
                break;
            }
            int symbol = char_to_index(ch);
            encode_symbol(symbol);
            update_model(symbol);
        }
        encode_symbol(eof_symbol());
        done_encoding();
        obitstream_.done_outputing_bits();
    }

private:
    long bits_to_follow_ = 0;

    obitstream& obitstream_;

    void encode_symbol(int symbol) {
        auto range = long(high_ - low_) + 1;
        high_ = low_ + (range * cum_freq(symbol - 1)) / cum_freq(0) - 1;
        low_ = low_ + (range * cum_freq(symbol)) / cum_freq(0);
        for (;;) {
            if (high_ < Half) {
                bit_plus_follow(0);
            }
            else if (low_ >= Half) {
                bit_plus_follow(1);
                low_ -= Half;
                high_ -= Half;
            }
            else if (low_ >= First_qtr && high_ < Third_qtr) {
                bits_to_follow_ += 1;
                low_ -= First_qtr;
                high_ -= First_qtr;
            }
            else break;

            low_ = 2 * low_;
            high_ = 2 * high_ + 1;
        }
    }

    void done_encoding() {
        bits_to_follow_ += 1;
        if (low_ < First_qtr) {
            bit_plus_follow(0);
        }
        else {
            bit_plus_follow(1);
        }
    }

    void bit_plus_follow(int bit) {
        obitstream_ << bit;
        while (bits_to_follow_ > 0) {
            obitstream_ << (!bit);
            bits_to_follow_ -= 1;
        }
    }
};


class arithmetic_decoder final : public arithmetic_coder {
public:
    arithmetic_decoder() = delete;
    explicit arithmetic_decoder(ibitstream& b_out) : arithmetic_coder(), ibitstream_(b_out) {
        for (auto i = 1; i <= Code_value_bits; i++) {
            int bit;
            ibitstream_ >> bit;
            value_ = 2 * value_ + bit;
        }
    }

    void decode(std::ostream& ostream) {
        while (true) {
            auto symbol = decode_symbol();
            if (symbol == eof_symbol()) {
                break;
            }
            auto ch = index_to_char(symbol);
            ostream.put(ch);
            update_model(symbol);
        }
    }

private:
    code_value value_ = 0;

    ibitstream& ibitstream_;

    int decode_symbol() {
        auto range = (long)(high_ - low_) + 1;
        int cum = (((long)(value_ - low_) + 1) * cum_freq(0) - 1) / range;
        int symbol;
        for (symbol = 1; cum_freq(symbol) > cum; symbol++);
        high_ = low_ + (range * cum_freq(symbol - 1)) / cum_freq(0) - 1;
        low_ = low_ + (range * cum_freq(symbol)) / cum_freq(0);
        while (true) {
            if (high_ < Half) {}
            else if (low_ >= Half) {
                value_ -= Half;
                low_ -= Half;
                high_ -= Half;
            }
            else if (low_ >= First_qtr && high_ < Third_qtr) {
                value_ -= First_qtr;
                low_ -= First_qtr;
                high_ -= First_qtr;
            }
            else break;
            low_ = 2 * low_;
            high_ = 2 * high_ + 1;

            int bit;
            ibitstream_ >> bit;
            value_ = 2 * value_ + bit;
        }
        return symbol;
    }
};