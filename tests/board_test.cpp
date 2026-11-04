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
    b.print();
    const Board s = Board::from_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    assert(s.piece_at(0) == Piece::W_rook);    // a1
    assert(s.piece_at(4) == Piece::W_king);    // e1
    assert(s.piece_at(12) == Piece::W_pawn);   // e2
    assert(s.piece_at(28) == Piece::No_piece); // e4
    assert(s.piece_at(59) == Piece::B_queen);  // d8
    assert(s.piece_at(60) == Piece::B_king);   // e8
    assert(s.piece_at(63) == Piece::B_rook);   // h8
    s.print();
    std::cout << "board_test: all passed\n";
}