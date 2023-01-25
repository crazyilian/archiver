SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-instr-generate -fcoverage-mapping")

add_catch(
        unit_tests_archiver
        test_archiver_utils.cpp ../src/ArchiverUtils.cpp
        test_binary_trie.cpp
        test_bit_reader.cpp ../src/BitReader.cpp
        test_bit_writer.cpp ../src/BitWriter.cpp
        test_priority_queue.cpp
        ../src/Exceptions.cpp
)
