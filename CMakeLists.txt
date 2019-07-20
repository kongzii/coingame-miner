cmake_minimum_required(VERSION 3.10)
project(TestDayMiner)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS  OFF)
set(CMAKE_CXX_FLAGS "-O3 -march=native")

include(FindPkgConfig)

pkg_check_modules(CURLPP REQUIRED curlpp)
find_package(OpenSSL REQUIRED)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

include_directories("../include/")
include_directories(${OPENSSL_INCLUDE_DIR})

add_executable(TestDayMiner
        src/main.cpp
        src/Config/Config.h
        src/Config/Config.cpp
        src/Miner/Generator.h
        src/Miner/Generator.cpp
        src/Miner/Block.h
        src/Miner/Block.cpp
        src/Miner/BlockChain.h
        src/Miner/BlockChain.cpp
        src/Miner/Transaction.h
        src/Miner/Transaction.cpp
        src/Miner/State.h
        src/Miner/GeneratorWorkers.cpp)

target_link_libraries(TestDayMiner pthread ${OPENSSL_LIBRARIES} ${CURLPP_LDFLAGS})
