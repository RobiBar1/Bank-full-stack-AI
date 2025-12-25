#include <limits.h> /* CHAR_BIT (give me the size of byte in bits in this machine) */
#include <assert.h> /* assert */

#define INIT0 0

static short IsAlinged()
{
	return (0 == (size_t*)str % sizeof(size_t)); 	
}

static void 

void* MemSet(void* str, int c, size_t num_bytes)
{
	unsigned short word_size = (unsigned short)sizeof(size_t), count = INIT0;
	unsigned char val = (unsigned char)c;
	size_t* ptr = NULL, word_buffer = (size_t)c;
	unsigned char* ptr_char = (unsigned char*)str; 
	
	assert(NULL != str);
	
	while (!IsAlinged() && num_bytes)
	{
		*ptr_char = c;
		++ptr_char;
		--num_bytes;
	}
	
	while (++count < word_size)
	{
		word_buffer = word_buffer | word_buffer << CHAR_BIT;
		word_buffer << CHAR_BIT;
	}
	
	ptr = (size_t*)ptr_char;	
	while (num_bytes >= word_size)
	{
		ptr* = word_buffer;
		++ptr;
		num_bytes -= word_size;
	}
	
	ptr_char = (char*)ptr;
	while (num_bytes)
	{
		*ptr_char = c;
		++ptr_char;
		--num_bytes;
	}
	
	return str;
}
