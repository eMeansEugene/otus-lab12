cmake_minimum_required(VERSION 3.14)
project(map_reduce LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)


add_library(project_warnings INTERFACE)
target_compile_options(project_warnings INTERFACE -Wall -Wextra -pedantic)



add_library(mapreduce_core
        csv_price.cpp
        price_accumulator.cpp
        reducer_core.cpp
        reducer_core.h
)
target_include_directories(mapreduce_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(mapreduce_core PRIVATE project_warnings)


add_executable(mapper_mean   mapper.cpp)
add_executable(mapper_var    mapper.cpp)
add_executable(reducer_mean  reducer_mean.cpp)
add_executable(reducer_var   reducer_var.cpp)
foreach(bin mapper_mean mapper_var reducer_mean reducer_var)
    target_link_libraries(${bin} PRIVATE mapreduce_core project_warnings)
endforeach()

# --- Тесты ---
enable_testing()


find_package(GTest QUIET)
if(NOT GTest_FOUND)
    include(FetchContent)
    FetchContent_Declare(googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG        v1.15.2)
    FetchContent_MakeAvailable(googletest)
endif()

include(GoogleTest)

foreach(test_name test_csv_price test_price_accumulator)
    add_executable(${test_name} tests/${test_name}.cpp)
    target_link_libraries(${test_name}
            PRIVATE mapreduce_core GTest::gtest GTest::gtest_main project_warnings)
    gtest_discover_tests(${test_name})
endforeach()

