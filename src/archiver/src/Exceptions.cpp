#include "Exceptions.h"

MessageException::MessageException(const std::string& msg) : msg_(msg) {
}

const char* MessageException::what() const noexcept {
    return msg_.c_str();
}

InvalidNumberOfArguments::InvalidNumberOfArguments() : MessageException("Invalid number of arguments.") {
}

InvalidOption::InvalidOption(const std::string& option) : MessageException("Invalid option '" + option + "'.") {
}

BitReaderError::BitReaderError() : MessageException("Error when reading the stream.") {
}

FileBitReaderError::FileBitReaderError(const std::string& filename)
    : MessageException("Error when reading the file \"" + filename + "\".") {
}

BitWriterError::BitWriterError() : MessageException("Error when writing to the stream.") {
}

FileBitWriterError::FileBitWriterError(const std::string& filename)
    : MessageException("Error when writing to the file \"" + filename + "\".") {
}

InvalidArchive::InvalidArchive() : MessageException("Invalid archive.") {
}
