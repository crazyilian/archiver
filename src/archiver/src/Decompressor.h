#pragma once

#include <map>
#include <string>
#include <vector>

#include "ArchiverUtils.h"
#include "BinaryTrie.h"
#include "BitReader.h"
#include "BitWriter.h"
#include "Exceptions.h"

namespace archiver {
class Decompressor {
public:
    explicit Decompressor(const std::string &archive_path);

    bool DecompressFile();

    const std::string &GetLastFileName() const;

private:
    FileBitReader archive_reader_;
    std::string filename_;

    uint16_t NextSymbol(BinaryTrie<uint16_t> &trie);
};

}  // namespace src
