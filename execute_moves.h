#ifndef EXECUTE_MOVES_H
#define EXECUTE_MOVES_H

#include <stdio.h>
#include "util.h"
#include "tables.h"
#include "moves.h"

int execute_move(int start_square, int end_square, int side, int piece, struct Board * board);
int execute_pawn_move(int start_square, int end_square, int side, struct Board * board);
int execute_knight_move(int start_square, int end_square, int side, struct Board * board);
int execute_bishop_move(int start_square, int end_square, int side, struct Board * board);
int execute_rook_move(int start_square, int end_square, int side, struct Board * board);
int execute_queen_move(int start_square, int end_square, int side, struct Board * board);
int execute_king_move(int start_square, int end_square, int side, struct Board * board);
int execute_short_castle(int side, struct Board * board);
int execute_long_castle(int side, struct Board * board);

#endif