#pragma once
#include <array>
#include "chess/bitboard.h"

namespace chess
{

    enum class Piece
    {
        W_pawn,
        W_knight,
        W_bishop,
        W_rook,
        W_queen,
        W_king,
        B_pawn,
        B_knight,
        B_bishop,
        B_rook,
        B_queen,
        B_king,
        Count,
        No_piece
    };

    struct Board
    {
        // 12 sheets, one per piece. {}makes every sheet 0 = no holes = emptyboard
        std::array<Bitboard, static_cast<int>(Piece::Count)> pieces{};

        // put piece p on square sq. Uses place_piece form bitboard.h
        void place(Piece p, int sq)
        {
            const int i = static_cast<int>(p);      // enum -> index
            pieces[i] = place_piece(pieces[i], sq); // plae_piece return a NEW value; store it
        }

        Piece piece_at(int sq) const
        {
            for (int i = 0; i < static_cast<int>(Piece::Count); i++)
            {
                if (has_piece(pieces[i], sq))
                {
                    return static_cast<Piece>(i);
                }
            }
            return Piece::No_piece;
        }
    };
}