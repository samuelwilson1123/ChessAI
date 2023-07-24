#include "execute_moves.h"

int execute_move(
    int start_square, int end_square, 
    int side, int piece, struct Board * board)
{
    switch (piece)
    {
    case PAWN:
        if (execute_pawn_move(start_square, end_square, side, board) < 0)
        {
            printf("Error executing pawn move\n");
            return -1;
        }
        else
        {
            break;
        }
    case KNIGHT:
        if (execute_knight_move(start_square, end_square, side, board) < 0)
        {
            printf("Error executing knight move\n");
            return -1;
        }
        else
        {
            break;
        }

    case BISHOP:

        if (execute_bishop_move(start_square, end_square, side, board) < 0)
        {
            printf("Error executing bishop move\n");
            return -1;
        }
        else
        {
            break;
        }

    case ROOK:
        
        if (execute_rook_move(start_square, end_square, side, board) < 0)
        {
            printf("Error executing rook move\n");
            return -1;
        }
        else
        {
            break;
        }

    case QUEEN:

        if (execute_queen_move(start_square, end_square, side, board) < 0)
        {
            printf("Error executing queen move\n");
            return -1;
        }
        else
        {
            break;
        }

    case KING:
        
        if (execute_king_move(start_square, end_square, side, board) < 0)
        {
            printf("Error executing king move\n");
            return -1;
        }
        else
        {
            break;
        }

    default:
        break;
    }

    // updating bitboards
    board->w_pieces = board->w_pawn | board->w_knight | board->w_bishop 
    | board->w_rook | board->w_queen | board->w_king;

    board->b_pieces = board->b_pawn | board->b_knight | board->b_bishop 
    | board->b_rook | board->b_queen | board->b_king;

    board->pieces = board->w_pieces | board->b_pieces;

    return 0;

}

// executes pawn move, TODO: clean this up
// TODO: en passant, promotions
int execute_pawn_move(int start_square, int end_square, int side, struct Board * board)
{

    U64 bb_start = 1ULL << start_square;
    U64 bb_end = 1ULL << end_square;

    U64 pawn_moves = get_pawn_moves(start_square, side);
    U64 pawn_attacks = get_pawn_attacks(start_square, side);

    // checking validity
    if (
        ((side == WHITE) && !(bb_start & board->w_pawn))
        || ((side == BLACK) && !(bb_start & board->b_pawn))
        )
    {
        return -1;
    }

    int valid = 0; // 0 if not valid, 1 if push, 2 if attack
    if (bb_end & pawn_moves) // pawn push
    {

        if (!(board->pieces & pawn_moves))
        {
            valid = 1;
        }
        else if (count_bits(pawn_moves) > 1) // double pawn push
        {
            if ((side == WHITE) && (bb_end == shift_n(start_square))
            && !(bb_end & board->pieces))
            {
                valid = 1;
            }
            else if ((side == BLACK) && (bb_end == shift_s(start_square))
            && !(bb_end & board->pieces))
            {
                valid = 1;
            }

        }

    }
    else if (bb_end & pawn_attacks) // pawn attack
    {
        if ((side == WHITE) && (pawn_attacks & board->b_pieces))
        {
            valid = 2;
        }
        else if ((side == BLACK) && (pawn_attacks & board->w_pieces))
        {
            valid = 2;
        }
    }

    if (valid == 0)
    {
        return -1;
    }
    else if (valid == 1) // executing pawn push 
    {
        
        /*
            Step 1. add move to pawn map
            Step 2. delete old position from pawn map
        */

       if (side == WHITE)
       {
            board->w_pawn |= bb_end; // step 1
            board->w_pawn ^= bb_start; // step 2
       }
       else
       {
            board->b_pawn |= bb_end; // step 1
            board->b_pawn ^= bb_start; // step 2
       }

    }
    else // executing pawn attack 
    {
        /*
            Step 1. remove enemy piece from the appropriate bb
                1a. finding appropriate enemy piece
            Step 2. add pawn move
            Step 3. delete old pawn position
        */

        remove_piece(end_square, board); // step 1
        if (side == WHITE)
        {
            board->w_pawn |= bb_end; // step 2
            board->w_pawn ^= bb_start; // step 3
        }
        else
        {
            board->b_pawn |= bb_end; // step 2
            board->b_pawn ^= bb_start; // step 3
        }

    }

    return 0;
}

