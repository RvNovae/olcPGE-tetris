cmake_minimum_required(VERSION 3.17)
project(pge_tetris_2)

set(CMAKE_CXX_STANDARD 20)

add_executable(pge_tetris_2 main.cpp playfield.cpp playfield.h tetrimino.cpp tetrimino.h tetriminoType.h userAction.h)