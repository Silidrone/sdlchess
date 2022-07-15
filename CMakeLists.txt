cmake_minimum_required(VERSION 3.22)
project(chess)

set(CMAKE_CXX_STANDARD 17)
target_link_libraries(SDL2, SDL2_image)
add_executable(chess main.cpp headers/Board.h headers/Game.h headers/MTexture.h headers/Piece.h headers/SharedData.h sources/Game.cpp)
