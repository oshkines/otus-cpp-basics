cmake_minimum_required(VERSION 3.10)
set(TEST_NAME container_test)
set(TEST_NAME_CONSISTENT container_test_consistent)
project(${TEST_NAME})

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
)
FetchContent_MakeAvailable(googletest)

add_executable(${TEST_NAME} 
    ${TEST_NAME}.cpp,
    ${TEST_NAME_CONSISTENT}.cpp    
)

target_link_libraries(${TEST_NAME}
    gtest_main
    gmock_main
)

target_include_directories(${TEST_NAME}
PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

enable_testing()