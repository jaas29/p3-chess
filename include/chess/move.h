#pragma once
#include "chess/board.h" //Piece

namespace chess
{
    enum class MoveFlag
    {
        Normal,
        DoublePush,
        EnPassant,
        Castle,
    };

    struct Move
    {
        int from = 0;
        int to = 0;
        Piece promotion = Piece::No_piece;
        MoveFlag flag = MoveFlag::Normal;
    };
}
