// Hand-checked cases for bitboard.h. Every expected value below was worked out on the
// 8x8 picture first, not by running the code.
#include <cassert>
#include <iostream>
#include <bit>
#include <vector>

#include "chess/bitboard.h"

using namespace chess;

int main()
{
    // square_mask: exactly one bit set, in the right place.
    assert(square_mask(0) == 1);                   // a1
    assert(square_mask(3) == 0b1000);              // d1, the 8-bit example from the lesson
    assert(square_mask(15) == 0x8000);             // h2
    assert(square_mask(63) == 0x8000000000000000); // h8, the top bit

    // The start position's white pawns: a2..h2 = bits 8..15.
    const Bitboard white_pawns = 0xFF00;
    assert(has_piece(white_pawns, 12));  // e2: pawn
    assert(has_piece(white_pawns, 15));  // h2: pawn
    assert(!has_piece(white_pawns, 28)); // e4: empty
    assert(!has_piece(white_pawns, 0));  // a1: no pawn

    // The 1 << 40 trap: square 40 (a6) must not be confused with square 8 (a2).
    assert(!has_piece(white_pawns, 40));
    assert(has_piece(square_mask(40), 40));
    assert(!has_piece(square_mask(40), 8));

    // place / remove, the lesson's 8-bit examples.
    assert(place_piece(0b0110'0010, 3) == 0b0110'1010);
    assert(remove_piece(0b0110'1010, 3) == 0b0110'0010);

    // Placing on an occupied square or removing from an empty one changes nothing.
    assert(place_piece(white_pawns, 12) == white_pawns);
    assert(remove_piece(white_pawns, 28) == white_pawns);

    // A move is remove + place: pawn e2 -> e4.
    const Bitboard after = place_piece(remove_piece(white_pawns, 12), 28);
    assert(!has_piece(after, 12));
    assert(has_piece(after, 28));
    assert(after == 0x1000EF00);
    // set-bit loop: collect every set square into a vector
    // step 1: b = square_mask(5) | square_mask(7)
    Bitboard b = square_mask(5) | square_mask(7);
    std::vector<int> squares;
    while (b != 0)
    {
        int sq = std::countr_zero(b);
        squares.push_back(sq);
        b &= b - 1;
    }

    assert(squares.size() == 2);
    assert(squares[0] == 5);
    assert(squares[1] == 7);

    std::cout << "bitboard_test: all passed\n";
}
