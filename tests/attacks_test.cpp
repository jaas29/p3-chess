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
    // knight in the middle

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

    // knight in the corner: h1 (the wrap-around test)

    // Ask: "where can a knight on h1 (square 7) go?"
    const Bitboard n_h1 = knight_attacks(7);

    // In the corner, most moves fall off the board. Only 2 are left.
    assert(std::popcount(n_h1) == 2);

    // Those 2 are g3 (22) and f2 (13).
    assert(has_piece(n_h1, 22) && has_piece(n_h1, 13));

    // if the code just added 10 to the square number (the wrap-around bug).
    assert(!has_piece(n_h1, 17));

    // knight on its start sq: g1

    // A knight on g1 (6) has 3 moves: e2, f3, h3. We only count them here.
    assert(std::popcount(knight_attacks(6)) == 3);

    // knight targets: own pieces block

    // white's pieces in the start position: squares 0-15
    const Bitboard white_start = 0xFFFFULL;

    // knight on g1 (6) jumps to e2 (12), f3 (21), h3 (23).
    // e2 has a white pawn, so only f3 and h3 remain.
    const Bitboard expected_g1 = square_mask(21) | square_mask(23);
    assert(knight_targets(6, white_start) == expected_g1);
    // knight on b1 (1) jumps to d2 (11), a3 (16), c3 (18).
    // d2 has a white pawn, so only a3 and c3 remain.
    const Bitboard expected_b1 = square_mask(16) | square_mask(18);
    assert(knight_targets(1, white_start) == expected_b1);

    // king

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

    // King targets
    // king on e1 (4) touches d1 (3), f1 (5), d2 (11), e2 (12), f2 (13).
    // all five are white pieces, so the king has nowhere to go: 0.
    const Bitboard expected_e1 = 0;
    assert(king_targets(4, white_start) == expected_e1);
    // king on e3 (20) touches d2 (11), e2 (12), f2 (13), d3 (19), f3 (21),
    // d4 (27), e4 (28), f4 (29). The three rank-2 squares are white pawns,
    // so they are removed and five squares remain.
    const Bitboard expected_e3 = square_mask(19) | square_mask(21) | square_mask(27) | square_mask(29) | square_mask(28);
    assert(king_targets(20, white_start) == expected_e3);

    // Rook
    const Bitboard occ = square_mask(43) | square_mask(29);
    const Bitboard r_d4 = rook_attacks(27, occ); // save the result in a box
    assert(std::popcount(r_d4) == 10);

    assert(has_piece(r_d4, 43));
    assert(!has_piece(r_d4, 51));

    // left: c4 (26), b4 (25), a4 (24) to the edge.
    const Bitboard white_d6 = square_mask(43);
    const Bitboard expected_d4 = square_mask(35) | square_mask(28) | square_mask(29) | square_mask(19) | square_mask(11) | square_mask(3) | square_mask(26) | square_mask(25) | square_mask(24);
    assert(rook_targets(27, occ, white_d6) == expected_d4);
    // bishop
    const Bitboard empty_board = 0;
    const Bitboard b_d4 = bishop_attacks(27, empty_board);
    assert(std::popcount(b_d4) == 13);
    const Bitboard occu = square_mask(45);
    const Bitboard b_d4_blocked = bishop_attacks(27, occu);
    assert(has_piece(b_d4_blocked, 45));
    assert(!has_piece(b_d4_blocked, 54));

    // queen
    const Bitboard q_d4 = queen_attacks(27, empty_board);
    assert(std::popcount(q_d4) == 27);

    // bishop and queen targets

    // bishop on d4 (27). White piece on f6 (45), black piece on b2 (9).
    // up-right (+9): e5 (36), then f6 is our own piece, so it stops before it.
    // up-left (+7): c5 (34), b6 (41), a7 (48) to the edge.
    // down-right (-7): e3 (20), f2 (13), g1 (6) to the edge.
    // down-left (-9): c3 (18), b2 (9) is black, so it captures there and stops.
    const Bitboard bishop_occ = square_mask(45) | square_mask(9);
    const Bitboard white_f6 = square_mask(45);
    const Bitboard expected_bishop_d4 = square_mask(36) | square_mask(34) | square_mask(41) | square_mask(48) | square_mask(20) | square_mask(13) | square_mask(6) | square_mask(18) | square_mask(9);
    assert(bishop_targets(27, bishop_occ, white_f6) == expected_bishop_d4);

    // queen on d4 (27) with all four pieces from the rook and bishop tests.
    // None of the rook's blockers sit on a diagonal, and none of the bishop's
    // sit on the rank or file, so the queen's targets are exactly
    // the rook's targets | the bishop's targets.
    const Bitboard queen_occ = occ | bishop_occ;
    const Bitboard white_queen_test = white_d6 | white_f6;
    assert(queen_targets(27, queen_occ, white_queen_test) == (expected_d4 | expected_bishop_d4));

    // pawn pushes

    // 0xFF00 = all 8 white pawns on rank 2 (bits 8 to 15).
    // ~Bitboard{0} = a sheet with ALL 64 holes = "every square is empty".
    // Pushing them one rank up must give all 8 on rank 3 = 0xFF0000 (bits 16 to 23).
    assert(white_pawn_pushes(0xFF00, ~Bitboard{0}) == 0xFF0000);

    // Same pawns, but now e3 (20) is NOT empty: ~square_mask(20) = every square
    // empty except e3. The e-pawn is blocked, so the answer is rank 3 without e3.
    assert(white_pawn_pushes(0xFF00, ~square_mask(20)) == (0xFF0000 & ~square_mask(20)));

    // Pawn captures on the right edge

    // A pawn on h2 (15) cannot capture to the right: there is no file after h.
    // 0 = an empty sheet = no squares. (Without the FILE_H mask it would say a4.)
    assert(white_pawn_attacks_right(square_mask(15)) == 0);

    // Its only capture is up-left, onto g3 (22).
    assert(white_pawn_attacks_left(square_mask(15)) == square_mask(22));

    // pawn captures in teh left edge

    // A pawn on a2 (8) cannot capture to the left: there is no file before a.
    // (Without the FILE_A mask it would say h2.)
    assert(white_pawn_attacks_left(square_mask(8)) == 0);

    // Its only capture is up-right, onto b3 (17).
    assert(white_pawn_attacks_right(square_mask(8)) == square_mask(17));

    // pawn captures in the middle

    // A pawn on e2 (12) captures up-left onto d3 (19)...
    assert(white_pawn_attacks_left(square_mask(12)) == square_mask(19));

    // ...and up-right onto f3 (21).
    assert(white_pawn_attacks_right(square_mask(12)) == square_mask(21));

    // black pawns
    // Black moves DOWN the board, so every shift is >> (square number gets smaller).
    // "left" and "right" are as seen on the printed board, same as white:
    // left = toward the a-file, right = toward the h-file.

    // A black pawn on e7 (52), every square empty: one push lands on e6 (44).
    assert(black_pawn_pushes(square_mask(52), ~Bitboard{0}) == square_mask(44));

    // All 8 black pawns on rank 7 (0x00FF000000000000), every square empty:
    // they all land on rank 6 (0x0000FF0000000000).
    assert(black_pawn_pushes(0x00FF000000000000, ~Bitboard{0}) == 0x0000FF0000000000);

    // Same pawns, but e6 (44) is occupied: the e-pawn is blocked.
    assert(black_pawn_pushes(0x00FF000000000000, ~square_mask(44)) == (0x0000FF0000000000 & ~square_mask(44)));

    // A black pawn on e7 (52) captures down-left onto d6 (43, = 52 - 9)...
    assert(black_pawn_attacks_left(square_mask(52)) == square_mask(43));

    // ...and down-right onto f6 (45, = 52 - 7).
    assert(black_pawn_attacks_right(square_mask(52)) == square_mask(45));

    // A black pawn on a7 (48) has nothing to its left. Without the FILE_A mask,
    // >> 9 would wrap it to h5 (39).
    assert(black_pawn_attacks_left(square_mask(48)) == 0);

    // Its only capture is down-right, onto b6 (41).
    assert(black_pawn_attacks_right(square_mask(48)) == square_mask(41));

    // A black pawn on h7 (55) has nothing to its right. Without the FILE_H mask,
    // >> 7 would wrap it to a7 (48).
    assert(black_pawn_attacks_right(square_mask(55)) == 0);

    // Its only capture is down-left, onto g6 (46).
    assert(black_pawn_attacks_left(square_mask(55)) == square_mask(46));

    // double pushes

    // A white pawn on e2 (12), everything empty: it can go two squares, to e4 (28).
    assert(white_pawn_double_pushes(square_mask(12), ~Bitboard{0}) == square_mask(28));

    // Same pawn, but a piece sits on e3 (20): it cannot jump over it. No double push.
    assert(white_pawn_double_pushes(square_mask(12), ~square_mask(20)) == 0);

    // Same pawn, e3 empty but e4 (28) occupied: the landing square is blocked. No double push.
    assert(white_pawn_double_pushes(square_mask(12), ~square_mask(28)) == 0);

    // A white pawn on e3 (20) has already moved: no double push, even on an empty board.
    assert(white_pawn_double_pushes(square_mask(20), ~Bitboard{0}) == 0);

    // All 8 white pawns on rank 2, everything empty: all 8 land on rank 4 (bits 24 to 31).
    assert(white_pawn_double_pushes(0xFF00, ~Bitboard{0}) == 0xFF000000);

    // A black pawn on e7 (52), everything empty: two squares down, to e5 (36).
    assert(black_pawn_double_pushes(square_mask(52), ~Bitboard{0}) == square_mask(36));

    // Same pawn, a piece on e6 (44): blocked, no double push.
    assert(black_pawn_double_pushes(square_mask(52), ~square_mask(44)) == 0);

    // A black pawn on e6 (44) has already moved: no double push.
    assert(black_pawn_double_pushes(square_mask(44), ~Bitboard{0}) == 0);

    // We only get here if every assert above was true.
    std::cout << "attacks_test: all passed\n";
}
