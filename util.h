#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>

// defining bitboard type
#define U64 unsigned long long

// defining basic macros
#define get_bit(bb, square) ((bb) & (1ULL << square))
#define set_bit(bb, square) ((bb) |= (1ULL << square))
#define clear_bit(bb, square) (get_bit(bb, square) ? (bb ^= (1ULL << square)) : 0)

// defining shift directions
#define shift_n(bb) ((bb & ~RANK_8) << 8)
#define shift_s(bb) ((bb & ~RANK_1) >> 8)
#define shift_e(bb) ((bb & ~FILE_H) << 1)
#define shift_w(bb) ((bb & ~FILE_A) >> 1)
#define shift_ne(bb) ((bb & ~(FILE_H | RANK_8)) << 9)
#define shift_nw(bb) ((bb & ~(FILE_A | RANK_8)) << 7)
#define shift_se(bb) ((bb & ~(FILE_H | RANK_1)) >> 7)
#define shift_sw(bb) ((bb & ~(FILE_A | RANK_1)) >> 9)

// defining shifts for knights
#define shift_nne(bb) ((bb & ~(FILE_H | RANK_7 | RANK_8)) << 17)
#define shift_nnw(bb) ((bb & ~(FILE_A | RANK_7 | RANK_8)) << 15)
#define shift_nee(bb) ((bb & ~(FILE_G | FILE_H | RANK_8)) << 10)
#define shift_nww(bb) ((bb & ~(FILE_A | FILE_B | RANK_8)) << 6)
#define shift_sse(bb) ((bb & ~(FILE_H | RANK_1 | RANK_2)) >> 15)
#define shift_ssw(bb) ((bb & ~(FILE_A | RANK_1 | RANK_2)) >> 17)
#define shift_see(bb) ((bb & ~(FILE_H | FILE_G | RANK_1)) >> 6)
#define shift_sww(bb) ((bb & ~(FILE_A | FILE_B | RANK_1)) >> 10)

// file definitions
#define FILE_A 0x0101010101010101ULL
#define FILE_B 0x0202020202020202ULL
#define FILE_G 0x4040404040404040ULL
#define FILE_H 0x8080808080808080ULL

// rank definitions
#define RANK_1 0xffULL
#define RANK_2 0xff00ULL
#define RANK_7 0xff000000000000ULL
#define RANK_8 0xff00000000000000ULL

// definitions for easily checking castling stuff
#define check_white_short 0x90ULL
#define check_white_long 0x11ULL
#define check_black_short 0x9000000000000000ULL
#define check_black_long 0x1100000000000000ULL

#define check_white_short_blockers 0x60ULL
#define check_white_long_blockers 0x0eULL
#define check_black_short_blockers 0x6000000000000000ULL
#define check_black_long_blockers 0x0e00000000000000ULL

// squares
enum {
    a1, b1, c1, d1, e1, f1, g1, h1,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a8, b8, c8, d8, e8, f8, g8, h8
};

// color enum
enum {
    WHITE,
    BLACK
};

// sliding pieces enum
enum {
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING
};

// board definition
struct Board 
{
    U64 w_pawn;
    U64 w_knight;
    U64 w_bishop;
    U64 w_rook;
    U64 w_queen;
    U64 w_king;

    U64 w_castle;

    U64 b_pawn;
    U64 b_knight;
    U64 b_bishop;
    U64 b_rook;
    U64 b_queen;
    U64 b_king;

    U64 b_castle;

    U64 w_pieces;
    U64 b_pieces;
    U64 pieces;

};

int count_bits(U64 bb);
int get_piece(int square, struct Board board);
void string_to_squares(char input[], int * start_square, int * end_square);
int promote(int file, int piece, int side, struct Board *board);
void remove_piece(int square, struct Board *board);

#endif