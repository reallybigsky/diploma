cmake_minimum_required(VERSION 3.26)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++ -lc++abi")
#set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(diploma)

set(FUZZTEST_FUZZING_MODE "on")

include(FetchContent)
include(GoogleTest)
find_package(Boost REQUIRED COMPONENTS filesystem)
add_subdirectory(lib/fuzztest)


##gen target
#add_executable(gen
#        gen/main.cpp
#)
#target_compile_options(gen PRIVATE -Wall -Wextra -std=c++23)
#target_include_directories(gen PRIVATE ${PROJECT_SOURCE_DIR})

#bench_output target
add_executable(bench_output
        test/bench/output/main.cpp
        lib/MemoryPool/MemoryPool.cpp
)
target_compile_options(bench_output PRIVATE -Wall -Wextra -std=c++23)
target_include_directories(bench_output PRIVATE ${PROJECT_SOURCE_DIR})

#bench_input target
add_executable(bench_input
        test/bench/input/main.cpp
        lib/MemoryPool/MemoryPool.cpp
)
target_compile_options(bench_input PRIVATE -Wall -Wextra -std=c++23)
target_include_directories(bench_input PRIVATE ${PROJECT_SOURCE_DIR})

#simple_test target
add_executable(simple_test
        test/unit/test.cpp
        lib/MemoryPool/MemoryPool.cpp
)
target_compile_options(simple_test PRIVATE -fstrict-flex-arrays=2 -Wall -Wextra -g -fno-omit-frame-pointer -std=c++23 -fsanitize=address)
target_include_directories(simple_test PRIVATE ${PROJECT_SOURCE_DIR})
target_link_libraries(simple_test PRIVATE gtest)
target_link_options(simple_test PRIVATE -fsanitize=address)

###################
####  FUZZING  ####
###################
fuzztest_setup_fuzzing_flags()

#simple_fuzz_test target
add_executable(simple_fuzz_test
        test/fuzz/simple/test.cpp
        lib/MemoryPool/MemoryPool.cpp
)
target_include_directories(simple_fuzz_test PRIVATE ${PROJECT_SOURCE_DIR})
target_link_libraries(simple_fuzz_test PRIVATE gtest)
target_compile_options(simple_fuzz_test PRIVATE -fstrict-flex-arrays=2 -g -DFUZZING_BUILD_MODE_UNSAFE_FOR_PRODUCTION -UNDEBUG -fsanitize-coverage=inline-8bit-counters -fsanitize-coverage=trace-cmp -fsanitize=address -DADDRESS_SANITIZER -std=c++23)
target_link_options(simple_fuzz_test PRIVATE -fsanitize=address)

link_fuzztest(simple_fuzz_test)
gtest_discover_tests(simple_fuzz_test)

#statshouse_fuzz_test target
add_executable(statshouse_fuzz_test
        test/fuzz/statshouse/test.cpp
        lib/MemoryPool/MemoryPool.cpp
)
target_include_directories(statshouse_fuzz_test PRIVATE ${PROJECT_SOURCE_DIR})
target_link_libraries(statshouse_fuzz_test PRIVATE gtest)
target_compile_options(statshouse_fuzz_test PRIVATE -fstrict-flex-arrays=2 -g -DFUZZING_BUILD_MODE_UNSAFE_FOR_PRODUCTION -UNDEBUG -fsanitize-coverage=inline-8bit-counters -fsanitize-coverage=trace-cmp -fsanitize=address -DADDRESS_SANITIZER -std=c++23)
target_link_options(statshouse_fuzz_test PRIVATE -fsanitize=address)

link_fuzztest(statshouse_fuzz_test)
gtest_discover_tests(statshouse_fuzz_test)
