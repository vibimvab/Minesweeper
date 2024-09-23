cmake_minimum_required(VERSION 3.14)
project(Minesweeper)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES src/main.cpp src/Window.h src/Window.cpp src/Board.cpp src/Board.h src/Tile.cpp src/Tile.h src/ImageTextures.cpp src/ImageTextures.h src/Button.cpp src/Button.h)
add_executable(Minesweeper ${SOURCE_FILES})
include_directories(/usr/local/include)

find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
include_directories(${SFML_INCLUDE_DIRS})
target_link_libraries(Minesweeper sfml-system sfml-window sfml-graphics sfml-audio sfml-network)
