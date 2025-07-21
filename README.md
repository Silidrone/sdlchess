# SDL Chess

A chess game implemented in C++ using SDL 2.0 with a chess engine and PGN testing framework. Chess rules obeyed following the FIDE Laws of Chess article: https://www.fide.com/FIDE/handbook/LawsOfChess.pdf

### Features

- Full chess game with all standard rules (castling, en passant, promotion)
- SDL2 graphics with piece sprites and interactive board
- Press `M` during gameplay to view game notation
- PGN parser that validates the engine against grandmaster games
- Move validation system

Tested against thousands of grandmaster games including:
- Complex endgames
- Smothered mate scenarios
- Games with unusual moves (underpromotions, 260+ move games, checkmating with en passant)
- Edge cases and rare positions


### Project Structure

- `main.cpp` Main game executable
- `tester.cpp` PGN parser and engine validation tool
- `sources/` Chess engine implementation
- `headers/` Header files for pieces and game logic
- `resources/` Game assets (sprites, fonts, textures)
- `pgn_games/` PGN files for testing

### Prerequisites
- SDL2 development libraries
- SDL2_image development libraries
- g++ compiler

### Technical Details
- Object-oriented design with separate classes for each piece type
- Move validation system
- Support for all chess rules including special moves
- Memory-safe C++ implementation
- Cross-platform through SDL2
