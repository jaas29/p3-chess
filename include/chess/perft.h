#pragma once
#include <cstdint>
#include "chess/movegen.h"
#include "chess/makemove.h"

namespace chess
{
    inline std::uint64_t perft(Board &b, int depth)
    {
        // base case: no moves left to make, we are standing in one position
        if (depth == 0)
        {
            return 1;
        }

        std::uint64_t nodes = 0;
        for (const Move &m : generate_legal_moves(b))
        {
            Undo u = make_move(b, m);
            nodes += perft(b, depth - 1); // count everything below this move
            unmake_move(b, m, u);
        }
        return nodes;
    }
}
