// HOW TO READ THIS FILE
// assert(something) means: "this must be true". If it is false, the program stops on that
// line and prints its number. If we reach the end, every check was true.

#include <cassert>  // gives us assert
#include <iostream> // gives us std::cout

#include "chess/board.h" // the Board we are testing

using namespace chess; // so we can write Board instead of chess::Board

int main()
{

    // Make a new board. The {} in Board sets all 12 sheets to 0, so it starts empty.
    Board b;

    // On an empty board, e4 (28) has nothing on it.
    assert(b.piece_at(28) == Piece::No_piece);

    // Put a white knight on g1 (square 6).
    b.place(Piece::W_knight, 6);

    // Now asking "what is on g1?" must answer "a white knight".
    assert(b.piece_at(6) == Piece::W_knight);

    // b1 (1) is still empty: placing on g1 did not touch any other square.
    assert(b.piece_at(1) == Piece::No_piece);

    // Put a black king on e8 (square 60).
    b.place(Piece::B_king, 60);

    // Asking "what is on e8?" must answer "a black king".
    assert(b.piece_at(60) == Piece::B_king);

    // Draw the board on the screen: you should see k on e8 and N on g1.
    b.print();

    // THE START POSITION, LOADED FROM A FEN

    // Read the standard starting position from its FEN text into a new board s.
    const Board s = Board::from_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    // a1 (0), the bottom-left corner: a white rook.
    assert(s.piece_at(0) == Piece::W_rook);

    // e1 (4): the white king.
    assert(s.piece_at(4) == Piece::W_king);

    // e2 (12): a white pawn.
    assert(s.piece_at(12) == Piece::W_pawn);

    // e4 (28), the middle of the board: empty at the start.
    assert(s.piece_at(28) == Piece::No_piece);

    // d8 (59): the black queen.
    assert(s.piece_at(59) == Piece::B_queen);

    // e8 (60): the black king. This is the check the =+ bug broke.
    assert(s.piece_at(60) == Piece::B_king);

    // h8 (63), the top-right corner and the very last bit: a black rook.
    assert(s.piece_at(63) == Piece::B_rook);

    // Draw the start position on the screen.
    s.print();

    // We only get here if every assert above was true.
    std::cout << "board_test: all passed\n";
}
