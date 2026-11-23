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
        u.captured = b.piece_at(m.to);
        u.ep_square = b.ep_square;
        u.castling = b.castling;

        const Piece moving = b.piece_at(m.from);

        if (u.captured != Piece::No_piece)
        {
            b.remove(u.captured, m.to);
        }

        b.remove(moving, m.from);
        b.place(moving, m.to);

        b.ep_square = -1;

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

        if (u.captured != Piece::No_piece)
        {
            b.place(u.captured, m.to);
        }
    }
}