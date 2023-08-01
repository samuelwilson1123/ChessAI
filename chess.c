#include <stdio.h>
#include <stdlib.h>

#include "util.h"
#include "moves.h"
#include "execute_moves.h"

// basic functions
void print_bitboard(U64 bitboard)
{

    for (int rank = 7; rank >= 0; rank--)
    {
        printf("%d ", rank + 1);

        for (int file = 0; file < 8; file++)
        {
            
            int square = rank * 8 + file;

            if (get_bit(bitboard, square))
            {
                printf(" %d ", 1);
            }
            else
            {
                printf(" . ");
            }
            //printf(" %d ", get_bit(bitboard, square) ? 1 : 0);
            //printf(" %d ", square);
        }
        printf("\n");
    }

    printf("   a  b  c  d  e  f  g  h\n");

}

void print_board_full(struct Board *board)
{
    for (int rank = 7; rank >= 0; rank--)
    {
        printf("%d ", rank + 1);

        for (int file = 0; file < 8; file++)
        {

            int square = rank * 8 + file;
            
            if (get_bit(board->pieces, square) == 0)
            {
                printf(" . ");
            }
            else if (get_bit(board->w_pawn, square))
            {
                printf(" P ");
            }
            else if (get_bit(board->w_knight, square))
            {
                printf(" N ");
            }
            else if (get_bit(board->w_bishop, square))
            {
                printf(" B ");
            }
            else if (get_bit(board->w_rook, square))
            {
                printf(" R ");
            }
            else if (get_bit(board->w_queen, square))
            {
                printf(" Q ");
            }
            else if (get_bit(board->w_king, square))
            {
                printf(" K ");
            }
            else if (get_bit(board->b_pawn, square))
            {
                printf(" p ");
            }
            else if (get_bit(board->b_knight, square))
            {
                printf(" n ");
            }
            else if (get_bit(board->b_bishop, square))
            {
                printf(" b ");
            }
            else if (get_bit(board->b_rook, square))
            {
                printf(" r ");
            }
            else if (get_bit(board->b_queen, square))
            {
                printf(" q ");
            }
            else if (get_bit(board->b_king, square))
            {
                printf(" k ");
            }
            else 
            {
                printf(" e ");
            }

        }
        printf("\n");
    }

    printf("   a  b  c  d  e  f  g  h\n\n");
}

// TODO: compress w turn and b turn into seperate method, add end game checks
// TODO: implement promotion for both sides
// CLEAN THIS UP
// main game loop
void loop(struct Board * board)
{
    int turn = 1;

    printf("Chess!\nEnter q to quit\n");
    printf("Move entry format example: e2e4, moves piece from e2 to e4\n\n");
    while (1)
    {

        int w_move = -1;
        int b_move = -1;

        printf("Turn %d:\n", turn);

        while (w_move < 0) // WHITE TURN
        {

            char input[10];
            int start_square;
            int end_square;
            int piece;

            print_board_full(board);

            printf("Enter white move: ");
            scanf("%s", input);

            if (input[0] == 'q')
            {
                printf("Thank you for playing\n");
                return;
            }
            
            string_to_squares(input, &start_square, &end_square);

            piece = get_piece(start_square, *board);
            if (piece < 0)
            {
                printf("Failure finding given piece\n");
            }
            else 
            {
                if (!(execute_move(start_square, end_square, WHITE, piece, board) < 0))
                {
                    w_move++;
                }
            }

        }

        while (b_move < 0) // BLACK TURN
        {
            
            char input[10];
            int start_square;
            int end_square;
            int piece;

            print_board_full(board);

            printf("Enter black move: ");
            scanf("%s", input);

            if (input[0] == 'q')
            {
                printf("Thank you for playing\n");
                return;
            }
            
            string_to_squares(input, &start_square, &end_square);

            piece = get_piece(start_square, *board);
            if (piece < 0)
            {
                printf("Failure finding given piece\n");
            }
            else 
            {
                if (!(execute_move(start_square, end_square, BLACK, piece, board) < 0))
                {
                    b_move++;
                }
            }

        }

        turn++;

    }

}

void init(struct Board *board)
{
    
    board->w_pawn = 0xff00ULL;
    board->w_knight = 0x42ULL;
    board->w_bishop = 0x24ULL;
    //board->w_bishop = 0x0ULL;
    board->w_rook = 0x81ULL;
    board->w_queen = 0x8ULL;
    board->w_king = 0x10ULL;

    board->w_castle = 0x91ULL;

    board->w_pieces = board->w_pawn | board->w_knight | board->w_bishop 
    | board->w_rook | board->w_queen | board->w_king;

    board->b_pawn = 0xff000000000000ULL;
    board->b_knight = 0x4200000000000000ULL;
    board->b_bishop = 0x2400000000000000ULL;
    board->b_rook = 0x8100000000000000ULL;
    board->b_queen = 0x800000000000000ULL;
    board->b_king = 0x1000000000000000ULL;

    board->b_castle = 0x9100000000000000ULL;

    board->b_pieces = board->b_pawn | board->b_knight | board->b_bishop 
    | board->b_rook | board->b_queen | board->b_king;

    board->pieces = board->w_pieces | board->b_pieces;
}

// main
int main()
{
    
    struct Board board;
    init(&board);

    loop(&board);

    return 0;
}