#include "Decompressor.h"

namespace archiver {

Decompressor::Decompressor(const std::string& archive_path) : archive_reader_(archive_path) {
}

uint16_t Decompressor::NextSymbol(BinaryTrie<uint16_t>& trie) {
    auto node = trie.Root();
    do {
        bool bit = false;
        if (!archive_reader_.ReadBit(bit)) {
            throw InvalidArchive();
        }
        node = trie.Go(node, bit);
    } while (node->left);
    return node->value;
}

bool Decompressor::DecompressFile() {
    // Reading alphabet length
    uint16_t alphabet_length = 0;
    if (archive_reader_.Read(alphabet_length, 9) != 9) {
        throw InvalidArchive();
    }

    // Reading alphabet symbols
    std::vector<uint16_t> symbols(alphabet_length);
    for (uint16_t index = 0; index < alphabet_length; ++index) {
        if (archive_reader_.Read(symbols[index], 9) != 9) {
            throw InvalidArchive();
        }
    }

    // Reading number of codes of each length
    std::vector<uint16_t> codes_length_cnt;
    {
        uint16_t sum_codes_cnt = 0;
        while (sum_codes_cnt < alphabet_length) {
            uint16_t length_cnt = 0;
            if (archive_reader_.Read(length_cnt, 9) != 9) {
                throw InvalidArchive();
            }
            codes_length_cnt.push_back(length_cnt);
            sum_codes_cnt += length_cnt;
        }
        if (sum_codes_cnt != alphabet_length) {
            throw InvalidArchive();
        }
    }

    // Calculating canonical Huffman codes
    auto codes_sequence = CalculateHuffmanCodesByLengths(codes_length_cnt);

    // Creating binary trie : code -> symbol
    BinaryTrie<uint16_t> trie;
    for (uint16_t symbol_index = 0; symbol_index < alphabet_length; ++symbol_index) {
        const auto& [code, length] = codes_sequence[symbol_index];
        trie.AddPath(code, length)->value = symbols[symbol_index];
    }

    // Reading filename
    filename_ = "";
    while (true) {
        uint16_t symbol = NextSymbol(trie);
        if (symbol == FILENAME_END) {
            break;
        } else if (symbol >= 256) {
            throw InvalidArchive();
        } else {
            filename_.push_back(static_cast<char>(symbol));
        }
    }

    // Reading file content
    FileBitWriter file_writer(filename_);
    while (true) {
        uint16_t symbol = NextSymbol(trie);
        if (symbol == ONE_MORE_FILE) {
            return true;
        } else if (symbol == ARCHIVE_END) {
            return false;
        } else {
            file_writer.Write(symbol, 8);
        }
    }
}

const std::string& Decompressor::GetLastFileName() const {
    return filename_;
}

}  // namespace src
