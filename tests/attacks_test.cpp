#include <bit> // std::popcount: how many bits are set = how many target squares
#include <cassert>
#include <iostream>

#include "chess/attacks.h"

using namespace chess;

int main()
{
    // Knight on d4 (27): your 8 squares e6 c6 f5 b5 f3 b3 e2 c2.
    const Bitboard n_d4 = knight_attacks(27);
    assert(std::popcount(n_d4) == 8);
    assert(has_piece(n_d4, 44) && has_piece(n_d4, 42) && has_piece(n_d4, 37) && has_piece(n_d4, 33));
    assert(has_piece(n_d4, 21) && has_piece(n_d4, 17) && has_piece(n_d4, 12) && has_piece(n_d4, 10));

    // Knight on h1 (7): only g3 (22) and f2 (13). NOT b3 (17), the wrap-around square.
    const Bitboard n_h1 = knight_attacks(7);
    assert(std::popcount(n_h1) == 2);
    assert(has_piece(n_h1, 22) && has_piece(n_h1, 13));
    assert(!has_piece(n_h1, 17));

    // Knight on g1 (6), where it starts: e2 (12), f3 (21), h3 (23).
    assert(std::popcount(knight_attacks(6)) == 3);

    // King on d4: all 8 neighbours.  King in a corner (a1): only b1, a2, b2.
    assert(std::popcount(king_attacks(27)) == 8);
    assert(king_attacks(0) == (square_mask(1) | square_mask(8) | square_mask(9)));

    // King on h1 (7): +1 file would wrap to a2 (8). Must not be there.
    assert(!has_piece(king_attacks(7), 8));
    assert(std::popcount(king_attacks(7)) == 3); // g1, g2, h2

    std::cout << "attacks_test: all passed\n";
}