// execute knight move
int execute_knight_move(int start_square, int end_square, int side, struct Board * board)
{

    U64 bb_start = 1ULL << start_square;
    U64 bb_end = 1ULL << end_square;

    /*
        step 1. check move for validity
        step 2. removing enemy piece from end square, if one exists
        step 3. add knight move
        step 4. delete current knight possition
    */

   // Step 1.
    if ( 
        ((side == WHITE) && !(bb_start & board->w_knight))
        || ((side == BLACK) && !(bb_start & board->b_knight))
        )
    { // checking if thek knight exists
        return -1;
    }

    if (!(bb_end & get_knight_attacks(start_square)))
    { // checks if the knight is able to move to the end square
        return -1;
    }

    if (
        ((side == WHITE) && (bb_end & board->w_pieces))
        || ((side == BLACK) && (bb_end & board->b_pieces))
        )
    { // checking if the knight is trying to attack an ally
        return -1;
    }

    // Step 2.
    if ((side == WHITE) && (bb_end & board->b_pieces))
    {
        remove_piece(end_square, board);
    }
    else if ((side == BLACK) && (bb_end & board->w_pieces))
    {
        remove_piece(end_square, board);
    }

    if (side == WHITE)
    {
        board->w_knight |= bb_end; // Step 3.
        board->w_knight ^= bb_start; // Step 4.
    }
    else 
    {
        board->b_knight |= bb_end; // Step 3.
        board->b_knight ^= bb_start; // Step 4.    
    }

    return 0;

}

// execute bishop move
int execute_bishop_move(int start_square, int end_square, int side, struct Board * board)
{

    U64 bb_start = 1ULL << start_square;
    U64 bb_end = 1ULL << end_square;

    /*
        Step 1. check basic validity (especially if moving to edge)
        Step 2. remove enemy piece if necessary
        Step 3. add bishop move
        Step 4. remove old bishop
    */

    U64 bb_range = bishop_moves(start_square, board->pieces);

    // Step 1.

    // checking if the bishop exists
    if ( 
        ((side == WHITE) && !(bb_start & board->w_bishop))
        || ((side == BLACK) && !(bb_start & board->b_bishop))
        )
    { 
        return -1;
    }

    // checking for moving to edge of the board because that is not taken into account in bishop moves
    if (bb_end & RANK_1)
    {

        // if the end square is not in range
        if (!(shift_ne(bb_end) & bb_range) && !(shift_nw(bb_end) & bb_range))
        {
            return -1;
        }

        // if there is a blocker in the way
        if (((shift_ne(bb_end) & bb_range) & board->pieces) 
        || ((shift_nw(bb_end) & bb_range) & board->pieces))
        {
            return -1;
        }

    }
    else if (bb_end & RANK_8)
    {

        // if the end square is in range
        if (!(shift_sw(bb_end) & bb_range) && !(shift_se(bb_end) & bb_range))
        {
            return -1;
        }

        // if there is a blocker in the way
        if (((shift_sw(bb_end) & bb_range) & board->pieces) 
        || ((shift_se(bb_end) & bb_range) & board->pieces))
        {
            return -1;
        }
    }
    else if (bb_end & FILE_A)
    {

        // if the end square is in range
        if (!(shift_ne(bb_end) & bb_range) && !(shift_se(bb_end) & bb_range))
        {
            return -1;
        }

        // if there is a blocker in the way
        if (((shift_se(bb_end) & bb_range) & board->pieces) 
        || ((shift_ne(bb_end) & bb_range) & board->pieces))
        {
            return -1;
        }
    }
    else if (bb_end & FILE_H)
    {

        // if the end square is in range
        if (!(shift_nw(bb_end) & bb_range) && !(shift_sw(bb_end) & bb_range))
        {
            return -1;
        }

        // if there is a blocker in the way
        if (((shift_nw(bb_end) & bb_range) & board->pieces) 
        || ((shift_sw(bb_end) & bb_range) & board->pieces))
        {
            return -1;
        }
    }
    else if (!(bb_end & bb_range))
    { 
        return -1;
    }

    // checking if the bishop is trying to attack an ally
    if (
        ((side == WHITE) && (bb_end & board->w_pieces))
        || ((side == BLACK) && (bb_end & board->b_pieces))
        )
    { 
        return -1;
    }

    // Step 2.
    if ((side == WHITE) && (bb_end & board->b_pieces))
    {
        remove_piece(end_square, board);
    }
    else if ((side == BLACK) && (bb_end & board->w_pieces))
    {
        remove_piece(end_square, board);
    }

    if (side == WHITE)
    {
        board->w_bishop |= bb_end; // Step 3.
        board->w_bishop ^= bb_start; // Step 4.
    }
    else 
    {
        board->b_bishop |= bb_end; // Step 3.
        board->b_bishop ^= bb_start; // Step 4.    
    }

    return 0;

}

