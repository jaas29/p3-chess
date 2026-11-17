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
    enum class Color
    {
        White,
        Black,
    };

    constexpr int CASTLE_WK = 1;
    constexpr int CASTLE_WQ = 2;
    constexpr int CASTLE_BK = 4;
    constexpr int CASTLE_BQ = 8;

    struct Board
    {
        // 12 sheets, one per piece. {}makes every sheet 0 = no holes = emptyboard
        std::array<Bitboard, static_cast<int>(Piece::Count)> pieces{};
        int castling = 0;
        int ep_square = -1;
        Color side_to_move = Color::White;

        // put piece p on square sq. Uses place_piece form bitboard.h
        void place(Piece p, int sq)
        {
            const int i = static_cast<int>(p);      // enum -> index
            pieces[i] = place_piece(pieces[i], sq); // plae_piece return a NEW value; store it
        }
        // every square holding a piece of color c: the 6 sheets of c OR'd together
        Bitboard color_pieces(Color c) const
        {
            int start = 0;
            if (c == Color::Black)
            {
                start = 6;
            }
            Bitboard result = 0;
            for (int i = start; i < start + 6; i++)
            {
                result |= pieces[i];
            }
            return result;
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
            const std::size_t pos = fen.find(' ');

            if (fen[pos + 1] == 'b')
            {
                b.side_to_move = Color::Black;
            }
            std::size_t i = pos + 3;
            while (i < fen.size() && fen[i] != ' ')
            {
                const char c = fen[i];
                if (c == 'K')
                {
                    b.castling |= CASTLE_WK;
                }
                else if (c == 'Q')
                {
                    b.castling |= CASTLE_WQ;
                }
                else if (c == 'k')
                {
                    b.castling |= CASTLE_BK;
                }
                else if (c == 'q')
                {
                    b.castling |= CASTLE_BQ;
                }
                i++;
            }
            // field 4 en passant square
            if (fen[i + 1] != '-')
            {
                file = fen[i + 1] - 'a';
                rank = fen[i + 2] - '1';
                b.ep_square = rank * 8 + file;
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
