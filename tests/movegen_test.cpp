#include <cassert>
#include <iostream>

#include "chess/movegen.h"

using namespace chess;

int main()
{
    const Board s = Board::from_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    // white knights: b1 -> a3 c3, g1 -> f3 h3
    std::vector<Move> moves;
    add_knight_moves(s, Color::White, moves);
    assert(moves.size() == 4);
    assert(moves[0].from == 1);
    assert(moves[0].to == 16);

    // black knights: b8 -> a6 first
    std::vector<Move> black_knights;
    add_knight_moves(s, Color::Black, black_knights);
    assert(black_knights.size() == 4);
    assert(black_knights[0].from == 57);
    assert(black_knights[0].to == 40);

    // start position: king, rook, bishop, queen are boxed in
    std::vector<Move> start_king;
    add_king_moves(s, Color::White, start_king);
    assert(start_king.size() == 0);

    std::vector<Move> start_rook;
    add_rook_moves(s, Color::White, start_rook);
    assert(start_rook.size() == 0);

    std::vector<Move> start_bishop;
    add_bishop_moves(s, Color::White, start_bishop);
    assert(start_bishop.size() == 0);

    std::vector<Move> start_queen;
    add_queen_moves(s, Color::White, start_queen);
    assert(start_queen.size() == 0);

    // open board: piece on d4, king on h1 (out of the way)

    // king h1: g1 g2 h2
    const Board rook_d4 = Board::from_fen("8/8/8/8/3R4/8/8/7K w - - 0 1");
    std::vector<Move> king_h1;
    add_king_moves(rook_d4, Color::White, king_h1);
    assert(king_h1.size() == 3);

    // rook d4: 7 + 7
    std::vector<Move> open_rook;
    add_rook_moves(rook_d4, Color::White, open_rook);
    assert(open_rook.size() == 14);

    // bishop d4: 13
    const Board bishop_d4 = Board::from_fen("8/8/8/8/3B4/8/8/7K w - - 0 1");
    std::vector<Move> open_bishop;
    add_bishop_moves(bishop_d4, Color::White, open_bishop);
    assert(open_bishop.size() == 13);

    // queen d4: 14 + 13
    const Board queen_d4 = Board::from_fen("8/8/8/8/3Q4/8/8/7K w - - 0 1");
    std::vector<Move> open_queen;
    add_queen_moves(queen_d4, Color::White, open_queen);
    assert(open_queen.size() == 27);

    // rook d4, black pawn d5: up stops at d5 (capture), 1 + 3 + 3 + 4
    const Board rook_blocked = Board::from_fen("8/8/8/3p4/3R4/8/8/7K w - - 0 1");
    std::vector<Move> blocked_rook;
    add_rook_moves(rook_blocked, Color::White, blocked_rook);
    assert(blocked_rook.size() == 11);

    // start pawns: 8 single + 8 double, each side
    std::vector<Move> white_pawns;
    add_pawn_moves(s, Color::White, white_pawns);
    assert(white_pawns.size() == 16);

    std::vector<Move> black_pawns;
    add_pawn_moves(s, Color::Black, black_pawns);
    assert(black_pawns.size() == 16);

    // pawn e4, black pawns d5 f5, e5 blocked: captures only
    const Board caps = Board::from_fen("8/8/8/3ppp2/4P3/8/8/7K w - - 0 1");
    std::vector<Move> cap_moves;
    add_pawn_moves(caps, Color::White, cap_moves);
    assert(cap_moves.size() == 2);
    // perft(1) from the start: 16 pawn + 4 knight = 20
    assert(generate_moves(s).size() == 20);

    // same position, black to move: also 20
    const Board s_black = Board::from_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1");
    assert(generate_moves(s_black).size() == 20);

    // knight on f3 attacks e5; nothing white attacks e6
    Board k = Board::from_fen("4k3/8/8/8/8/5N2/8/4K3 w - - 0 1");
    assert(is_square_attacked(k, 36, Color::White));  // e5 = 4*8+4
    assert(!is_square_attacked(k, 44, Color::White)); // e6

    // rook on a1 attacks a8, unless something stands in between
    Board r = Board::from_fen("4k3/8/8/8/8/8/8/R3K3 w - - 0 1");
    assert(is_square_attacked(r, 56, Color::White)); // a8
    Board rb = Board::from_fen("4k3/8/8/8/P7/8/8/R3K3 w - - 0 1");
    assert(!is_square_attacked(rb, 56, Color::White)); // a8, the pawn on a4 blocks

    // black pawn on d5 attacks e4, not d4
    Board p = Board::from_fen("4k3/8/8/3p4/8/8/8/4K3 w - - 0 1");
    assert(is_square_attacked(p, 28, Color::Black));  // e4
    assert(!is_square_attacked(p, 27, Color::Black)); // d4: pawns don't capture straight
    std::cout << "movegen_test: all passed\n";
}
