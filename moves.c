#include "moves.h"

// relevent occupancy bits (how many squares are relevant to check for blockers)
int relevent_bits_rook[64] = {
    12, 11, 11, 11, 11, 11, 11, 12,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    12, 11, 11, 11, 11, 11, 11, 12
};
int relevent_bits_bishop[64] = {
    6, 5, 5, 5, 5, 5, 5, 6,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    6, 5, 5, 5, 5, 5, 5, 6
};

/*

    PAWNS

*/

// print out 2d array to hold pawn moves
void print_pawn_moves()
{

    printf("U64 PAWN_MOVES[2][64] = {\n");
    for (int side = 0; side < 2; side++)
    {
        printf("\t{\n");
        for (int square = 0; square < 64; square++)
        {
            printf("\t\t%lluULL", generate_pawn_moves(square, side));
            if (square != 63)
            {
                printf(",");
            }
            printf("\n");
        }
        printf("\t},\n");
    }
    printf("};\n");

}

// print out 2d array to hold pawn attacks
void print_pawn_attacks()
{

    printf("U64 PAWN_ATTACKS[2][64] = {\n");
    for (int side = 0; side < 2; side++)
    {
        printf("\t{\n");
        for (int square = 0; square < 64; square++)
        {
            printf("\t\t%lluULL", generate_pawn_attacks(square, side));
            if (square != 63)
            {
                printf(",");
            }
            printf("\n");
        }
        printf("\t},\n");
    }
    printf("};\n");

}

// gets pawn moves, not including attacks
U64 generate_pawn_moves(int square, int side)
{

    U64 piece = 1ULL << square;
    if (side == WHITE)
    {
        if ((piece & RANK_2) != 0ULL)
        {
            piece = shift_n(piece);
            piece |= shift_n(piece);
        }
        else 
        {
            piece = shift_n(piece);
        }
    }
    else
    {
        if (piece & RANK_7)
        {
            piece = shift_s(piece);
            piece |= shift_s(piece);
        }
        else
        {
            piece = shift_s(piece);
        }
    }

    return piece;
}

// gets pawn attacks
U64 generate_pawn_attacks(int square, int side)
{
    U64 attacks = 1ULL << square;

    if (side == WHITE)
    {
        attacks = shift_ne(attacks) | shift_nw(attacks);
    }
    else
    {
        attacks = shift_se(attacks) | shift_sw(attacks);
    }

    return attacks;
}

/*

    LEAPING PIECES

*/


// Returns all possible moves for a knight in a given square
U64 knight_moves(int square)
{
    U64 piece = 1ULL << square;
    
    U64 attacks = 
    (
        shift_nne(piece)
        | shift_nnw(piece)
        | shift_nee(piece)
        | shift_nww(piece)
        | shift_sse(piece)
        | shift_ssw(piece)
        | shift_see(piece)
        | shift_sww(piece)
    );

    return attacks;
}

// Returns all possible moves for a king in a given square
U64 king_moves(int square)
{
    
    U64 piece = 1ULL << square;

    U64 attacks = 
    (
        shift_n(piece)
        | shift_ne(piece)
        | shift_e(piece)
        | shift_se(piece)
        | shift_s(piece)
        | shift_sw(piece)
        | shift_w(piece)
        | shift_nw(piece)
    );

    return attacks;

}

/*

    SLIDING PIECES

*/

U64 queen_moves(int square, U64 blockers)
{
    return (bishop_moves(square, blockers) | rook_moves(square, blockers));
}

U64 bishop_moves(int square, U64 blockers)
{
    blockers &= get_bishop_masks(square);
    blockers *= get_bishop_magics(square);
    blockers >>= 64 - relevent_bits_bishop[square];
    return get_bishop_attacks(square, blockers);
}

U64 rook_moves(int square, U64 blockers)
{
    // getting blockers from array
    blockers &= get_rook_masks(square);
    blockers *= get_rook_magics(square);
    blockers >>= 64 - relevent_bits_rook[square];
    U64 attacks = get_rook_attacks(square, blockers);

    return attacks;
}

