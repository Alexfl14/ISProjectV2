cmake_minimum_required(VERSION 3.20)
project(BlackjackTests)

# Explicitly list all test files (NO SimpleTest.cpp)
set(TEST_SOURCES
    src/core_tests/CardTests.cpp
    src/core_tests/DeckTests.cpp
    src/core_tests/FakeDeck.h
    src/core_tests/GameFlowTests.cpp
    src/core_tests/HandTests.cpp
    src/core_tests/PlayerTests.cpp
    src/core_tests/DealerStrategyTests.cpp
)

# Create Executable
add_executable(BlackjackTests ${TEST_SOURCES})

# Link Dependencies
target_link_libraries(BlackjackTests
    PRIVATE
    BlackjackCore
    GTest::gtest
    GTest::gtest_main
)

# Include Directories
target_include_directories(BlackjackTests
    PRIVATE
    ${CMAKE_SOURCE_DIR}/BlackjackCore/include
    ${CMAKE_SOURCE_DIR}/BlackjackUI/include
)

# Compiler options
if(MSVC)
    target_compile_options(BlackjackTests PRIVATE /W4)
else()
    target_compile_options(BlackjackTests PRIVATE -Wall -Wextra -Wpedantic)
endif()

# USE add_test() NOT gtest_discover_tests()
enable_testing()
add_test(NAME BlackjackTests COMMAND BlackjackTests)