// execute rook move
int execute_rook_move(int start_square, int end_square, int side, struct Board * board)
{
    U64 bb_start = 1ULL << start_square;
    U64 bb_end = 1ULL << end_square;
    U64 bb_range = rook_moves(start_square, board->pieces);

    /*
        Step 1. check basic validity (especially if moving to edge)
        Step 2. remove enemy piece if necessary
        Step 3. add rook move
        Step 4. remove old rook
    */

    // Step 1.

    // checking if the rook exists
    if ( 
        ((side == WHITE) && !(bb_start & board->w_rook))
        || ((side == BLACK) && !(bb_start & board->b_rook))
        )
    { 
        return -1;
    }

    // checking for moving to edge of the board because that is not taken into account in rook moves
    if (bb_end & RANK_1)
    {

        // if the end square is not in range
        if (!(shift_n(bb_end) & bb_range) && !(bb_end && bb_range))
        {
            return -1;
        }

        // if there is a blocker in the way
        if ((shift_n(bb_end) & bb_range) & board->pieces)
        {
            return -1;
        }

    }
    else if (bb_end & RANK_8)
    {

        // if the end square is not in range
        if (!(shift_s(bb_end) & bb_range) && !(bb_end && bb_range))
        {
            return -1;
        }

        // if there is a blocker in the way
        if ((shift_s(bb_end) & bb_range) & board->pieces)
        {
            return -1;
        }
    }
    else if (bb_end & FILE_A)
    {

        // if the end square is not in range
        if (!(shift_e(bb_end) & bb_range) && !(bb_end && bb_range))
        {
            return -1;
        }

        // if there is a blocker in the way
        if ((shift_e(bb_end) & bb_range) & board->pieces)
        {
            return -1;
        }
    }
    else if (bb_end & FILE_H)
    {

        // if the end square is not in range
        if (!(shift_w(bb_end) & bb_range) && !(bb_end && bb_range))
        {
            return -1;
        }

        // if there is a blocker in the way
        if ((shift_w(bb_end) & bb_range) & board->pieces)
        {
            return -1;
        }
    }
    else if (!(bb_end & bb_range))
    { 
        return -1;
    }

    // checking if the rook is trying to attack an ally
    if (
        ((side == WHITE) && (bb_end & board->w_pieces))
        || ((side == BLACK) && (bb_end & board->b_pieces))
        )
    { 
        return -1;
    }

    // Step 2.
    if ((side == WHITE) && (bb_end & board->b_pieces))
    {
        remove_piece(end_square, board);
    }
    else if ((side == BLACK) && (bb_end & board->w_pieces))
    {
        remove_piece(end_square, board);
    }

    if (side == WHITE)
    {
        board->w_rook |= bb_end; // Step 3.
        board->w_rook ^= bb_start; // Step 4.
    }
    else 
    {
        board->b_rook |= bb_end; // Step 3.
        board->b_rook ^= bb_start; // Step 4.    
    }

    return 0;

}

