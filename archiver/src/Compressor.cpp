#include "Compressor.h"

namespace archiver {

Compressor::HeapValue::HeapValue(size_t cnt, uint16_t symbol, BinaryTrie<uint16_t>::Node *node)
    : cnt(cnt), symbol(symbol), node(node) {
}

Compressor::HeapValue::HeapValue(const HeapValue &val1, const HeapValue &val2) {
    cnt = val1.cnt + val2.cnt;
    symbol = std::min(val1.symbol, val2.symbol);
    node = new BinaryTrie<uint16_t>::Node(symbol);
    node->left = val1.node;
    node->right = val2.node;
}

bool Compressor::HeapValue::operator<(const HeapValue &other) const {
    return std::tie(cnt, symbol) < std::tie(other.cnt, other.symbol);
}

Compressor::Compressor(const std::string &archive_name) : archive_writer_(archive_name) {
}

void Compressor::WriteCode(const HuffmanCode &code) {
    archive_writer_.Write(code.code, code.length);
}

void Compressor::FindTrieHeightsDFS(BinaryTrie<uint16_t>::Node *node,
                                    std::vector<std::pair<uint8_t, uint16_t>> &heights, uint8_t height) const {
    if (!node->left) {
        // Terminal node
        heights.emplace_back(height, node->value);
        return;
    }
    FindTrieHeightsDFS(node->left, heights, height + 1);
    FindTrieHeightsDFS(node->right, heights, height + 1);
}

void Compressor::AddFile(const std::string &filepath, bool is_last_file) {
    std::string filename = std::filesystem::path(filepath).filename();

    uint16_t max_symbols_count = 1 << 9;
    std::vector<size_t> symbol_count(max_symbols_count, 0);
    uint16_t alphabet_length = 0;

    // Count system symbols
    symbol_count[archiver::FILENAME_END] = 1;
    symbol_count[archiver::ONE_MORE_FILE] = 1;
    symbol_count[archiver::ARCHIVE_END] = 1;

    // Count symbols in filename
    for (const uint8_t byte : filename) {
        ++symbol_count[byte];
    }

    // Count symbols in file
    {
        FileBitReader bit_reader(filepath);
        uint8_t byte = 0;
        while (bit_reader.Read(byte, 8)) {
            ++symbol_count[byte];
        }
    }

    // Initialize priority queue and terminal nodes of trie
    BinaryHeap<HeapValue> heap;
    for (uint16_t symbol = 0; symbol < max_symbols_count; ++symbol) {
        size_t cnt = symbol_count[symbol];
        if (cnt > 0) {
            ++alphabet_length;
            HeapValue value{cnt, symbol, new BinaryTrie<uint16_t>::Node(symbol)};
            heap.Insert(value);
        }
    }

    // Build binary trie
    while (heap.size() > 1) {
        auto left = heap.PopMin();
        auto right = heap.PopMin();
        HeapValue merged(left, right);
        heap.Insert(merged);
    }
    auto trie_root = heap.PopMin().node;

    // Find huffman code lengths
    std::vector<std::pair<uint8_t, uint16_t>> codes_length;
    codes_length.reserve(alphabet_length);
    FindTrieHeightsDFS(trie_root, codes_length, 0);
    delete trie_root;

    // Calculate canonical huffman codes
    std::sort(codes_length.begin(), codes_length.end());  // Sorting by length, then by symbol
    std::vector<uint16_t> codes_length_cnt;
    for (const auto &[length, symbol] : codes_length) {  // Calculating count of codes of each length
        while (length > codes_length_cnt.size()) {
            codes_length_cnt.push_back(0);
        }
        ++codes_length_cnt.back();
    }
    auto codes_sequence = CalculateHuffmanCodesByLengths(codes_length_cnt);  // Calculating canonical Huffman codes
    std::vector<HuffmanCode> codes(max_symbols_count);
    for (uint16_t index = 0; index < alphabet_length; ++index) {  // Saving to vector: symbol -> code
        codes[codes_length[index].second] = codes_sequence[index];
    }

    // Write system info
    archive_writer_.Write(alphabet_length, 9);
    for (const auto &[length, symbol] : codes_length) {
        archive_writer_.Write(symbol, 9);
    }
    for (const auto length_cnt : codes_length_cnt) {
        archive_writer_.Write(length_cnt, 9);
    }

    // Write filename
    for (const uint8_t byte : filename) {
        WriteCode(codes[byte]);
    }
    WriteCode(codes[FILENAME_END]);

    // Write file content
    {
        FileBitReader bit_reader(filepath);
        uint8_t byte = 0;
        while (bit_reader.Read(byte, 8)) {
            WriteCode(codes[byte]);
        }
    }

    // Write end of file
    if (is_last_file) {
        WriteCode(codes[ARCHIVE_END]);
        archive_writer_.Close();
    } else {
        WriteCode(codes[ONE_MORE_FILE]);
    }
}

}  // namespace archiver
