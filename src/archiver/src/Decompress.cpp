#include "Decompress.h"

void Decompress(const std::vector<std::string> &arguments) {
    if (arguments.size() != 1) {
        throw InvalidNumberOfArguments();
    }
    auto archive_name = arguments[0];

    archiver::Decompressor decompressor(archive_name);
    bool more_files = true;
    while (more_files) {
        more_files = decompressor.DecompressFile();
        std::cout << "Decompressed: " << decompressor.GetLastFileName() << std::endl;
    }
}
