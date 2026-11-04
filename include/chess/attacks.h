#pragma once
#include "chess/bitboard.h"

namespace chess
{
    // Every square a knight on sq could jump to (ignoring whats on those squares).
    inline Bitboard knight_attacks(int sq)
    {
        const int rank = sq / 8;
        const int file = sq % 8;

        // the 8 (rank change, file change) pairs from your table
        constexpr int moves[8][2] = {{+2, +1}, {+2, -1}, {+1, +2}, {+1, -2}, {-1, +2}, {-1, -2}, {-2, +1}, {-2, -1}};

        Bitboard result = 0;
        for (const auto &m : moves)
        {
            const int r = rank + m[0];
            const int f = file + m[1];
            if (r >= 0 && r < 8 && f >= 0 && f < 8) // still on the board? (the h1->b3 fix)
            {
                result = place_piece(result, r * 8 + f);
            }
        }
        return result;
    }

    inline Bitboard king_attacks(int sq)
    {
        const int rank = sq / 8;
        const int file = sq % 8;

        constexpr int moves[8][2] = {{0, +1}, {0, -1}, {+1, 0}, {-1, 0}, {+1, +1}, {+1, -1}, {-1, +1}, {-1, -1}};
        Bitboard result = 0;
        for (const auto &m : moves)
        {
            const int r = rank + m[0];
            const int f = file + m[1];
            if (r >= 0 && r < 8 && f >= 0 && f < 8) // still on the board? (the h1->b3 fix)
            {
                result = place_piece(result, r * 8 + f);
            }
        }
        return result;
    }

}