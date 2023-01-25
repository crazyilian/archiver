#include "ArchiverUtils.h"

namespace archiver {

HuffmanCode::HuffmanCode() : code(0), length(0) {
}

std::vector<HuffmanCode> CalculateHuffmanCodesByLengths(const std::vector<uint16_t> &codes_length_cnt) {
    uint16_t max_code_length = codes_length_cnt.size();
    HuffmanCode code;
    std::vector<HuffmanCode> codes;
    for (uint16_t length = 1; length <= max_code_length; ++length) {
        while (code.length != length) {
            code.code <<= 1;
            ++code.length;
        }
        for (uint16_t index = 0; index < codes_length_cnt[length - 1]; ++index) {
            codes.push_back(code);
            ++code.code;
        }
    }
    return codes;
}

}  // namespace archiver
