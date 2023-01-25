#include "Compress.h"

void Compress(const std::vector<std::string> &arguments) {
    if (arguments.size() < 2) {
        throw InvalidNumberOfArguments();
    }
    auto archive_name = arguments[0];
    size_t files_count = arguments.size() - 1;

    archiver::Compressor compressor(archive_name);
    for (size_t file_index = 0; file_index < files_count; ++file_index) {
        const auto &filepath = arguments[file_index + 1];
        std::cout << "Compressing: " << filepath << std::endl;
        compressor.AddFile(filepath, file_index + 1 == files_count);
    }
}