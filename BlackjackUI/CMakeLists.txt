cmake_minimum_required(VERSION 3.20)

# Gather source files
file(GLOB_RECURSE UI_SOURCES "src/*.cpp")
file(GLOB_RECURSE UI_HEADERS "include/*.h" "include/*.hpp")

# BlackjackUI executable
add_executable(BlackjackUI
    ${UI_SOURCES}
    "main.cpp"
)

# Link dependencies
if(UNIX AND NOT APPLE)
    target_link_libraries(BlackjackUI
        PRIVATE
        Blackjack::Core
        raylib
        glfw
    )
else()
    target_link_libraries(BlackjackUI
        PRIVATE
        Blackjack::Core
        raylib
    )
endif()

# Include directories
target_include_directories(BlackjackUI
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Compiler warnings
if(MSVC)
    target_compile_options(BlackjackUI PRIVATE /W4)
else()
    target_compile_options(BlackjackUI PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Copy assets to build directory (for future use)
# add_custom_command(TARGET BlackjackUI POST_BUILD
#         COMMAND ${CMAKE_COMMAND} -E copy_directory
#         ${CMAKE_CURRENT_SOURCE_DIR}/assets
#         ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets
#         COMMENT "Copying assets to build directory"
# )

# Installation rules
install(TARGETS BlackjackUI
    RUNTIME DESTINATION bin
)
