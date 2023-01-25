#include "BitWriter.h"

#include <iostream>

BitWriter::BitWriter(std::ostream &ostream, std::streamsize buffer_size) : ostream_(ostream) {
    buffer_size_ = buffer_size;
    buffer_index_ = 0;
    buffer_ = new uint8_t[buffer_size_ * 8];
}

BitWriter::~BitWriter() {
    Flush();
    delete[] buffer_;
}

void BitWriter::WriteBit(bool bit) {
    if (buffer_index_ == buffer_size_ * 8) {
        Flush();
    }

    auto &buf = buffer_[buffer_index_ >> 3];
    auto buf_ind = 7 - (buffer_index_ & 7);
    buf = (buf & ~(1 << buf_ind)) | (bit << buf_ind);
    ++buffer_index_;
}

void BitWriter::Flush() {
    if (buffer_index_ == 0) {
        return;
    }
    if ((buffer_index_ & 7) != 0) {
        Write(0, 8 - (buffer_index_ & 7));
    }
    ostream_.write(reinterpret_cast<char *>(buffer_), buffer_index_ >> 3);
    buffer_index_ = 0;
    if (!ostream_) {
        WritingError();
    }
}

void BitWriter::WritingError() {
    throw BitWriterError();
}

FileBitWriter::FileBitWriter(std::string filename, std::streamsize buffer_size)
    : BitWriter(ofstream_, buffer_size), filename_(filename), ofstream_(filename, std::ios::out | std::ios::binary) {
}

FileBitWriter::~FileBitWriter() {
    Close();
}

void FileBitWriter::Close() {
    Flush();
    ofstream_.close();
}

void FileBitWriter::WritingError() {
    throw FileBitWriterError(filename_);
}