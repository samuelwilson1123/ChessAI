#ifndef TABLES_H
#define TABLES_H

#include "util.h"

// getters for pawn moves
U64 get_pawn_moves(int square, int side);
// getters for pawn attacks
U64 get_pawn_attacks(int square, int side);

// getters for leaping piece tables
U64 get_knight_attacks(int square);
U64 get_king_attacks(int square);

// getters for bishop and rook mask tables
U64 get_bishop_masks(int square);
U64 get_rook_masks(int square);

// getters for bishop and rook magic numbers
U64 get_bishop_magics(int square);
U64 get_rook_magics(int square);

// get attack table for bishop
U64 get_bishop_attacks(int square, int magic_index);
U64 get_rook_attacks(int square, int magic_index);



#endif