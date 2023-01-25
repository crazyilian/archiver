#include "BitReader.h"

BitReader::BitReader(std::istream &istream, std::streamsize buffer_size) : istream_(istream) {
    buffer_size_ = buffer_size;
    buffer_index_ = buffer_size_ * 8;
    is_eof_ = false;
    buffer_ = new uint8_t[buffer_size_];
}

BitReader::~BitReader() {
    delete[] buffer_;
}

bool BitReader::ReadBit(bool &result) {
    if (!is_eof_ && buffer_index_ == buffer_size_ * 8) {
        buffer_index_ = 0;
        if (!istream_.read(reinterpret_cast<char *>(buffer_), buffer_size_)) {
            if (istream_.eof()) {
                is_eof_ = true;
                eof_index_ = istream_.gcount();
            } else {
                ReadingError();
            }
        }
    }
    if (is_eof_ && buffer_index_ == eof_index_ * 8) {
        return false;
    }
    result = buffer_[buffer_index_ >> 3] & (1 << (7 - (buffer_index_ & 7)));
    ++buffer_index_;
    return true;
}

void BitReader::ReadingError() {
    throw BitReaderError();
}

FileBitReader::FileBitReader(const std::string &filename, std::streamsize buffer_size)
    : BitReader(ifstream_, buffer_size), filename_(filename), ifstream_(filename, std::ios::in | std::ios::binary) {
}

FileBitReader::~FileBitReader() {
    Close();
}

void FileBitReader::Close() {
    ifstream_.close();
}

void FileBitReader::ReadingError() {
    throw FileBitReaderError(filename_);
}