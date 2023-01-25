#include "catch.hpp"
#include <sstream>

#include "../src/BitWriter.h"

std::string StreamToBitString(const std::stringstream &stream) {
    std::string bit_string;
    std::string byte_string = stream.str();
    for (uint8_t byte : byte_string) {
        for (size_t i = 0; i < 8; ++i) {
            bool bit = (byte >> (7 - i)) & 1;
            bit_string.push_back(static_cast<char>('0' + bit));  // false -> '0', true -> '1'
        }
    }
    return bit_string;
}

void WriteBitString(BitWriter &bw, const std::string &target, size_t bits_length) {
    for (size_t start = 0; start < target.size(); start += bits_length) {
        uint8_t bits = 0;
        size_t end = std::min(start + bits_length, target.size());
        for (size_t i = start; i < end; ++i) {
            bits <<= 1;
            bits |= target[i] == '1';  // '0' -> false, '1' -> true
        }
        bw.Write(bits, end - start);
    }
}

TEST_CASE("[BitWriter]: Write 3 bits") {
    std::stringstream stream;
    BitWriter bw(stream, 1);
    std::string target = "001110101100111100010110";
    WriteBitString(bw, target, 3);
    bw.Flush();
    auto result = StreamToBitString(stream);
    REQUIRE(target == result);
}

TEST_CASE("[BitWriter]: Destructor flush") {
    std::stringstream stream;
    std::string target = "01000101";
    {
        BitWriter bw(stream, 1);
        WriteBitString(bw, target, 1);
    }
    auto result = StreamToBitString(stream);
    REQUIRE(target == result);
}

TEST_CASE("[BitWriter]: Align bytes with zeros") {
    std::stringstream stream;
    BitWriter bw(stream, 2);
    std::string target = "10011";
    WriteBitString(bw, target, 2);
    bw.Flush();
    auto result = StreamToBitString(stream);
    REQUIRE("10011000" == result);
}

TEST_CASE("[BitWriter]: FileBitWriter error") {
    try {
        FileBitWriter fbw("/", 1);
        WriteBitString(fbw, "10010011", 2);
        fbw.Flush();
        REQUIRE(false);  // no error
    } catch (FileBitWriterError &e) {
        REQUIRE(true);  // right error
    } catch (...) {
        REQUIRE(false);  // wrong error
    }
}

TEST_CASE("[BitWriter]: Empty flush") {
    std::stringstream stream;
    BitWriter bw(stream, 2);
    std::string target = "0011101011001111";
    WriteBitString(bw, target.substr(0, 8), 2);
    bw.Flush();
    bw.Flush();
    bw.Flush();
    WriteBitString(bw, target.substr(8), 2);
    bw.Flush();
    bw.Flush();
    bw.Flush();
    auto result = StreamToBitString(stream);
    REQUIRE(target == result);
}

TEST_CASE("[BitWriter]: FileBitWriter close empty file") {
    try {
        FileBitWriter fbw("/", 1);
        fbw.Close();
        REQUIRE(true);  // no errors
    } catch (...) {
        REQUIRE(false);  // some error
    }
}
