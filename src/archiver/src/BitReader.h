#pragma once

#include <fstream>
#include <istream>

#include "Exceptions.h"

class BitReader {
public:
    static const std::streamsize BUFFER_SIZE = 16 * 1024;

    explicit BitReader(std::istream& istream, std::streamsize buffer_size = BUFFER_SIZE);
    ~BitReader();

    bool ReadBit(bool& result);
    template <typename T>
    uint8_t Read(T& result, uint8_t count);

private:
    std::istream& istream_;
    uint8_t* buffer_;
    std::streamsize buffer_size_;
    std::streamsize buffer_index_;
    bool is_eof_;
    std::streamsize eof_index_;

    virtual void ReadingError();
};

class FileBitReader : public BitReader {
public:
    explicit FileBitReader(const std::string& filename, std::streamsize buffer_size = BUFFER_SIZE);
    ~FileBitReader();

    void Close();

private:
    std::string filename_;
    std::ifstream ifstream_;

    void ReadingError() override;
};

/////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////// Definitions //////////////////////////////////////////

template <typename T>
uint8_t BitReader::Read(T& res, uint8_t count) {
    res = 0;
    for (uint8_t index = 0; index < count; ++index) {
        bool bit_result = false;
        if (ReadBit(bit_result)) {
            res |= static_cast<T>(bit_result) << (count - index - 1);
        } else {
            res >>= count - index;
            return index;
        }
    }
    return count;
}