// range to check for blockers in bishop attacks from certain square
U64 bishop_range(int square)
{

    U64 attacks = 0ULL;

    int rank, file;

    // northeast
    rank = (square / 8) + 1;
    file = (square % 8) + 1;
    while (rank < 7 && file < 7)
    {
        attacks |= (1ULL << (rank * 8 + file));
        rank++;
        file++;
    }

    // southeast
    rank = (square / 8) - 1;
    file = (square % 8) + 1;
    while (rank > 0 && file < 7)
    {
        attacks |= (1ULL << (rank * 8 + file));
        rank--;
        file++;
    }


    // southwest
    rank = (square / 8) - 1;
    file = (square % 8) - 1;
    while (rank > 0 && file > 0)
    {
        attacks |= (1ULL << (rank * 8 + file));
        rank--;
        file--;
    }


    // northwest
    rank = (square / 8) + 1;
    file = (square % 8) - 1;
    while (rank < 7 && file > 0)
    {
        attacks |= (1ULL << (rank * 8 + file));
        rank++;
        file--;
    }

    return attacks;
}

// range to check for blockers in rook attacks from certain square
U64 rook_range(int square)
{
    U64 attacks = 0ULL;

    int rank, file;

    // north
    rank = (square / 8) + 1;
    file = square % 8;
    while (rank < 7)
    {
        attacks |= (1ULL << (rank * 8 + file));
        rank++;
    }

    // east
    rank = square / 8;
    file = (square % 8) + 1;
    while (file < 7)
    {
        attacks |= (1ULL << (rank * 8 + file));
        file++;
    }

    // south
    rank = (square / 8) - 1;
    file = square % 8;
    while (rank > 0)
    {
        attacks |= (1ULL << (rank * 8 + file));
        rank--;
    }

    // west
    rank = square / 8;
    file = (square % 8) - 1;
    while (file > 0)
    {
        attacks |= (1ULL << (rank * 8 + file));
        file--;
    }

    return attacks;

}

// creating the lookup table for sliding pieces masks
void print_slider_ranges()
{
    printf("const U64 BISHOP_MASKS[64] = {\n");
    for (int i = 0; i < 64; i++)
    {
        printf("\t%lluULL", bishop_range(i));
        if (i != 63)
        {
            printf(",");
        }
        printf("\n");
    }
    printf("};\n");

    printf("const U64 ROOK_MASKS[64] = {\n");
    for (int i = 0; i < 64; i++)
    {
        printf("\t%lluULL", rook_range(i));
        if (i != 63)
        {
            printf(",");
        }
        printf("\n");
    }
    printf("};\n");

}

/*
    generating tables for every possible sliding attack with any combination of blockers
*/
void generate_sliding_attacks(int piece)
{

    U64 BISHOP_ATTACKS[64][4096];
    U64 ROOK_ATTACKS[64][4096];

    // looping through every square
    for (int i = 0; i < 64; i++)
    {

        // getting current mask
        U64 mask;
        if (piece == BISHOP)
        {
            mask = get_bishop_masks(i);
        }
        else
        {
            mask = get_rook_masks(i);
        }

        int bit_amt = count_bits(mask);

        // amt of different possible blocker permutations
        int permutations = 1 << bit_amt;

        // looping through every blocker permutation
        for (int j = 0; j < permutations; j++)
        {
            
            // getting current permutation of blockers
            U64 blockers = 0ULL;
            U64 temp_mask = mask;
            for (int k = 0; k < bit_amt; k++)
            {
                int square = count_bits((temp_mask & -temp_mask) - 1); // getting least significant bit
                clear_bit(temp_mask, square); // removing blocker from mask
                if (j & (1 << k)) // if blocker is on the board
                    blockers |= (1ULL << square);
            }
        
            // getting index and finding attack mask
            if (piece == BISHOP)
            {
                U64 magic_index = blockers * get_bishop_magics(i) >> (64 - relevent_bits_bishop[i]);
                BISHOP_ATTACKS[i][magic_index] = generate_bishop_attacks(i, blockers);
            }
            else
            {
                U64 magic_index = blockers * get_rook_magics(i) >> (64 - relevent_bits_rook[i]);
                ROOK_ATTACKS[i][magic_index] = generate_rook_attacks(i, blockers);
            }
        }
    }

    // printing attack tables
    if (piece == BISHOP)
    {
        print_sliding_attacks(BISHOP_ATTACKS, BISHOP);
    }
    else 
    {
        print_sliding_attacks(ROOK_ATTACKS, ROOK);
    }

}

