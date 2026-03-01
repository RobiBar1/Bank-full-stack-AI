#include <assert.h> /* assert */
#include <stdio.h>  /* printf */
#include <time.h>   /* time_t */

#include "knight_tour.h" /* our api  */

#define TURN_ON_BIT 1
#define TURN_OFF_BIT 0

#define VALID 1
#define NOT_VALID 0
#define NUM_SQARE_IN_ROW 8
#define NUM_SQARE_IN_COL 8
#define IS_ROW_OR_COL_VALID(x) (0 <= (x) && (NUM_SQARE_IN_ROW) > (x))

typedef struct move_delta
{
    signed char dx;
    signed char dy;
} move_delta_t;

typedef struct square_moves
{
    unsigned char dst_squares[NUM_MOVES];
    unsigned char num_legal_squares;
} square_moves_t;

static const move_delta_t knight_moves[] = {
    {1, 2}, {2, 1}, {2, -1}, {1, -2}, {-1, -2}, {-2, -1}, {-2, 1}, {-1, 2}};

static square_moves_t board_moves[BOARD_SIZE] = {0};

unsigned char XYToPosition(unsigned char x, unsigned char y)
{
    return (x * NUM_SQARE_IN_ROW + y);
}

void PositionToXY(unsigned char position, unsigned char* x, unsigned char* y)
{
    assert(NULL != x);
    assert(NULL != y);

    *x = position / NUM_SQARE_IN_ROW;
    *y = position & (NUM_SQARE_IN_COL - 1);
}

int IsValidPosition(unsigned char position_xy, bitarr_t free_map)
{
    if (BOARD_SIZE <= position_xy ||
        BitArrGetBit(free_map, (size_t)position_xy))
    {
        return NOT_VALID;
    }

    return VALID;
}

static void InitSqare(square_moves_t* sqare, unsigned char position)
{
    unsigned char x = 0;
    unsigned char y = 0;
    int int_x = 0;
    int int_y = 0;
    size_t i = 0;

    assert(NULL != sqare);

    sqare->num_legal_squares = 0;
    PositionToXY(position, &x, &y);
    for (; i < NUM_MOVES; ++i)
    {
        int_x = (int)knight_moves[i].dx + x;
        int_y = (int)knight_moves[i].dy + y;
        if (IS_ROW_OR_COL_VALID(int_x) && IS_ROW_OR_COL_VALID(int_y))
        {
            sqare->dst_squares[sqare->num_legal_squares++] =
                XYToPosition((unsigned char)int_x, (unsigned char)int_y);
        }
    }
}

static void SortMoves(unsigned char position)
{
    size_t i = 0;
    size_t j = 0;
    unsigned char tmp = 0;

    for (i = 0; i < board_moves[position].num_legal_squares; ++i)
    {
        for (j = 0; j < board_moves[position].num_legal_squares - 1 - i; ++j)
        {
            if (board_moves[board_moves[position].dst_squares[j]]
                    .num_legal_squares >
                board_moves[board_moves[position].dst_squares[j + 1]]
                    .num_legal_squares)
            {
                tmp = board_moves[position].dst_squares[j];
                board_moves[position].dst_squares[j] =
                    board_moves[position].dst_squares[j + 1];
                board_moves[position].dst_squares[j + 1] = tmp;
            }
        }
    }
}

static void InitBoard(int use_heuristic)
{
    unsigned char i = 0;

    for (; i < BOARD_SIZE; ++i)
    {
        InitSqare(&(board_moves[i]), i);
    }

    if (use_heuristic)
    {
        for (i = 0; i < BOARD_SIZE; ++i)
        {
            SortMoves(i);
        }
    }
}

static knight_status_t
KnightTourRecursiveStep(unsigned char position,
                        unsigned char result_path[BOARD_SIZE],
                        bitarr_t free_map, size_t step_counter,
                        time_t start_time, unsigned int time_out_sec)
{
    static size_t call_count = 0;
    knight_status_t status = NOT_FOUND;
    size_t i = 0;

    assert(NULL != result_path);

    ++call_count;
    if (BOARD_SIZE == step_counter)
    {
        return SUCCESS;
    }
    if (!IsValidPosition(position, free_map))
    {
        return NOT_FOUND;
    }
    if (0 == (call_count & 0xFFF) && time(NULL) - start_time > time_out_sec)
    {
        return TIMEOUT;
    }

    free_map = BitArrSetBit(free_map, (size_t)position, TURN_ON_BIT);
    for (; i < board_moves[position].num_legal_squares && TIMEOUT != status;
         ++i)
    {
        status = KnightTourRecursiveStep(
            board_moves[position].dst_squares[i], result_path, free_map,
            step_counter + 1, start_time, time_out_sec);
        if (SUCCESS == status)
        {
            result_path[step_counter] = position;

            return status;
        }
    }

    return status;
}

knight_status_t KnightTour(unsigned char position_xy,
                           unsigned char result_path[BOARD_SIZE],
                           unsigned int time_out_sec, int use_heuristic)
{
    size_t step_counter = 0;
    bitarr_t free_places = 0;
    time_t start_time = 0;

    assert(NULL != result_path);

    InitBoard(use_heuristic);
    start_time = time(NULL);

    return (KnightTourRecursiveStep(position_xy, result_path, free_places,
                                    step_counter, start_time, time_out_sec));
}
