#pragma once

#include <stdint.h>

#include <algorithm>
#include <cstddef>
#include <tuple>
#include <vector>

namespace archiver {

const uint16_t FILENAME_END = 256;
const uint16_t ONE_MORE_FILE = 257;
const uint16_t ARCHIVE_END = 258;

struct HuffmanCode {
    uint64_t code;
    uint8_t length;

    HuffmanCode();

    bool operator<(const HuffmanCode &other) const {
        return std::tie(length, code) < std::tie(other.length, other.code);
    }
};

std::vector<HuffmanCode> CalculateHuffmanCodesByLengths(const std::vector<uint16_t> &codes_length_cnt);

}  // namespace src
