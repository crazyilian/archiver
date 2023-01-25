#include "PrintUtils.h"

void PrintMessageAndTryHelp(const std::string &message) {
    PrintMessage(message);
    std::cout << "Try 'src -h' for help." << std::endl;
}

void PrintMessage(const std::string &message) {
    if (!message.empty()) {
        std::cout << message << std::endl;
    }
}

void PrintHelpMessage() {
    std::cout << "Usage: src [options] [archive_name] [filenames]\n"
                 "    -h                                       help message\n"
                 "    -c archive_name file1 [file2 ...]        compress files to archive 'archive_name'\n"
                 "    -d archive_name                          decompress archive 'archive_name' to ./"
              << std::endl;
}
