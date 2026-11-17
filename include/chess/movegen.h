#pragma once
#include "chess/board.h"
#include "chess/attacks.h"
#include "chess/move.h"
#include <vector>
#include <bit>
namespace chess
{
    // add every knight move for side to moves
    inline void add_knight_moves(const Board &b, Color side, std::vector<Move> &moves)
    {
        Piece knight = Piece::W_knight;
        if (side == Color::Black)
        {
            knight = Piece::B_knight;
        }
        Bitboard own = b.color_pieces(side);
        Bitboard knights = b.pieces[static_cast<int>(knight)];

        while (knights != 0)
        {
            const int from = std::countr_zero(knights);
            Bitboard targets = knight_targets(from, own);

            while (targets != 0)
            {
                const int to = std::countr_zero(targets);
                moves.push_back(Move{from, to});
                targets &= targets - 1;
            }
            knights &= knights - 1;
        }
    }
    inline void add_king_moves(const Board &b, Color side, std::vector<Move> &moves)
    {
        Piece king = Piece::W_king;
        if (side == Color::Black)
        {
            king = Piece::B_king;
        }
        Bitboard own = b.color_pieces(side);
        Bitboard kings = b.pieces[static_cast<int>(king)];

        while (kings != 0)
        {
            const int from = std::countr_zero(kings);
            Bitboard targets = king_targets(from, own);

            while (targets != 0)
            {
                const int to = std::countr_zero(targets);
                moves.push_back(Move{from, to});
                targets &= targets - 1;
            }
            kings &= kings - 1;
        }
    }
    inline void add_rook_moves(const Board &b, Color side, std::vector<Move> &moves)
    {
        Piece rook = Piece::W_rook;
        if (side == Color::Black)
        {
            rook = Piece::B_rook;
        }
        Bitboard own = b.color_pieces(side);
        const Bitboard occupied = b.color_pieces(Color::White) | b.color_pieces(Color::Black);
        Bitboard rooks = b.pieces[static_cast<int>(rook)];

        while (rooks != 0)
        {
            const int from = std::countr_zero(rooks);
            Bitboard targets = rook_targets(from, occupied, own);

            while (targets != 0)
            {
                const int to = std::countr_zero(targets);
                moves.push_back(Move{from, to});
                targets &= targets - 1;
            }
            rooks &= rooks - 1;
        }
    }
    inline void add_bishop_moves(const Board &b, Color side, std::vector<Move> &moves)
    {
        Piece bishop = Piece::W_bishop;
        if (side == Color::Black)
        {
            bishop = Piece::B_bishop;
        }
        Bitboard own = b.color_pieces(side);
        const Bitboard occupied = b.color_pieces(Color::White) | b.color_pieces(Color::Black);
        Bitboard bishops = b.pieces[static_cast<int>(bishop)];

        while (bishops != 0)
        {
            const int from = std::countr_zero(bishops);
            Bitboard targets = bishop_targets(from, occupied, own);

            while (targets != 0)
            {
                const int to = std::countr_zero(targets);
                moves.push_back(Move{from, to});
                targets &= targets - 1;
            }
            bishops &= bishops - 1;
        }
    }
    inline void add_queen_moves(const Board &b, Color side, std::vector<Move> &moves)
    {
        Piece queen = Piece::W_queen;
        if (side == Color::Black)
        {
            queen = Piece::B_queen;
        }
        Bitboard own = b.color_pieces(side);
        const Bitboard occupied = b.color_pieces(Color::White) | b.color_pieces(Color::Black);
        Bitboard queens = b.pieces[static_cast<int>(queen)];

        while (queens != 0)
        {
            const int from = std::countr_zero(queens);
            Bitboard targets = queen_targets(from, occupied, own);

            while (targets != 0)
            {
                const int to = std::countr_zero(targets);
                moves.push_back(Move{from, to});
                targets &= targets - 1;
            }
            queens &= queens - 1;
        }
    }

    inline void add_pawn_targets(Bitboard targets, int offset, MoveFlag flag, std::vector<Move> &moves)
    {
        while (targets != 0)
        {
            const int to = std::countr_zero(targets);
            moves.push_back(Move{to - offset, to, Piece::No_piece, flag});
            targets &= targets - 1;
        }
    }

    inline void add_pawn_moves(const Board &b, Color side, std::vector<Move> &moves)
    {
        const Bitboard white = b.color_pieces(Color::White);
        const Bitboard black = b.color_pieces(Color::Black);
        const Bitboard empty = ~(white | black);
        if (side == Color::White)
        {
            const Bitboard pawns = b.pieces[static_cast<int>(Piece::W_pawn)];
            add_pawn_targets(white_pawn_pushes(pawns, empty), 8, MoveFlag::Normal, moves);
            add_pawn_targets(white_pawn_double_pushes(pawns, empty), 16, MoveFlag::DoublePush, moves);
            add_pawn_targets(white_pawn_attacks_right(pawns) & black, 9, MoveFlag::Normal, moves);
            add_pawn_targets(white_pawn_attacks_left(pawns) & black, 7, MoveFlag::Normal, moves);
        }
        else
        {
            const Bitboard pawns = b.pieces[static_cast<int>(Piece::B_pawn)];
            // black moves DOWN, so to - from is negative: -8, -16, -9 (left), -7 (right)
            add_pawn_targets(black_pawn_pushes(pawns, empty), -8, MoveFlag::Normal, moves);
            add_pawn_targets(black_pawn_double_pushes(pawns, empty), -16, MoveFlag::DoublePush, moves);
            add_pawn_targets(black_pawn_attacks_left(pawns) & white, -9, MoveFlag::Normal, moves);
            add_pawn_targets(black_pawn_attacks_right(pawns) & white, -7, MoveFlag::Normal, moves);
        }
    }
    inline std::vector<Move> generate_moves(const Board &b)
    {
        std::vector<Move> moves;
        const Color side = b.side_to_move;
        add_pawn_moves(b, side, moves);
        add_knight_moves(b, side, moves);
        add_queen_moves(b, side, moves);
        add_bishop_moves(b, side, moves);
        add_rook_moves(b, side, moves);
        add_king_moves(b, side, moves);

        return moves;
    }
}