// executes king move
int execute_king_move(int start_square, int end_square, int side, struct Board * board)
{
    U64 bb_start = 1ULL << start_square;
    U64 bb_end = 1ULL << end_square;

    /*
        Step 1. check validity
        Step 2. removing enemy if they exist
        Step 3. add king move
        Step 4. remove old king
        Step 5. disallow any future castling
    */

   // Step 1.

    // checking for castling
    if (bb_end == (bb_start << 2)) // short castle
    {

        if (side == WHITE)
        {
            return execute_short_castle(WHITE, board);
        }
        else
        {
            return execute_short_castle(BLACK, board);
        }

    }
    else if (bb_end == (bb_start >> 2)) // long castle
    {

        if (side == WHITE)
        {
            return execute_long_castle(WHITE, board);
        }
        else
        {
            return execute_long_castle(BLACK, board);
        }

    }

    if ( 
        ((side == WHITE) && !(bb_start & board->w_king))
        || ((side == BLACK) && !(bb_start & board->b_king))
        )
    { // checking if the king exists
        return -1;
    }

    if (!(bb_end & get_king_attacks(start_square)))
    { // checks if the king is able to move to the end square
        return -1;
    }

    if (
        ((side == WHITE) && (bb_end & board->w_pieces))
        || ((side == BLACK) && (bb_end & board->b_pieces))
        )
    { // checking if the king is trying to attack an ally
        return -1;
    }

    // Step 2.
    if ((side == WHITE) && (bb_end & board->b_pieces))
    {
        remove_piece(end_square, board);
    }
    else if ((side == BLACK) && (bb_end & board->w_pieces))
    {
        remove_piece(end_square, board);
    }

    if (side == WHITE)
    {
        board->w_king |= bb_end; // Step 3.
        board->w_king ^= bb_start; // Step 4.
        board->w_castle = 0ULL; // Step 5.
    }
    else 
    {
        board->b_king |= bb_end; // Step 3.
        board->b_king ^= bb_start; // Step 4.    
        board->b_castle = 0ULL; // Step 5.
    }

    return 0;
}

// executes queen move
int execute_queen_move(int start_square, int end_square, int side, struct Board * board)
{

    U64 bb_start = 1ULL << start_square;
    U64 bb_end = 1ULL << end_square;
    U64 bb_range = (bishop_moves(start_square, board->pieces) | rook_moves(start_square, board->pieces));

    /*
        Step 1. check validity
        Step 2. remove enemy
        Step 3. add queen move
        Step 4. delete old queen
    */

   // checking if queen exists
   if (
        ((side == WHITE) && !(bb_start & board->w_queen))
        || ((side == BLACK) && !(bb_start & board->b_queen))
        )
   {
        return -1;
   }

    // checking for moves to end of board becuase that is not tracked in current range calc
    if (bb_end & RANK_1)
    {

        // if the end square is not in range
        if (
            !(shift_ne(bb_end) & bb_range) && !(shift_nw(bb_end) & bb_range)
            && !(shift_n(bb_end) & bb_range) && !(bb_end && bb_range)
        )
        {
            return -1;
        }

        // if there is a blocker in the way
        if (
            ((shift_ne(bb_end) & bb_range) & board->pieces) 
        || ((shift_nw(bb_end) & bb_range) & board->pieces)
        || ((shift_n(bb_end) & bb_range) & board->pieces)
        )
        {
            return -1;
        }

    }
    else if (bb_end & RANK_8)
    {

        // if the end square is not in range
        if (
            !(shift_se(bb_end) & bb_range) && !(shift_sw(bb_end) & bb_range)
            && !(shift_s(bb_end) & bb_range) && !(bb_end && bb_range)
        )
        {
            return -1;
        }

        // if there is a blocker in the way
        if (
            ((shift_se(bb_end) & bb_range) & board->pieces) 
        || ((shift_sw(bb_end) & bb_range) & board->pieces)
        || ((shift_s(bb_end) & bb_range) & board->pieces)
        )
        {
            return -1;
        }

    }
    else if (bb_end & FILE_A)
    {

        // if the end square is not in range
        if (
            !(shift_ne(bb_end) & bb_range) && !(shift_se(bb_end) & bb_range)
            && !(shift_e(bb_end) & bb_range) && !(bb_end && bb_range)
        )
        {
            return -1;
        }

        // if there is a blocker in the way
        if (
            ((shift_ne(bb_end) & bb_range) & board->pieces) 
        || ((shift_se(bb_end) & bb_range) & board->pieces)
        || ((shift_e(bb_end) & bb_range) & board->pieces)
        )
        {
            return -1;
        }

    }
    else if (bb_end & FILE_H)
    {

        // if the end square is not in range
        if (
            !(shift_nw(bb_end) & bb_range) && !(shift_sw(bb_end) & bb_range)
            && !(shift_w(bb_end) & bb_range) && !(bb_end && bb_range)
        )
        {
            return -1;
        }

        // if there is a blocker in the way
        if (
            ((shift_nw(bb_end) & bb_range) & board->pieces) 
        || ((shift_sw(bb_end) & bb_range) & board->pieces)
        || ((shift_w(bb_end) & bb_range) & board->pieces)
        )
        {
            return -1;
        }

    }
    else if (!(bb_end & bb_range))
    {
        return -1;
    }

    // checking if the bishop is trying to attack an ally
    if (
        ((side == WHITE) && (bb_end & board->w_pieces))
        || ((side == BLACK) && (bb_end & board->b_pieces))
        )
    { 
        return -1;
    }

    // Step 2.
    if ((side == WHITE) && (bb_end & board->b_pieces))
    {
        remove_piece(end_square, board);
    }
    else if ((side == BLACK) && (bb_end & board->w_pieces))
    {
        remove_piece(end_square, board);
    }

    if (side == WHITE)
    {
        board->w_queen |= bb_end; // Step 3.
        board->w_queen ^= bb_start; // Step 4.
    }
    else 
    {
        board->b_queen |= bb_end; // Step 3.
        board->b_queen ^= bb_start; // Step 4.    
    }

    return 0;

}

