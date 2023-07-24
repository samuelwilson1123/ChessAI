#ifndef MOVES_H
#define MOVES_H

#include <stdio.h>
#include "util.h"
#include "tables.h"

/*
    PAWN MOVES
*/
void print_pawn_moves();
void print_pawn_attacks();
U64 generate_pawn_moves(int square, int side);
U64 generate_pawn_attacks(int square, int side);

/*
    LEAPING PIECES
*/
// move generators for creating lookup tables
U64 king_moves(int square);
U64 knight_moves(int square);

/*
    SLIDING PIECES
*/  
// pulls bishop moves from attack table
U64 bishop_moves(int square, U64 blockers);
U64 rook_moves(int square, U64 blockers);

// generating masks for sliding pieces range
U64 queen_moves(int square, U64 blockers);
U64 bishop_range(int square);
U64 rook_range(int square);

// prints ranges of sliding pieces
void print_slider_ranges();

// generates and prints attack tables for sliding pieces
void generate_sliding_attacks(int piece);

// generate masks for sliding pieces considering blockers
U64 generate_bishop_attacks(int square, U64 blockers);
U64 generate_rook_attacks(int square, U64 blockers);

// prints the attack table for a sliding piece
void print_sliding_attacks(U64 table[64][4096], int piece);



#endif