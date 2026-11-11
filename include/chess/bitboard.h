#pragma once

#include <cstdint>

namespace chess
{

    // One bit per square. Square numbers: sq = rank * 8 + file, a1 = 0, h1 = 7, a2 = 8, h8 = 63.
    using Bitboard = std::uint64_t;

    // 1. square_mask(sq)       -> a Bitboard with only bit sq set (one hole in the sheet).
    inline Bitboard square_mask(int sq)
    {
        return Bitboard{1} << sq;
    }
    //                             Watch the type of the 1 before you shift it.
    // 2. has_piece(b, sq)      -> true if b has a piece on sq.        Uses square_mask and &.
    inline bool has_piece(Bitboard b, int sq)
    {
        return (b & square_mask(sq)) != 0;
    }
    // 3. place_piece(b, sq)    -> b with a piece added on sq.         Uses square_mask and |.
    inline Bitboard place_piece(Bitboard b, int sq)
    {
        return b | square_mask(sq);
    }
    // 4. remove_piece(b, sq)   -> b with the piece on sq taken off.   Uses square_mask, ~ and &.
    inline Bitboard remove_piece(Bitboard b, int sq)
    {
        return b & ~square_mask(sq);
    }

}
