#include <cassert>
#include <iostream>

#include "chess/board.h"

using namespace chess;

int main()
{
    Board b;
    assert(b.piece_at(28) == Piece::No_piece); // empty board: e4 is empty

    b.place(Piece::W_knight, 6); // knight on g1
    assert(b.piece_at(6) == Piece::W_knight);
    assert(b.piece_at(1) == Piece::No_piece); // b1 still empty

    b.place(Piece::B_king, 60); // black king on e8
    assert(b.piece_at(60) == Piece::B_king);

    std::cout << "board_test: all passed\n";
}