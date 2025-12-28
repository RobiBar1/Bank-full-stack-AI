#include <assert.h> /* assert */
#include <stddef.h> /* size_t */
#include <stdio.h> /* printf */

#include "string.h"
#include "word_boundary_optimization.h"

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"

typedef enum {TEST_STATUS_SUCCESS, TEST_STATUS_FAILURE} test_status_t;

static void CheckTest(test_status_t result, char* name);
static int OffsetBytesToNextWordBoundary(const void* str);
static int OffsetBytesFromRecentWordBoundary(const void* str);

static test_status_t MemSet_CheckFillsAligned(void);
static test_status_t MemSet_CheckFillsUnalignedTwoWords(void);
static test_status_t MemSet_CheckFillsUnaligned(void);
static test_status_t MemCpy_CheckFillsAligned(void);
static test_status_t MemCpy_Check(void);
static test_status_t MemMove_Check(void);
static test_status_t MemMove_CheckNoOverlap(void);
static test_status_t MemMove_CheckNoOverLapLong(void);
static test_status_t MemMove_CheckOverLapLong(void);

/* gd test_word_boundary_optimization.c word_boundary_optimization.c ../pointers2/String.c -I ../pointers2/ */

static int OffsetBytesFromRecentWordBoundary(const void* str)
{
	return (size_t)str % sizeof(size_t);
}

static int OffsetBytesToNextWordBoundary(const void* str)
{
	return sizeof(size_t) - OffsetBytesFromRecentWordBoundary(str);
}


static void CheckTest(test_status_t result, char* name)
{
	if (result == TEST_STATUS_SUCCESS)
	{
		printf("-> %s: ", name);
		printf(GREEN "PASSED\n\n" RESET);
	}
	else 
	{
		printf(RED "-> %s: FAILED\n\n" RESET, name);
	}
}


int main()
{
	printf("\n\n---- TEST MEMSET ----\n\n");
	CheckTest(MemSet_CheckFillsAligned(), "MemSet_CheckFillsAligned");
	CheckTest(MemSet_CheckFillsUnalignedTwoWords(), "MemSet_CheckFillsUnalignedTwoWords");
	CheckTest(MemSet_CheckFillsUnaligned(), "MemSet_CheckFillsUnaligned");
	
	printf("\n\n---- TEST MEMCPY ----\n\n");
	CheckTest(MemCpy_CheckFillsAligned(), "MemCpy_CheckFillsAligned");
	CheckTest(MemCpy_Check(), "MemCpy_Check");
	
	printf("\n\n---- TEST MEMMOVE ----\n\n");
	CheckTest(MemMove_Check(), "MemMove_Check");
	CheckTest(MemMove_CheckNoOverlap(), "MemMove_CheckNoOverlap");
	CheckTest(MemMove_CheckNoOverLapLong(), "MemMove_CheckNoOverLapLong");
	CheckTest(MemMove_CheckOverLapLong(), "MemMove_CheckOverLapLong");
	
	return 0;
}

static test_status_t MemSet_CheckFillsAligned(void)
{
	char str[] = {		'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a',
						'a', 'x', '\0'};
						
	char str_ans[] = "bbbbbbbbax";
	
	MemSet(str, 'b', 8);
	
	printf("new str: \n%s\n\n", str);
	
	if (!(strcmp(str, str_ans)))
	{
		return TEST_STATUS_SUCCESS;
	}
	return TEST_STATUS_FAILURE;
}

static test_status_t MemSet_CheckFillsUnalignedTwoWords(void)
{
	char str[] = {		'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a',
						'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a',
						'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a',
						'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a',
						'\0'};
						
	char str_ans[] = "aabbbbbbbbaaaaaaaaaaaaaaaaaaaaaa";
	
	MemSet(str + 2, 'b', 8);
	
	printf("new str: \n%s\n\n", str);
	
	if (!(strcmp(str, str_ans)))
	{
		return TEST_STATUS_SUCCESS;
	}
	return TEST_STATUS_FAILURE;
}

static test_status_t MemSet_CheckFillsUnaligned(void)
{
	char str[] = {		'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a',
						'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a',
						'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a',
						'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a',
						'\0'};
						
	char str_ans[] = "aabbbbbbbbbbbbbbbbbbaaaaaaaaaaaa";
	
	MemSet(str + 2, 'b', 18);
	
	printf("new str: \n%s\n\n", str);
	
	if (!(strcmp(str, str_ans)))
	{
		return TEST_STATUS_SUCCESS;
	}
	return TEST_STATUS_FAILURE;
}

