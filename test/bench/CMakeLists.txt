cmake_minimum_required(VERSION 3.16)

set(CMAKE_C_COMPILER "clang-18")
set(CMAKE_CXX_COMPILER "clang++-18")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++ -lc++abi")
#set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(diploma_bench)

get_filename_component(PARENT_DIR ${PROJECT_SOURCE_DIR} DIRECTORY)
get_filename_component(PARENT_PARENT_DIR ${PARENT_DIR} DIRECTORY)

add_compile_options(-flto=thin -mllvm -polly -Ofast -std=c++2b -march=native -mtune=native -ffast-math -funroll-loops -fno-rtti)
link_libraries(-fuse-ld=lld)

#bench_output target
add_executable(bench_output
        output/main.cpp
        ../../lib/MemoryPool/MemoryPool.cpp
)
target_compile_options(bench_output PRIVATE -Wall -Wextra -std=c++23)
target_include_directories(bench_output PRIVATE ${PARENT_PARENT_DIR})

#bench_input target
add_executable(bench_input
        input/main.cpp
        ../../lib/MemoryPool/MemoryPool.cpp
)
target_compile_options(bench_input PRIVATE -Wall -Wextra -std=c++23)
target_include_directories(bench_input PRIVATE ${PARENT_PARENT_DIR})
FIND_LIBRARY(LIBRT rt)
target_link_libraries(bench_input LINK_PUBLIC ${LIBRT})

#message(PROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}")
#message(PARENT_DIR="${PARENT_DIR}")
#message(PARENT_PARENT_DIR="${PARENT_PARENT_DIR}")