// TODO: add rule where king cannot pass through check in castle

// executes castle
int execute_short_castle(int side, struct Board * board)
{

    /*
        Step 1. check validity
        Step 2. move rook
        Step 3. remove old rook
        Step 4. slide king
        Step 5. clear castling bb in board
    */

   // Step 1.

    // checking to make neither the rook or king has moved
    if ((side == WHITE) && !((board->w_castle & check_white_short) == check_white_short))
    {
        return -1;
    }
    else if ((side == BLACK) && !((board->b_castle & check_black_short) == check_black_short))
    {
        return -1;
    }

    // checking to see if there are any pieces between the king and rook
    if ((side == WHITE) && (board->pieces & check_white_short_blockers))
    {
        return -1;
    }
    else if ((side == BLACK) && (board->pieces & check_black_short_blockers))
    {
        return -1;
    }

    if (side == WHITE)
    {
        board->w_rook |= (board->w_king << 1); // Step 2
        board->w_rook ^= (board->w_king << 3); // Step 3
        board->w_king <<= 2; // Step 4
        board->w_castle = 0ULL;
    }
    else
    {
        board->b_rook |= (board->b_king << 1); // Step 2
        board->b_rook ^= (board->b_king << 3); // Step 3
        board->b_king <<= 2; // Step 4
        board->b_castle = 0ULL;
    }

    return 0;
}

// executes castle
int execute_long_castle(int side, struct Board * board)
{

   /*
        Step 1. check validity
        Step 2. move rook
        Step 3. remove old rook
        Step 4. slide king
        Step 5. disallow any future castling
    */

   // Step 1.

    // checking to make neither the rook or king has moved
    if ((side == WHITE) && !((board->w_castle & check_white_long) == check_white_long))
    {
        return -1;
    }
    else if ((side == BLACK) && !((board->b_castle & check_black_long) == check_black_long))
    {
        return -1;
    }

    // checking to see if there are any pieces between the king and rook
    if ((side == WHITE) && (board->pieces & check_white_long_blockers))
    {
        return -1;
    }
    else if ((side == BLACK) && (board->pieces & check_black_long_blockers))
    {
        return -1;
    }

    if (side == WHITE)
    {
        board->w_rook |= (board->w_king >> 1); // Step 2
        board->w_rook ^= (board->w_king >> 4); // Step 3
        board->w_king >>= 2; // Step 4
        board->w_castle = 0ULL; // Step 5
    }
    else
    {
        board->b_rook |= (board->b_king >> 1); // Step 2
        board->b_rook ^= (board->b_king >> 4); // Step 3
        board->b_king >>= 2; // Step 4
        board->b_castle = 0ULL; // Step 5
    }

    return 0;
}