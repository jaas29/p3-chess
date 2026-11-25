#pragma once
#include "chess/board.h"
#include "chess/move.h"

namespace chess
{
    struct Undo
    {
        Piece captured = Piece::No_piece;
        int ep_square = -1;
        int castling = 0;
    };

    inline Undo make_move(Board &b, const Move &m)
    {
        Undo u;
        u.ep_square = b.ep_square;
        u.castling = b.castling;

        const Piece moving = b.piece_at(m.from);
        int cap_sq = m.to;

        if (m.flag == MoveFlag::EnPassant)
        {
            if (b.side_to_move == Color::White)
            {
                cap_sq = m.to - 8;
            }
            else
            {
                cap_sq = m.to + 8;
            }
        }
        u.captured = b.piece_at(cap_sq);
        if (u.captured != Piece::No_piece)
        {
            b.remove(u.captured, cap_sq);
        }
        b.remove(moving, m.from);
        b.place(moving, m.to);

        // en passant lasts one move, unless this move is a double push
        b.ep_square = -1;
        if (m.flag == MoveFlag::DoublePush)
        {
            b.ep_square = (m.from + m.to) / 2; // the jumped-over square
        }

        if (b.side_to_move == Color::White)
        {
            b.side_to_move = Color::Black;
        }
        else
        {
            b.side_to_move = Color::White;
        }
        return u;
    }
    inline void unmake_move(Board &b, const Move &m, const Undo &u)
    {
        if (b.side_to_move == Color::White)
        {
            b.side_to_move = Color::Black;
        }
        else
        {
            b.side_to_move = Color::White;
        }

        b.ep_square = u.ep_square;
        b.castling = u.castling;

        const Piece moving = b.piece_at(m.to);
        b.remove(moving, m.to);
        b.place(moving, m.from);

        // same as make: the side is flipped back, so side_to_move is the mover again
        int cap_sq = m.to;
        if (m.flag == MoveFlag::EnPassant)
        {
            if (b.side_to_move == Color::White)
            {
                cap_sq = m.to - 8;
            }
            else
            {
                cap_sq = m.to + 8;
            }
        }

        if (u.captured != Piece::No_piece)
        {
            b.place(u.captured, cap_sq);
        }
    }

}