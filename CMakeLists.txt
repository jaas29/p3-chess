cmake_minimum_required(VERSION 3.28)
project(chess LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Same warnings as P2, written once instead of copied per target.
function(chess_target name source)
  add_executable(${name} ${source})
  target_include_directories(${name} PRIVATE include)
  target_compile_options(${name} PRIVATE -Wall -Wextra -Wpedantic -Werror -ferror-limit=0)
endfunction()

chess_target(bitboard_test tests/bitboard_test.cpp)
chess_target(board_test tests/board_test.cpp)
chess_target(attacks_test tests/attacks_test.cpp)