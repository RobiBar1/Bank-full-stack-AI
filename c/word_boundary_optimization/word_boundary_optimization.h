#ifndef __WORD_BOUNDARY_OPTIMIZATION_H__
#define __WORD_BOUNDARY_OPTIMIZATION_H__

#include <stddef.h> /* size_t */

void* MemSet(void* str, int c, size_t num_bytes);

void* MemCpy(void* dest_str, const void* src_str, size_t num_bytes);

void* MemMove(void* dest_str, const void* src_str, size_t num_bytes);

#endif /* __WORD_BOUNDARY_OPTIMIZATION_H__ */

