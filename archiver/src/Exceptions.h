#pragma once

#include <exception>
#include <string>

class MessageException : public std::exception {
public:
    explicit MessageException(const std::string& msg);

    const char* what() const noexcept override;

protected:
    std::string msg_;
};

class InvalidNumberOfArguments : public MessageException {
public:
    InvalidNumberOfArguments();
};

class InvalidOption : public MessageException {
public:
    explicit InvalidOption(const std::string& option);
};

class BitReaderError : public MessageException {
public:
    BitReaderError();
};

class FileBitReaderError : public MessageException {
public:
    explicit FileBitReaderError(const std::string& filename);
};

class BitWriterError : public MessageException {
public:
    BitWriterError();
};

class FileBitWriterError : public MessageException {
public:
    explicit FileBitWriterError(const std::string& filename);
};

class InvalidArchive : public MessageException {
public:
    InvalidArchive();
};