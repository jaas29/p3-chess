#include <cassert>  // gives us assert
#include <iostream> // gives us std::cout

#include "chess/move.h"

using namespace chess;

int main()
{
    const Move m = Move{52, 62, Piece::W_queen, MoveFlag::Normal};
    assert(m.from == 52);
    assert(m.to == 62);
    assert(m.promotion == Piece::W_queen);
    assert(m.flag == MoveFlag::Normal);
    const Move k = Move{6, 21};
    assert(k.promotion == Piece::No_piece);
    assert(k.flag == MoveFlag::Normal);
}
