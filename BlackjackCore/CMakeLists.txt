cmake_minimum_required(VERSION 3.20)

file(GLOB_RECURSE CORE_SOURCES "src/*.cpp")
file(GLOB_RECURSE CORE_HEADERS_FOR_COMPILATION "include/*.h" "include/*.hpp")
list(APPEND CORE_SOURCES ${CORE_HEADERS_FOR_COMPILATION})

# BlackjackCore library
add_library(BlackjackCore STATIC
        ${CORE_SOURCES}
)

# Create alias for consistent naming
add_library(Blackjack::Core ALIAS BlackjackCore)

# Include directories
target_include_directories(BlackjackCore
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Link dependencies
target_link_libraries(BlackjackCore
        PUBLIC
        nlohmann_json::nlohmann_json
)

# Compiler warnings
if(MSVC)
    target_compile_options(BlackjackCore PRIVATE /W4)
else()
    target_compile_options(BlackjackCore PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Installation rules (optional, for later)
install(TARGETS BlackjackCore
        EXPORT BlackjackCoreTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
)

install(DIRECTORY include/
        DESTINATION include
)