#include <limits.h> /* CHAR_BIT (give me the size of byte in bits in this machine) */
#include <assert.h> /* assert */
#include <stddef.h> /* size_t */

#define INIT0 0
#define WORD_SIZE (sizeof(size_t))
#define FORWARD 1
#define BACKWARD -1
#define IS_SAME_ALINGED (((size_t)(dest_str)) % (WORD_SIZE) == ((size_t)(src_str) % (WORD_SIZE)))
#define TAKE_MIN(x, y) ((x) < (y) ? (x) : (y))
static short IsAlinged(void* str)
{
	return (0 == (size_t)str % WORD_SIZE); 	
}

void* MemSet(void* str, int c, size_t num_bytes)
{
	unsigned short count = INIT0;
	unsigned char val = (unsigned char)c;
	size_t* ptr = NULL;
	size_t word_buffer = (size_t)val;
	unsigned char* ptr_char = (unsigned char*)str; 
	
	assert(NULL != str);
	
	while (!IsAlinged((void*)ptr_char) && num_bytes)
	{
		*ptr_char = val;
		++ptr_char;
		--num_bytes;
	}
	
	while (++count < WORD_SIZE)
	{
		word_buffer |= (word_buffer << CHAR_BIT);
	}
	
	ptr = (size_t*)ptr_char;	
	while (num_bytes >= WORD_SIZE)
	{
		*ptr = word_buffer;
		++ptr;
		num_bytes -= WORD_SIZE;
	}
	
	ptr_char = (unsigned char*)ptr;
	while (num_bytes)
	{
		*ptr_char = val;
		++ptr_char;
		--num_bytes;
	}
	
	return str;
}

static void CopyBytes(const void** dst_ptr, const void** src_ptr, size_t num_bytes, int direction)
{
	char* dst_runner = NULL;
	char* src_runner = NULL;

	assert(NULL != dst_ptr);
	assert(NULL != src_ptr);

	dst_runner = *(char**)dst_ptr;
	src_runner = *(char**)src_ptr;
	if (FORWARD == direction)
	{
		for (; num_bytes > 0; --num_bytes)
		{
			*dst_runner++ = *src_runner++;
		}
	}
	else
	{
		for (; num_bytes > 0; --num_bytes)
		{
			*--dst_runner = *--src_runner;
		}
	}

	*dst_ptr = (void*)dst_runner;
	*src_ptr = (void*)src_runner;
}

static void CopyWords(const void** dst_ptr, const void** src_ptr, size_t num_words, int direction)
{	
	size_t* dst_runner = NULL;
	size_t* src_runner = NULL;

	assert(NULL != dst_ptr);
	assert(NULL != src_ptr);

	dst_runner = *(size_t**)dst_ptr;
	src_runner = *(size_t**)src_ptr;
	if (direction == FORWARD)
	{
		for (; num_words > 0; --num_words)
		{
			*dst_runner++ = *src_runner++;
		}
	}
	else
	{
		for (; num_words > 0; --num_words)
		{
			*--dst_runner = *--src_runner;
		}
	}

	*dst_ptr = (void*)dst_runner;
	*src_ptr = (void*)src_runner;
}

void* MemCpy(void* dest_str, const void* src_str, size_t num_bytes)
{
	void* res = NULL; 
	size_t bytes_to_write = INIT0;
	size_t words_to_write = INIT0;

	assert(NULL != dest_str);
	assert(NULL != src_str);
	assert(IS_SAME_ALINGED);
	
	res = dest_str;
	if ((size_t)dest_str % WORD_SIZE != 0)
	{
		bytes_to_write = WORD_SIZE - (size_t)dest_str % WORD_SIZE;
		bytes_to_write = TAKE_MIN(num_bytes, bytes_to_write);
		CopyBytes((const void**)&dest_str, &src_str, bytes_to_write, FORWARD);
		num_bytes -= bytes_to_write;
	}

	if (num_bytes >= WORD_SIZE)
	{
		words_to_write = num_bytes / WORD_SIZE;
		CopyWords((const void**)&dest_str, &src_str, words_to_write, FORWARD);
		num_bytes -= words_to_write * WORD_SIZE;
	}

	if (num_bytes > 0)
	{
		bytes_to_write = num_bytes;
		CopyBytes((const void**)&dest_str, &src_str, bytes_to_write, FORWARD);
		num_bytes -= bytes_to_write;
	}

	return res;
}

static void* MemCpyBackward(void* dest_str, const void* src_str, size_t num_bytes)
{
	void* res = dest_str;
	size_t bytes_to_write = 0;
	size_t words_to_write = 0;

	assert(NULL != dest_str);
	assert(NULL != src_str);
	assert(IS_SAME_ALINGED);

	if ((size_t)dest_str % WORD_SIZE != 0)
	{
		bytes_to_write = (size_t)dest_str % WORD_SIZE;
		bytes_to_write = TAKE_MIN(num_bytes, bytes_to_write);
		CopyBytes((const void**)&dest_str, &src_str, bytes_to_write, BACKWARD);
		num_bytes -= bytes_to_write;
	}

	if (num_bytes >= WORD_SIZE)
	{
		words_to_write = num_bytes / WORD_SIZE;
		CopyWords((const void**)&dest_str, &src_str, words_to_write, BACKWARD);
		num_bytes -= words_to_write * WORD_SIZE;
	}

	if (num_bytes > 0)
	{
		bytes_to_write = num_bytes;
		CopyBytes((const void**)&dest_str, &src_str, bytes_to_write, BACKWARD);
		num_bytes -= bytes_to_write;
	}

	return res;
}

void* MemMove(void *dest_str, const void *src_str, size_t num_bytes)
{
	void* res = dest_str;

	if (dest_str < src_str)
	{
		MemCpy(dest_str, src_str, num_bytes);
	}
	else
	{
		MemCpyBackward((char*)dest_str + num_bytes, (char*)src_str + num_bytes, num_bytes);
	}

	return res;
}