static test_status_t MemCpy_CheckFillsAligned(void)
{
	char dest_str[] = {	'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a',
						'b', 'b', 'b', 'b', 'b', 'b', 'b', 'b',
						'c', 'c', 'c', 'c', 'c', 'c', 'c', 'c',
						'd', 'd', 'd', 'd', 'd', 'd', 'd', 'd',
						'\0'};
						
	char src_str[] = "xxxx12345678klmnopqr";
	
	char str_ans[] = "aaaaaaaaxxxx1234ccccccccdddddddd";						
	
	size_t offset_of_dest = OffsetBytesToNextWordBoundary(dest_str) % 8;
	size_t offset_of_src = OffsetBytesToNextWordBoundary(src_str) % 8;
	
	printf("offset_of_dest: %lu, offset_of_src: %lu\n", offset_of_dest, offset_of_src);
	
	MemCpy(dest_str+offset_of_dest+8, src_str+offset_of_src, 8);
	
	printf("new str: \n%s\n\n", dest_str);
	
	
	if (!(strcmp(dest_str, str_ans)))
	{
		return TEST_STATUS_SUCCESS;
	}
	return TEST_STATUS_FAILURE;
}

static test_status_t MemCpy_Check(void)
{
	char dest_str[] = {	'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a',
						'b', 'b', 'b', 'b', 'b', 'b', 'b', 'b',
						'c', 'c', 'c', 'c', 'c', 'c', 'c', 'c',
						'd', 'd', 'd', 'd', 'd', 'd', 'd', 'd',
						'\0'};
						
	char src_str[] = "xxxx12345678klmnopqr";
	
	char str_ans[] = "aaxx12345678klbbccccccccdddddddd";
	
	MemCpy(dest_str+2, src_str+2, 12);
	
	printf("new str: \n%s\n\n", dest_str);
	
	
	if (!(strcmp(dest_str, str_ans)))
	{
		return TEST_STATUS_SUCCESS;
	}
	return TEST_STATUS_FAILURE;
}


static test_status_t MemMove_Check(void)
{
	char complete_string[] = {  'x', 'x', 'r', 'r', 'r', 'r', 'r', 'r',
                            'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g',
                            'b', 'b', 'b', 'b', 'b', 'b', 'b', 'x',
                            'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x',
                            'x', 'x', 'x', 'x', 'x', 'x', 'x', '\0'}; 
						
	char* src_str = complete_string+2;
	char* dest_str = complete_string+2+(8*2);
	
	char src_str_ans[] = "rrrrrrggggggggbbrrrrrrggggggggbbbbbbb";
	char dest_str_ans[] = "rrrrrrggggggggbbbbbbb";
	
	printf("old src_str: \n%s\n\n", src_str);
	printf("old dest_str: \n%s\n\n", dest_str);
	
	MemMove(dest_str, src_str, 21);
	
	printf("new src_str: \n%s\n\n", src_str);
	printf("new dest_str: \n%s\n\n", dest_str);
	
	if (0 != (strcmp(src_str, src_str_ans)))
	{
		return TEST_STATUS_FAILURE;
	}
	
	if (0 != (strcmp(dest_str, dest_str_ans)))
	{
		return TEST_STATUS_FAILURE;
	}

	return TEST_STATUS_SUCCESS;
}

static test_status_t MemMove_CheckNoOverlap(void)
{
	char complete_string[] = {	'x', 'x', 'r', 'r', 'r', 'r', 'r', 'r',
								'r', 'r', 'r', 'r', 'g', 'g', 'g', 'g',
								'x', 'x', 'b', 'b', 'b', 'b', 'b', 'b',
								'b', 'b', 'b', 'b', 'x', 'x', 'x', 'x',
								'\0'};
						
	char* src_str = complete_string+2;
	char* dest_str = complete_string+2+(8*2);
	
	char src_str_ans[] = "rrrrrrrrrrggggxxrrrrrrrrrrxxxx";
	char dest_str_ans[] = "rrrrrrrrrrxxxx";
	
	printf("old src_str: \n%s\n\n", src_str);
	printf("old dest_str: \n%s\n\n", dest_str);
	
	MemMove(dest_str, src_str, 10);
	
	printf("new src_str: \n%s\n\n", src_str);
	printf("new dest_str: \n%s\n\n", dest_str);
	
	if (0 != (strcmp(src_str, src_str_ans)))
	{
		return TEST_STATUS_FAILURE;
	}
	
	if (0 != (strcmp(dest_str, dest_str_ans)))
	{
		return TEST_STATUS_FAILURE;
	}

	return TEST_STATUS_SUCCESS;
}

