#pragma once

#include <fstream>
#include <ostream>

#include "Exceptions.h"

class BitWriter {
public:
    static const std::streamsize BUFFER_SIZE = 16 * 1024;

    explicit BitWriter(std::ostream &ostream, std::streamsize buffer_size = BUFFER_SIZE);
    ~BitWriter();

    void WriteBit(bool bit);
    template <typename T>
    void Write(T bits, uint8_t count);

    void Flush();

private:
    std::ostream &ostream_;
    uint8_t *buffer_;
    std::streamsize buffer_size_;
    std::streamsize buffer_index_;

    virtual void WritingError();
};

class FileBitWriter : public BitWriter {
public:
    explicit FileBitWriter(std::string filename, std::streamsize buffer_size = BUFFER_SIZE);
    ~FileBitWriter();

    void Close();

private:
    std::string filename_;
    std::ofstream ofstream_;

    void WritingError() override;
};

/////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////// Definitions //////////////////////////////////////////

template <typename T>
void BitWriter::Write(T bits, uint8_t count) {
    for (uint8_t index = 0; index < count; ++index) {
        bool bit = (bits >> (count - index - 1)) & 1;
        WriteBit(bit);
    }
}
