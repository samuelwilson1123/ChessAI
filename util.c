#include "util.h"

// getting amt of bits in a bitboard (Brian Kernighan's way)
int count_bits(U64 bb)
{
    int count = 0;

    // popping bits until bb empty
    while(bb)
    {
        count++;

        bb &= bb - 1;
    }

    return count;
}

// TODO: add error handling, so that the inputs must be in specified format
// changes an input format such as e2e4 to just 2 numbers signifying the square.
void string_to_squares(char input[], int * start_square, int * end_square)
{

    int file1 = input[0] - 'a';
    int rank1 = input[1] - '1';
    int file2 = input[2] - 'a';
    int rank2 = input[3] - '1';

    *start_square = (rank1 * 8) + file1;
    *end_square = (rank2 * 8) + file2;
    
    return;

}

// gets piece from a given square
int get_piece(int square, struct Board board)
{
    U64 bb_square = 1ULL << square;

    if (bb_square & (board.w_pawn | board.b_pawn))
    {
        return PAWN;
    }
    else if (bb_square & (board.w_knight | board.b_knight))
    {
        return KNIGHT;
    }
    else if (bb_square & (board.w_bishop | board.b_bishop))
    {
        return BISHOP;
    }
    else if (bb_square & (board.w_rook | board.b_rook))
    {
        return ROOK;
    }
    else if (bb_square & (board.w_queen | board.b_queen))
    {
        return QUEEN;
    }
    else if (bb_square & (board.w_king | board.b_king))
    {
        return KING;
    }
    else 
    {
        return -1;
    }
    
}

// TODO: test this code
// changes board to promote pawn on end of board to desired piece
int promote(int file, int piece, int side, struct Board *board)
{

    U64 bb_square = 1ULL;
    int rank;
 
    // checking if file is in valid range
    if (file > 7 || file < 0)
    {
        return -1;
    }

    // setting rank to check based on color
    if (side == WHITE)
    {
        rank = 7;
    }
    else
    {
        rank = 0;
    }

    bb_square <<= rank * 8 + file;

    // checking if there is a pawn of correct color on tile
    if (side == WHITE && (bb_square & board->w_pawn) == 0)
    {
        return -1;
    }
    else if (side == BLACK && ((bb_square & board->b_pawn) == 0))
    {
        return -1;
    }

    // removing pawn from board
    remove_piece(rank * 8 + file, board);

    // adding desired piece
    if (side == WHITE)
    {
        if (piece == KNIGHT)
        {
            board->w_knight |= bb_square;
        }
        else if (piece == BISHOP)
        {
            board->w_bishop |= bb_square;
        }
        else if (piece == ROOK)
        {
            board->w_rook |= bb_square;
        }
        else if (piece == QUEEN)
        {
            board->w_queen |= bb_square;
        }
        else 
        {
            return -1;
        }

        board->w_pieces |= bb_square;
    }
    else
    {
        if (piece == KNIGHT)
        {
            board->b_knight |= bb_square;
        }
        else if (piece == BISHOP)
        {
            board->b_bishop |= bb_square;
        }
        else if (piece == ROOK)
        {
            board->b_rook |= bb_square;
        }
        else if (piece == QUEEN)
        {
            board->b_queen |= bb_square;
        }
        else 
        {
            return -1;
        }

        board->b_pieces |= bb_square;
    }

    board->pieces = board->b_pieces | board->w_pieces;
    return 0;

}

// TODO: change to int for error handling
// remove piece from board from any given square
void remove_piece(int square, struct Board *board)
{

    U64 bb_square = (1ULL << square);

    // getting piece type
    int piece;
    if ((board->w_pawn | board->b_pawn) & bb_square)
    {
        piece = PAWN;
    }
    else if ((board->w_knight | board->b_knight) & bb_square)
    {
        piece = KNIGHT;
    }
    else if ((board->w_bishop | board->b_bishop) & bb_square)
    {
        piece = BISHOP;
    }
    else if ((board->w_rook | board->b_rook) & bb_square)
    {
        piece = ROOK;
    }
    else if ((board->w_queen | board->b_queen) & bb_square)
    {
        piece = QUEEN;
    }
    else if ((board->w_king | board->b_king) & bb_square)
    {
        piece = KING;
    }
    else
    {
        printf("piece not fount in remove_piece\n");
    }

    // removing piece based on color
    if (bb_square & board->w_pieces)
    {

        switch (piece)
        {
            case PAWN:
                clear_bit(board->w_pawn, square);
                break;
            case KNIGHT:
                clear_bit(board->w_knight, square);
                break;
            case BISHOP:
                clear_bit(board->w_bishop, square);
                break;
            case ROOK:
                clear_bit(board->w_rook, square);
                break;
            case QUEEN:
                clear_bit(board->w_queen, square);
                break;
            case KING:
                clear_bit(board->w_king, square);
                break;
        }

    }
    else if (bb_square & board->b_pieces)
    {

        switch (piece)
        {
            case PAWN:
                clear_bit(board->b_pawn, square);
                break;
            case KNIGHT:   
                clear_bit(board->b_knight, square);
                break; 
            case BISHOP: 
                clear_bit(board->b_bishop, square);
                break; 
            case ROOK: 
                clear_bit(board->b_rook, square);
                break; 
            case QUEEN:
                clear_bit(board->b_queen, square);
                break; 
            case KING: 
                clear_bit(board->b_king, square);
                break; 
        }

    }
    else
    {
        printf("piece not fount in remove_piece\n");       
    }

    // updating bitboards
    board->w_pieces = board->w_pawn | board->w_knight | board->w_bishop 
    | board->w_rook | board->w_queen | board->w_king;

    board->b_pieces = board->b_pawn | board->b_knight | board->b_bishop 
    | board->b_rook | board->b_queen | board->b_king;

    board->pieces = board->w_pieces | board->b_pieces;

}