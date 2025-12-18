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

### SETUP
1. Clone this repository 
2. Install the vcpkg on your computer
3. Open the terminal and go to the root of the vcpkg installation folder, run git rev-parse HEAD and copy the baseline
4. Paste the baseline value you just copied in vcpkg.json's "builtin-baseline" entry.
5. Create and environment variable 'VCPKG_ROOT=path\to\vcpkg' (the fist layer inside vcpkg directory)
6. Open the project as directory in VisualStudio



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
