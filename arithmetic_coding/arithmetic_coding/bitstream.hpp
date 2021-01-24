#pragma once
#include <fstream>
#include <exception>


class bitstream {
public:
    bitstream() = default;
    virtual ~bitstream() = default;

    virtual bool operator!() const = 0;

protected:
    const static int Code_value_bits = 16;

    int buffer_ = 0;
    int bits_to_go_ = 0;
    int garbage_bits_ = 0;
};


class ibitstream : public bitstream {
public:
    ibitstream() = delete;
    explicit ibitstream(const std::string& filename) : bitstream(), stream_(filename, std::ios_base::binary) {}

    bool operator!() const override {
        return !stream_;
    }

    void operator>>(int& t) {
        if (bits_to_go_ == 0) {
            buffer_ = stream_.get();
            if (stream_.eof()) {
                garbage_bits_ += 1;
                if (garbage_bits_ > Code_value_bits - 2) {
                    throw std::ios_base::failure("input_bits_error\n");
                }
            }
            bits_to_go_ = 8;
        }
        t = buffer_ & 1;
        buffer_ >>= 1;
        bits_to_go_ -= 1;
    }

private:
    std::ifstream stream_;
};


class obitstream : public bitstream {
public:
    obitstream() = delete;
    explicit obitstream(const std::string& filename) : bitstream(), stream_(filename, std::ios_base::binary) {
        bits_to_go_ = 8;
    }

    bool operator!() const override {
        return !stream_;
    }

    void operator<<(int bit) {
        buffer_ >>= 1;
        if (bit) {
            buffer_ |= 0x80;
        }
        bits_to_go_ -= 1;
        if (bits_to_go_ == 0) {
            stream_.put(buffer_);
            bits_to_go_ = 8;
        }
    }

    void done_outputing_bits() {
        stream_.put(buffer_ >> bits_to_go_);
    }

private:
    std::ofstream stream_;
};