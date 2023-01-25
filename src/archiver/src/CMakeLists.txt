set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -static")

add_executable(
        archiver
        archiver.cpp
        ArchiverUtils.cpp
        BitReader.cpp
        BitWriter.cpp
        CommandLineArguments.cpp
        Compress.cpp
        Compressor.cpp
        Decompress.cpp
        Decompressor.cpp
        Exceptions.cpp
        PrintUtils.cpp
)