/*
    printing sliding attacks
*/
void print_sliding_attacks(U64 table[64][4096], int piece)
{
    
    int permutations;

    // setting amount of loops for permutation
    if (piece == BISHOP)
    {
        permutations = 512;
        printf("U64 BISHOP_ATTACKS[64][%d] = {\n", permutations);
    }
    else
    {
        permutations = 4096;
        printf("U64 ROOK_ATTACKS[64][%d] = {\n", permutations);
    }

    for (int i = 0; i < 64; i++)
    {
        printf("\t{\n");
        for (int j = 0; j < permutations; j++)
        {
            printf("\t\t%lluULL", table[i][j]);
            if (j < permutations)
            {
                printf(",");
            }
            printf("\n");
        }
        printf("\t},\n");
    }

    printf("};");

}

/*
    generating mask for bishop attacks considering blockers
*/
U64 generate_bishop_attacks(int square, U64 blockers)
{

    U64 attacks = 0ULL;

    int rank, file;

    // northeast
    rank = (square / 8) + 1;
    file = (square % 8) + 1;
    while (rank < 7 && file < 7)
    {
        attacks |= (1ULL << (rank * 8 + file));
        if (blockers & (1ULL << (rank * 8 + file))) 
            break;
        rank++;
        file++;
    }

    // southeast
    rank = (square / 8) - 1;
    file = (square % 8) + 1;
    while (rank > 0 && file < 7)
    {
        attacks |= (1ULL << (rank * 8 + file));
        if (blockers & (1ULL << (rank * 8 + file)))
            break;
        rank--;
        file++;
    }

    // southwest
    rank = (square / 8) - 1;
    file = (square % 8) - 1;
    while (rank > 0 && file > 0)
    {
        attacks |= (1ULL << (rank * 8 + file));
        if (blockers & (1ULL << (rank * 8 + file)))
            break;
        rank--;
        file--;
    }

    // northwest
    rank = (square / 8) + 1;
    file = (square % 8) - 1;
    while (rank < 7 && file > 0)
    {
        attacks |= (1ULL << (rank * 8 + file));
        if (blockers & (1ULL << (rank * 8 + file)))
            break;
        rank++;
        file--;
    }

    return attacks;
}

/*
    generating mask for rook attacks considering blockers
*/
U64 generate_rook_attacks(int square, U64 blockers)
{

    U64 attacks = 0ULL;

    int rank, file;

    // north
    rank = (square / 8) + 1;
    file = square % 8;
    while (rank < 7)
    {
        attacks |= (1ULL << (rank * 8 + file));
        if (blockers & (1ULL << (rank * 8 + file)))
            break;
        rank++;
    }

    // east
    rank = square / 8;
    file = (square % 8) + 1;
    while (file < 7)
    {
        attacks |= (1ULL << (rank * 8 + file));
        if (blockers & (1ULL << (rank * 8 + file)))
            break;
        file++;
    }

    // south
    rank = (square / 8) - 1;    
    file = square % 8;
    while (rank > 0)
    {
        attacks |= (1ULL << (rank * 8 + file));
        if (blockers & (1ULL << (rank * 8 + file)))
            break;
        rank--;
    }

    // west
    rank = square / 8;
    file = (square % 8) - 1;
    while (file > 0)
    {
        attacks |= (1ULL << (rank * 8 + file));
        if (blockers & (1ULL << (rank * 8 + file)))
            break;
        file--;
    }

    return attacks;

}

