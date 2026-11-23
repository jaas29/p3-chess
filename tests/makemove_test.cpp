#include <cassert>
#include <iostream>

#include "chess/makemove.h"

using namespace chess;

int main()
{
    Board b = Board::from_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    Move m{12, 28};
    make_move(b, m);
    assert(b.piece_at(12) == Piece::No_piece);
    assert(b.piece_at(28) == Piece::W_pawn);
    assert(b.side_to_move == Color::Black);

    Board c = Board::from_fen("rnbqkbnr/ppp1pppp/8/3p4/4P3/8/PPPP1PPP/RNBQKBNR w KQkq d6 0 2");
    Move p{28, 35};
    Undo u = make_move(c, p);
    assert(c.piece_at(28) == Piece::No_piece);
    assert(c.piece_at(35) == Piece::W_pawn);
    assert(u.captured == Piece::B_pawn);
    assert(!has_piece(c.pieces[static_cast<int>(Piece::B_pawn)], 35));
    // make then unmake gives back the same board: quiet move
    Board before = Board::from_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    Board r = before;
    Undo u1 = make_move(r, m);
    unmake_move(r, m, u1);
    assert(r == before);
    // make then unmake gives back the same board: capture
    Board before2 = Board::from_fen("rnbqkbnr/ppp1pppp/8/3p4/4P3/8/PPPP1PPP/RNBQKBNR w KQkq d6 0 2");
    Board r2 = before2;
    Undo u2 = make_move(r2, p);
    unmake_move(r2, p, u2);
    assert(r2 == before2);
}