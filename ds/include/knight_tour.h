#ifndef __KNIGHT_TOUR_H__
#define __KNIGHT_TOUR_H__

#include "bit_array.h" /* bitarray */

#define BOARD_SIZE (64)
#define NUM_MOVES (8)

typedef enum knight_status { SUCCESS = 0, NOT_FOUND, TIMEOUT } knight_status_t;

/**
 * @brief - solves the knights tour problem
 * @param[in] - position_xy - the starting position of the knight.
 * @param[out] - result_path - the result path.
 * @param[in] - time_out_sec -The approximate number of seconds after which the
 * function will return.
 * @param[in] - use_heuristic - 0 no heuristics, 1 use Warnsdorff heuristics.
 * @pre - 0 <= position_xy < 64
 * @returns KNIGHT_STATUS
 */
knight_status_t KnightTour(unsigned char position_xy,
                           unsigned char result_path[BOARD_SIZE],
                           unsigned int time_out_sec, int use_heuristic);

/**
 * @brief - Convert X and Y coordinates to a position index
 * @param[in] - x - the horizontal coordinate (column)
 * @param[in] - y - the vertical coordinate (row)
 * @pre - 0 <= x < 8
 * @pre - 0 <= y < 8
 * @returns position index (>= 0, <64)
 */
unsigned char XYToPosition(unsigned char x, unsigned char y);

/**
 * @brief - Convert position index to X and Y coordinates.
 * @param[in] - position - position index
 * @param[out] - x -  the result horizontal coordinate (column)
 * @param[out] - y -  the result vertical coordinate (row)
 * @pre - x != NULL
 * @pre - y != NULL
 * @returns void
 */
void PositionToXY(unsigned char position, unsigned char *x, unsigned char *y);

/**
 * @brief - checks if a position is valid
 * @param[in] - position_xy - position index
 * @returns 1 if Valid, 0 if invalid
 */
int IsValidPosition(unsigned char position_xy, bitarr_t free_map);

#endif /* __KNIGHT_TOUR_H__ */