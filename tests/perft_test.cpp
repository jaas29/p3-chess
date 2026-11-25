#include <cassert>
#include <iostream>

#include "chess/movegen.h"
#include "chess/perft.h"

using namespace chess;

int main()
{
    Board b = Board::from_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1");
    assert(perft(b, 0) == 1);
    assert(perft(b, 1) == 20);
    assert(perft(b, 2) == 400);
    assert(perft(b, 3) == 8902);
    // depth 4 is the first one where checks matter
    assert(perft(b, 4) == 197281);
    // depth 5 is the first with en passant captures (258 of them)
    assert(perft(b, 5) == 4865609);
    Board p3 = Board::from_fen("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");
    assert(perft(p3, 4) == 43238);
}