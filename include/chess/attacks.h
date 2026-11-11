#pragma once
#include "chess/bitboard.h"

namespace chess
{

    constexpr Bitboard FILE_A = 0x0101010101010101;
    constexpr Bitboard FILE_H = 0x8080808080808080;
    constexpr Bitboard RANK_3 = 0xFF0000;
    constexpr Bitboard RANK_6 = 0x0000FF0000000000;

    // All white pawns pushed one squared forward, onto emptyu squares only.
    inline Bitboard white_pawn_pushes(Bitboard pawns, Bitboard empty)
    {
        return (pawns << 8) & empty;
    }
    // Sqaures white pawns attacj up right. h-file pawns removed first: no wrap
    inline Bitboard white_pawn_attacks_right(Bitboard pawns)
    {
        return (pawns & ~FILE_H) << 9;
    }
    inline Bitboard white_pawn_attacks_left(Bitboard pawns)
    {
        return (pawns & ~FILE_A) << 7;
    }
    inline Bitboard white_pawn_double_pushes(Bitboard pawns, Bitboard empty)
    {
        const Bitboard one_step = white_pawn_pushes(pawns, empty);
        const Bitboard from_start = one_step & RANK_3;
        return white_pawn_pushes(from_start, empty);
    }

    inline Bitboard black_pawn_pushes(Bitboard pawns, Bitboard empty)
    {
        return (pawns >> 8) & empty;
    }
    inline Bitboard black_pawn_attacks_left(Bitboard pawns)
    {
        return (pawns & ~FILE_A) >> 9;
    }
    inline Bitboard black_pawn_attacks_right(Bitboard pawns)
    {
        return (pawns & ~FILE_H) >> 7;
    }
    inline Bitboard black_pawn_double_pushes(Bitboard pawns, Bitboard empty)
    {
        const Bitboard one_step = black_pawn_pushes(pawns, empty);
        const Bitboard from_start = one_step & RANK_6;
        return black_pawn_pushes(from_start, empty);
    }
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

    inline Bitboard rook_attacks(int sq, Bitboard occupied)
    {
        const int rank = sq / 8;
        const int file = sq % 8;

        constexpr int directions[4][2] = {{+1, 0}, {-1, 0}, {0, +1}, {0, -1}};
        Bitboard result = 0;
        for (const auto &d : directions)
        {
            int r = rank;
            int f = file;
            while (true)
            {
                r += d[0];
                f += d[1];
                if (r < 0 || r > 7 || f < 0 || f > 7)
                {
                    break;
                }
                result = place_piece(result, r * 8 + f);
                if (has_piece(occupied, r * 8 + f))
                {
                    break;
                }
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