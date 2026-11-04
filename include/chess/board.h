#pragma once
#include <array>
#include "chess/bitboard.h"
#include <iostream>
#include <string_view>
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
        static char piece_char(Piece p)
        {
            if (p == Piece::No_piece)
            {
                return '.';
            }
            return "PNBRQKpnbrqk"[static_cast<int>(p)];
        }

        //'N' -> Piece::W_Knight. Anything that isn't a piece letter -> No piece
        static Piece piece_from_char(char c)
        {
            const std::string_view letters = "PNBRQKpnbrqk";
            const auto i = letters.find(c);
            if (i == std::string_view::npos)
            {
                return Piece::No_piece;
            }
            return static_cast<Piece>(i);
        }

        static Board from_fen(std::string_view fen)
        {
            Board b;
            int rank = 7; // rank 8= index 7
            int file = 0;

            for (char c : fen)
            {
                if (c == ' ')
                {
                    break;
                }
                else if (c == '/')
                {
                    rank = rank - 1;
                    file = 0;
                }
                else if (c >= '1' && c <= '8')
                {
                    file += c - '0';
                }
                else
                {
                    b.place(piece_from_char(c), rank * 8 + file);
                    file++;
                }
            }
            return b;
        }
        void print() const
        {
            for (int rank = 7; rank >= 0; rank--)
            {
                std::cout << rank + 1 << "  ";
                for (int file = 0; file < 8; file++)
                {
                    int sq = rank * 8 + file;
                    std::cout << piece_char(piece_at(sq)) << " ";
                }
                std::cout << '\n';
            }
            std::cout << "   a b c d e f g h\n";
        }
    };
}