static test_status_t MemMove_CheckNoOverLapLong(void)
{
	char complete_string[] = {	'x', 'x', 'r', 'r', 'r', 'r', 'r', 'r',
								'r', 'r', 'r', 'r', 'r', 'r', 'r', 'r',
								'r', 'r', 'r', 'r', 'r', 'r', 'r', 'r',
								'r', 'r', 'r', 'r', 'g', 'g', 'g', 'g',
								'g', 'g', 'b', 'b', 'b', 'b', 'b', 'b',
								'b', 'b', 'b', 'b', 'b', 'b', 'b', 'b',
								'b', 'b', 'b', 'b', 'b', 'b', 'b', 'b',
								'b', 'b', 'b', 'b', 'y', 'y', 'y', 'y',
								'\0'};
						
	char* src_str = complete_string+2;
	char* dest_str = complete_string+2+(8*4);
	
	char src_str_ans[] = "rrrrrrrrrrrrrrrrrrrrrrrrrrggggggrrrrrrrrrrrrrrrrrrrrrrrrrryyyy";
	char dest_str_ans[] = 								 "rrrrrrrrrrrrrrrrrrrrrrrrrryyyy";
	
	printf("old src_str: \n%s\n\n", src_str);
	printf("old dest_str: \n%s\n\n", dest_str);
	
	MemMove(dest_str, src_str, 26);
	
	printf("new src_str: \n%s\n\n", src_str);
	printf("new dest_str: \n%s\n\n", dest_str);
	
	if (0 != (strcmp(src_str, src_str_ans)))
	{
		return TEST_STATUS_FAILURE;
	}
	
	if (0 != (strcmp(dest_str, dest_str_ans)))
	{
		return TEST_STATUS_FAILURE;
	}

	return TEST_STATUS_SUCCESS;
}

static test_status_t MemMove_CheckOverLapLong(void)
{
	char complete_string[] = {	'x', 'x', 'y', 'y', 'y', 'y', 'y', 'y',
								'r', 'r', 'r', 'r', 'r', 'r', 'r', 'r',
								'r', 'r', 'r', 'r', 'r', 'r', 'r', 'r',
								'r', 'r', 'g', 'g', 'g', 'g', 'g', 'g',
								'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g',
								'g', 'g', 'g', 'g', 'g', 'g', 'g', 'g',
								'g', 'g', 'b', 'b', 'b', 'b', 'b', 'b',
								'b', 'b', 'b', 'b', 'b', 'b', 'b', 'b',
								'b', 'b', 'b', 'b', 'b', 'b', 'b', 'b',
								'b', 'b', 'x', 'x', 'x', 'x', 'x', 'x',
								'\0'};
						
	char* src_str = complete_string+2;
	char* dest_str = complete_string+2+(8*3);
	
	char src_str_ans[] =
	 "yyyyyyrrrrrrrrrrrrrrrrrryyyyyyrrrrrrrrrrrrrrrrrrggggggggggggggggggggggggxxxxxx";
	char dest_str_ans[] =    "yyyyyyrrrrrrrrrrrrrrrrrrggggggggggggggggggggggggxxxxxx";
	
	printf("old src_str: \n%s\n\n", src_str);
	printf("old dest_str: \n%s\n\n", dest_str);
	
	MemMove(dest_str, src_str, 48);
	
	printf("new src_str: \n%s\n\n", src_str);
	printf("new dest_str: \n%s\n\n", dest_str);
	
	if (0 != (strcmp(src_str, src_str_ans)))
	{
		return TEST_STATUS_FAILURE;
	}
	
	if (0 != (strcmp(dest_str, dest_str_ans)))
	{
		return TEST_STATUS_FAILURE;
	}

	return TEST_STATUS_SUCCESS;
}





