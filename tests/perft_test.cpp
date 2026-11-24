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
}