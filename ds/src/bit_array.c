#include <limits.h>    /* CHAR_BIT */
#include <assert.h>    /* assert */

#include "bit_array.h" /* our api */

#define INIT0 0
#define ALL_ONE_ON -1
#define UNUSED(X) (void(x))

bitarr_t BitArrSetAllOn(bitarr_t bit_arr)
{
	UNUSED(bit_arr);
	return ALL_ONE_ON;
}

bitarr_t BitArrSetAllOff(bitarr_t bit_arr)
{
	UNUSED(bit_arr);
	return INIT0;
}

bitarr_t BitArrSetBit(bitarr_t bit_arr, size_t index, int boolean_value)
{
	size_t mask = 0x1;
	mask = mask << index;
	
	if (boolean_value)
	{
		return bit_arr | mask;
	}
	else
	{
		return ~mask & bit_arr;
	}
}

int BitArrGetBit(bitarr_t bit_arr, size_t index)
{
	size_t mask = 0x1;
	mask = mask << index;
	
	return (mask & bit_arr) == mask;
}

bitarr_t BitArrFlipBit(bitarr_t bit_arr, size_t index)
{
	return	BitArrSetBit(bit_arr, index, 
	(!BitArrGetBit(bit_arr, index)));
}

size_t BitArrCountOn(bitarr_t bit_arr)
{
	/* check if the 32-bit macro (__i386__) is missing. If __i386__ is NOT defined, can assume its 64-bit. */
	size_t tmp_num = bit_arr;
	size_t mask_1 = 0x5555555555555555;
	size_t mask_2 = 0x3333333333333333;
	size_t mask_4 = 0x0f0f0f0f0f0f0f0f;
	size_t mask_8 = 0x00ff00ff00ff00ff;
	size_t mask_16 = 0x0000ffff0000ffff;
	size_t mask_32 = 0x00000000ffffffff;
	
	tmp_num = (tmp_num & mask_1) +
	((tmp_num >> 1) & mask_1);
	tmp_num = (tmp_num & mask_2) +
	((tmp_num >> 2) & mask_2);
	tmp_num = (tmp_num & mask_4) +
	((tmp_num >> 4) & mask_4);
	tmp_num = (tmp_num & mask_8) +
	((tmp_num >> 8) & mask_8);
	tmp_num = (tmp_num & mask_16) +
	((tmp_num >> 16) & mask_16);
	
	#ifndef __i386__ 
		return (tmp_num & mask_32) +
	((tmp_num >> 32) & mask_32);
    #else
        return tmp_num;
    #endif
}

size_t BitArrCountOff(bitarr_t bit_arr)
{
	return BitArrCountOn(~bit_arr);
}

bitarr_t BitArrMirror(bitarr_t bit_arr)
{
	size_t tmp_num = bit_arr;
	size_t mask_1 = 0x5555555555555555;
	size_t mask_2 = 0x3333333333333333;
	size_t mask_4 = 0x0f0f0f0f0f0f0f0f;
	size_t mask_8 = 0x00ff00ff00ff00ff;
	size_t mask_16 = 0x0000ffff0000ffff;
	size_t mask_32 = 0x00000000ffffffff;
	
	tmp_num = ((tmp_num & mask_1) << 1) |
	((tmp_num >> 1) & mask_1);
	tmp_num = ((tmp_num & mask_2) << 2) |
	((tmp_num >> 2) & mask_2);
	tmp_num = ((tmp_num & mask_4) << 4) |
	((tmp_num >> 4) & mask_4);
	tmp_num = ((tmp_num & mask_8) << 8) |
	((tmp_num >> 8) & mask_8);
	tmp_num = ((tmp_num & mask_16) << 16) |
	((tmp_num >> 16) & mask_16);
	
	#ifndef __i386__ 
		return ((tmp_num & mask_32) << 32) |
	((tmp_num >> 32) & mask_32);
    #else
        return tmp_num;
    #endif
}

bitarr_t BitArrRotateLeft(bitarr_t bit_arr, size_t num_rotations)
{	
	size_t copy_word = bit_arr;
	
	num_rotations %=  sizeof(bit_arr) * CHAR_BIT;
	copy_word = copy_word >> ((sizeof(bit_arr) * CHAR_BIT) - num_rotations) ;
	
	bit_arr = bit_arr << num_rotations;
	
	return bit_arr | copy_word;
}

bitarr_t BitArrRotateRight(bitarr_t bit_arr, size_t num_rotations)
{	
	size_t copy_word = bit_arr;
	
	num_rotations %=  sizeof(bit_arr) * CHAR_BIT;
	copy_word = copy_word << ((sizeof(bit_arr) * CHAR_BIT) - num_rotations) ;
	
	bit_arr = bit_arr >> num_rotations;
	
	return bit_arr | copy_word;
}

char* BitArrToString(bitarr_t bit_arr, char* dest)
{
	size_t mask_1 = 0x1;
	size_t i = INIT0;
	size_t arr_last_index = (sizeof(size_t) * CHAR_BIT) - 1;
	char c = INIT0	;
	
	assert(NULL != dest);
	
	mask_1 = mask_1 << arr_last_index;
	for (; i <= arr_last_index; ++i)
	{
		c = mask_1 & bit_arr? '1' : '0';
		dest[i] = c;
		mask_1 = mask_1 >> 1;
	}
	
	dest[i] = '\0';
	
	return dest;
}
