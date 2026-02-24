#include "knight_tour.h" /* our api  */
#include "bit_array.h"   /* bitarray */

typedef struct move_delta
{
    signed char dx;
    signed char dy;
} move_delta_t;

static const move_delta_t knight_moves[] = {
    {1, 2}, {2, 1}, {2, -1}, {1, -2}, {-1, -2}, {-2, -1}, {-2, 1}, {-1, 2}};

typedef struct square_moves
{
    unsigned char dst_squares[NUM_MOVES];
    unsigned char num_legal_squares;
} square_moves_t;

static void InitBoard()
{
    bitarr_t board = {0};

    return;
}

knight_status_t KnightTour(unsigned char position_xy,
                           unsigned char result_path[BOARD_SIZE],
                           unsigned int time_out_sec, int use_heuristic)
{
    InitBoard();

    return SUCCESS;
}
