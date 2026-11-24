#pragma once
#include "chess/board.h"
#include "chess/attacks.h"
#include "chess/move.h"
#include <vector>
#include <bit>
#include "chess/makemove.h"
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

    inline bool is_square_attacked(const Board &b, int sq, Color by)
    {
        const Bitboard occupied = b.color_pieces(Color::White) | b.color_pieces(Color::Black);

        // the 6 sheets of the attackers: white = 0...5, black = 6..11
        int base = 0;
        if (by == Color::Black)
        {
            base = 6;
        }
        const Bitboard pawns = b.pieces[base + 0];
        const Bitboard knights = b.pieces[base + 1];
        const Bitboard bishops = b.pieces[base + 2];
        const Bitboard rooks = b.pieces[base + 3];
        const Bitboard queens = b.pieces[base + 4];
        const Bitboard king = b.pieces[base + 5];

        // fake knight on sq
        if ((knight_attacks(sq) & knights) != 0)
            return true;
        // fake bishop: bishops AND queens move diagonally
        if ((bishop_attacks(sq, occupied) & (bishops | queens)) != 0)
            return true;
        // fake rook: rooks AND queens move straight
        if ((rook_attacks(sq, occupied) & (rooks | queens)) != 0)
            return true;
        // fake king
        if ((king_attacks(sq) & king) != 0)
            return true;
        // fake pawn: pretend to be the OTHER colour's pawn standinng on sq
        const Bitboard here = square_mask(sq);
        Bitboard pawn_spots = 0;
        if (by == Color::Black)
        {
            pawn_spots = white_pawn_attacks_left(here) | white_pawn_attacks_right(here);
        }
        else
        {
            pawn_spots = black_pawn_attacks_left(here) | black_pawn_attacks_right(here);
        }
        if ((pawn_spots & pawns) != 0)
            return true;

        return false;
    }
    // only the moves that don't leave our own king attacked
    inline std::vector<Move> generate_legal_moves(Board &b)
    {
        std::vector<Move> legal;
        const Color us = b.side_to_move;
        Color them = Color::White;
        if (us == Color::White)
        {
            them = Color::Black;
        }
        // our king's sheet: W_king or B_king
        Piece our_king = Piece::W_king;
        if (us == Color::Black)
        {
            our_king = Piece::B_king;
        }
        for (const Move &m : generate_moves(b))
        {
            Undo u = make_move(b, m);
            const int king_sq = std::countr_zero(b.pieces[static_cast<int>(our_king)]);
            if (!is_square_attacked(b, king_sq, them))
            {
                legal.push_back(m);
            }
            unmake_move(b, m, u);
        }
        return legal;
    }
}
