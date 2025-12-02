# Blackjack Game

A C++ implementation of the classic Blackjack card game with a graphical user interface built using Raylib.

## Features

- Object-oriented design with proper separation of concerns
- Core game logic (BlackjackCore)
- Graphical user interface (BlackjackUI) using Raylib
- Comprehensive unit tests using Google Test
- Observer pattern implementation for game state updates
- Cross-platform support (Windows)

## Project Structure

```
blackjack/
├── BlackjackCore/       # Core game logic and engine
│   ├── include/         # Header files
│   └── src/             # Implementation files
├── BlackjackUI/         # User interface
│   ├── include/         # UI header files
│   ├── src/             # UI implementation
│   └── main.cpp         # Entry point
└── BlackjackTests/      # Unit tests
    └── src/             # Test files
```

## Dependencies

- CMake 3.20+
- C++17 compiler
- vcpkg (for package management)
- Raylib (graphics library)
- Google Test (for testing)
- nlohmann/json (JSON parsing)

## Building

### Prerequisites

1. Install Visual Studio 2022 with C++ development tools
2. Install vcpkg

### Build Instructions

```bash
# Configure with CMake
cmake -B build -S .

# Build
cmake --build build --config Debug

# Run tests
cd build
ctest -C Debug
```

## Running

After building, run the executable:

```bash
./build/bin/BlackjackUI.exe
```

## Testing

The project includes comprehensive unit tests for:
- Card mechanics
- Deck operations
- Hand calculations
- Player actions
- Game flow

Run tests using:
```bash
cd build
ctest -C Debug --verbose
```

## License

This project is part of academic coursework.
