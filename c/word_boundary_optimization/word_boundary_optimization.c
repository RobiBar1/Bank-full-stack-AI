#include <limits.h> /* CHAR_BIT (give me the size of byte in bits in this machine) */
#include <assert.h> /* assert */

#define INIT1 1
#define RAISE_COUNT_POW2 (count *= 2)
#define Shift_MASK_LEFT_BY_POW2 ((mask) | ((mask) << ((byte_size) * (count))))

void* MemSet(void* str, int c, size_t num_bytes)
{
	unsigned short word_size = (unsigned short)sizeof(size_t),
	count = INIT1, byte_size = CHAR_BIT;
	unsigned char val = (unsigned char)c;
	size_t mask = val | 0;
	--num_bytes;
	
	assert(NULL != str);
	
	while (count < word_size && num_bytes >= count)
	{
	    mask = Shift_MASK_LEFT_BY_POW2;
	    num_bytes -= count;
	    RAISE_COUNT_POW2;
	}
	
	if (num)
	
	return str;
}
