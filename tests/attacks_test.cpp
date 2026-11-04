// HOW TO READ THIS FILE
// assert(something) means: "this must be true". If it is true, nothing happens and the
// program goes to the next line. If it is false, the program stops right there and prints
// the line number, so you know exactly which check failed.
// If the program reaches the last line, every check was true: "all passed".

#include <bit>      // gives us std::popcount: counts how many bits are 1 = how many squares
#include <cassert>  // gives us assert
#include <iostream> // gives us std::cout, to print the final message

#include "chess/attacks.h" // the functions we are testing

using namespace chess; // so we can write knight_attacks instead of chess::knight_attacks

int main()
{
    // ---------- KNIGHT IN THE MIDDLE: d4 ----------

    // Ask: "where can a knight on d4 (square 27) go?" The answer is a bitboard,
    // a sheet with a hole on every square it can jump to. We save it in n_d4.
    const Bitboard n_d4 = knight_attacks(27);

    // Count the holes. A knight in the middle of the board has 8 moves.
    assert(std::popcount(n_d4) == 8);

    // Check 4 of the 8 squares one by one: e6 (44), c6 (42), f5 (37), b5 (33).
    // && means "and": all four must have a hole.
    assert(has_piece(n_d4, 44) && has_piece(n_d4, 42) && has_piece(n_d4, 37) && has_piece(n_d4, 33));

    // The other 4 squares: f3 (21), b3 (17), e2 (12), c2 (10).
    assert(has_piece(n_d4, 21) && has_piece(n_d4, 17) && has_piece(n_d4, 12) && has_piece(n_d4, 10));

    // ---------- KNIGHT IN THE CORNER: h1 (the wrap-around test) ----------

    // Ask: "where can a knight on h1 (square 7) go?"
    const Bitboard n_h1 = knight_attacks(7);

    // In the corner, most moves fall off the board. Only 2 are left.
    assert(std::popcount(n_h1) == 2);

    // Those 2 are g3 (22) and f2 (13).
    assert(has_piece(n_h1, 22) && has_piece(n_h1, 13));

    // if the code just added 10 to the square number (the wrap-around bug).
    assert(!has_piece(n_h1, 17));

    // ---------- KNIGHT ON ITS START SQUARE: g1 ----------

    // A knight on g1 (6) has 3 moves: e2, f3, h3. We only count them here.
    assert(std::popcount(knight_attacks(6)) == 3);

    // ---------- KING ----------

    // A king on d4 (27) touches all 8 squares around it.
    assert(std::popcount(king_attacks(27)) == 8);

    // A king on a1 (0) has only 3 neighbours: b1 (1), a2 (8), b2 (9).
    // We build that answer ourselves with | (put the 3 holes on one sheet)
    // and check the function gives exactly the same sheet, no more, no less.
    assert(king_attacks(0) == (square_mask(1) | square_mask(8) | square_mask(9)));

    // A king on h1 (7): moving one file right would wrap to a2 (8). It must NOT be there.
    assert(!has_piece(king_attacks(7), 8));

    // And it has exactly 3 moves: g1, g2, h2.
    assert(std::popcount(king_attacks(7)) == 3);

    // ---------- PAWN PUSHES ----------

    // 0xFF00 = all 8 white pawns on rank 2 (bits 8 to 15).
    // ~Bitboard{0} = a sheet with ALL 64 holes = "every square is empty".
    // Pushing them one rank up must give all 8 on rank 3 = 0xFF0000 (bits 16 to 23).
    assert(white_pawn_pushes(0xFF00, ~Bitboard{0}) == 0xFF0000);

    // Same pawns, but now e3 (20) is NOT empty: ~square_mask(20) = every square
    // empty except e3. The e-pawn is blocked, so the answer is rank 3 without e3.
    assert(white_pawn_pushes(0xFF00, ~square_mask(20)) == (0xFF0000 & ~square_mask(20)));

    // ---------- PAWN CAPTURES ON THE RIGHT EDGE: h2 ----------

    // A pawn on h2 (15) cannot capture to the right: there is no file after h.
    // 0 = an empty sheet = no squares. (Without the FILE_H mask it would say a4.)
    assert(white_pawn_attacks_right(square_mask(15)) == 0);

    // Its only capture is up-left, onto g3 (22).
    assert(white_pawn_attacks_left(square_mask(15)) == square_mask(22));

    // ---------- PAWN CAPTURES ON THE LEFT EDGE: a2 ----------

    // A pawn on a2 (8) cannot capture to the left: there is no file before a.
    // (Without the FILE_A mask it would say h2.)
    assert(white_pawn_attacks_left(square_mask(8)) == 0);

    // Its only capture is up-right, onto b3 (17).
    assert(white_pawn_attacks_right(square_mask(8)) == square_mask(17));

    // ---------- PAWN CAPTURES IN THE MIDDLE: e2 ----------

    // A pawn on e2 (12) captures up-left onto d3 (19)...
    assert(white_pawn_attacks_left(square_mask(12)) == square_mask(19));

    // ...and up-right onto f3 (21).
    assert(white_pawn_attacks_right(square_mask(12)) == square_mask(21));

    // We only get here if every assert above was true.
    std::cout << "attacks_test: all passed\n";
}
