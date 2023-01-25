#pragma once

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

#include "ArchiverUtils.h"
#include "BinaryHeap.h"
#include "BinaryTrie.h"
#include "BitReader.h"
#include "BitWriter.h"

namespace archiver {
class Compressor {
public:
    explicit Compressor(const std::string &archive_name);

    void WriteCode(const HuffmanCode &code);
    void AddFile(const std::string &filepath, bool is_last_file);

private:
    FileBitWriter archive_writer_;

    void FindTrieHeightsDFS(BinaryTrie<uint16_t>::Node *node, std::vector<std::pair<uint8_t, uint16_t>> &heights,
                            uint8_t height) const;

    class HeapValue {
    public:
        size_t cnt;
        uint16_t symbol;
        BinaryTrie<uint16_t>::Node *node;

        HeapValue(size_t cnt, uint16_t symbol, BinaryTrie<uint16_t>::Node *node);
        HeapValue(const HeapValue &val1, const HeapValue &val2);

        bool operator<(const HeapValue &other) const;
    };
};

}  // namespace src
