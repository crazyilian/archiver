#include "catch.hpp"

#include "../src/ArchiverUtils.h"

void CheckCodes(std::vector<archiver::HuffmanCode> actual, std::vector<uint16_t> lengths_cnt) {
    std::vector<archiver::HuffmanCode> result = archiver::CalculateHuffmanCodesByLengths(lengths_cnt);
    REQUIRE(actual.size() == result.size());
    for (size_t i = 0; i < actual.size(); ++i) {
        REQUIRE(actual[i].length == result[i].length);
        REQUIRE(actual[i].code == result[i].code);
    }
}

std::vector<archiver::HuffmanCode> CreateCodes(std::vector<std::pair<int, int>> pairs) {
    std::vector<archiver::HuffmanCode> codes;
    for (const auto &[length, code] : pairs) {
        codes.emplace_back();
        codes.back().length = length;
        codes.back().code = code;
    }
    return codes;
}

TEST_CASE("[ArchiverUtils]: Zero lengths") {
    std::vector<uint16_t> lengths_cnt0{};
    std::vector<uint16_t> lengths_cnt2{0, 0};
    auto result = CreateCodes({});
    CheckCodes(result, lengths_cnt0);
    CheckCodes(result, lengths_cnt2);
}

TEST_CASE("[ArchiverUtils]: Different lengths") {
    std::vector<uint16_t> lengths_cnt{1, 1, 2};
    auto result = CreateCodes({{1, 0b0}, {2, 0b10}, {3, 0b110}, {3, 0b111}});
    CheckCodes(result, lengths_cnt);
}

TEST_CASE("[ArchiverUtils]: Equal lengths") {
    std::vector<uint16_t> lengths_cnt{0, 4};
    auto result = CreateCodes({{2, 0b00}, {2, 0b01}, {2, 0b10}, {2, 0b11}});
    CheckCodes(result, lengths_cnt);
}

TEST_CASE("[ArchiverUtils]: Unique lengths") {
    std::vector<uint16_t> lengths_cnt{1, 1, 1, 1};
    auto result = CreateCodes({{1, 0b0}, {2, 0b10}, {3, 0b110}, {4, 0b1110}});
    CheckCodes(result, lengths_cnt);
}

TEST_CASE("[ArchiverUtils]: Huffman Code Struct") {
    archiver::HuffmanCode code;
    REQUIRE(code.code == 0);
    REQUIRE(code.length == 0);
}

TEST_CASE("[ArchiverUtils]: Huffman Code Compare") {
    archiver::HuffmanCode code1;
    archiver::HuffmanCode code2;
    {
        code1.code = 0b10;
        code1.length = 2;
        code2.code = 0b010;
        code2.length = 3;
        REQUIRE(code1 < code2);
        REQUIRE(!(code2 < code1));
    }
    {
        code1.code = 0b01;
        code1.length = 2;
        code2.code = 0b10;
        code2.length = 2;
        REQUIRE(code1 < code2);
        REQUIRE(!(code2 < code1));
    }
    {
        code1.code = 0b01;
        code1.length = 2;
        code2.code = 0b01;
        code2.length = 2;
        REQUIRE(!(code1 < code2));
        REQUIRE(!(code2 < code1));
    }
}
