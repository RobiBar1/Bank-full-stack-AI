#ifndef __BIT_ARRAY_H__
#define __BIT_ARRAY_H__

#include <stddef.h>

typedef size_t bitarr_t;

/** @desc sets all the bits on
 * @param[in,out] bit_arr - value representing a bit array
 * @return the modified bitarr_t
 **/
bitarr_t BitArrSetAllOn(bitarr_t bit_arr);

/** @desc sets all the bits off
 * @param[in,out] bit_arr - value representing a bit array
 * @return the modified bitarr_t
 **/
bitarr_t BitArrSetAllOff(bitarr_t bit_arr);

/** @desc sets or clears a specific bit
 * @param[in,out] bit_arr - value representing a bit array
 * @param[in] index - location of the bit that is going to be set
 * @param[in] boolean_value - value indicating whether to set the bit to 1 or to 0
 * @return the modified bitarr_t
 **/
bitarr_t BitArrSetBit(bitarr_t bit_arr, size_t index, int boolean_value);

/** @desc gets the value of a specific bit in the given bit array.
 * @param[in] bit_arr - value representing a bit array
 * @param[in] index - the index of the bit to retrieve
 * @param[in] boolean_value - any value other than zero is treated as 1 (non-zero)
 * @return int - the modified bitarr_t
 **/
int BitArrGetBit(bitarr_t bit_arr, size_t index);

/** @desc flips the value of a specific bit at a specific location
 * @param[in,out] bit_arr - value representing a bit array
 * @param[in] index - the index of the bit that is going to be set
 * @return int - 0 if off, 1 if on
 **/
bitarr_t BitArrFlipBit(bitarr_t bit_arr, size_t index);

/** @desc counts the number of set bits
 * @param[in] bit_arr - value representing a bit array
 * @return number of bits that are on
 **/
size_t BitArrCountOn(bitarr_t bit_arr);

/** @desc counts the number of off bits
 * @param[in] bit_arr - value representing a bit array
 * @return size_t - number of bits that are off
 **/
size_t BitArrCountOff(bitarr_t bit_arr);

/** @desc mirrors the given word
 * @param[in,out] bit_arr - value representing a bit array
 * @return the modified bitarr_t
 **/
bitarr_t BitArrMirror(bitarr_t bit_arr);

/** @desc rotates the word to the left
 * @param[in,out] bit_arr - value representing a bit array
 * @return the modified bitarr_t
 **/
bitarr_t BitArrRotateLeft(bitarr_t bit_arr, size_t num_rotations);

/** @desc rotates the word to the right
 * @param[in,out] bit_arr - value representing a bit array
 * @return the modified bitarr_t
 **/
bitarr_t BitArrRotateRight(bitarr_t bit_arr, size_t num_rotations);

/** @desc transforms the word into a string
 * @param[in,out] bit_arr - value representing a bit array
 * @return char* - the transformed string
 **/
char* BitArrToString(bitarr_t* word, char* dest);

#endif /*__BIT_ARRAY_H__*/n
