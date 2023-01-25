#include "catch.hpp"
#include <sstream>

#include "../src/BitReader.h"

TEST_CASE("[BitReader]: Read 3 bits") {
    // 00111010 01011110 00001100;
    std::vector<int> targets = {0b001, 0b110, 0b100, 0b101, 0b111, 0b000, 0b001, 0b100};
    std::string target_bytes{0b00111010, 0b01011110, 0b00001100};
    std::stringstream stream(target_bytes);
    BitReader br(stream, 1);
    for (size_t i = 0; i < targets.size(); ++i) {
        uint16_t bits = 0;
        REQUIRE(br.Read(bits, 3) == 3);
        REQUIRE(targets[i] == bits);
    }
}

TEST_CASE("[BitReader]: Read eof") {
    std::stringstream stream("a");
    BitReader br(stream, 2);
    uint16_t bits = 0;
    REQUIRE(br.Read(bits, 16) == 8);
    REQUIRE(bits == 'a');
    bits = 0;
    REQUIRE(br.Read(bits, 16) == 0);
}

TEST_CASE("[BitReader]: FileBitReader error") {
    try {
        FileBitReader fbr("/", 1);
        uint16_t bits = 0;
        fbr.Read(bits, 8);
        REQUIRE(false);  // no error
    } catch (FileBitReaderError &e) {
        REQUIRE(true);  // right error
    } catch (...) {
        REQUIRE(false);  // wrong error
    }
}
