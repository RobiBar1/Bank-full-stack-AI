#include <stddef.h> /* size_t */
#include <stdio.h>  /* printf */
#include <string.h> /* strcmp */

#include "bit_array.h"

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"

#if _WIN32 || _WIN64
#if _WIN64
#define ENVIRONMENT_BIT 64
#else
#define ENVIRONMENT_BIT 32
#endif /* _WIN64 */
#endif /* _WIN32 || _WIN64 */

#if __GNUC__
#if __x86_64__ || __ppc64__
#define ENVIRONMENT_BIT 64
#else
#define ENVIRONMENT_BIT 32
#endif /* __x86_64__ || __ppc64__ */
#endif /* __GNUC__ */

#define UNUSED(x) (void(x))

typedef enum {TEST_STATUS_SUCCESS, TEST_STATUS_FAILURE} test_status_t;

static void CheckTest(test_status_t result, char* name);

static test_status_t BitArrSetAllOn_TestBasic(void);
static test_status_t BitArrFlipBit_TestBasic(void);
static test_status_t BitArrSetBit_TestBasic(void);
static test_status_t BitArrGetBit_TestBasic(void);
static test_status_t BitArrCountOn_TestBasic(void);
static test_status_t BitArrCountOff_TestBasic(void);
static test_status_t BitArrMirror_TestBasic(void);
static test_status_t BitArrRotateLeft_TestBasic(void);
static test_status_t BitArrRotateRight_TestBasic(void);
static test_status_t BitArrToString_TestBasic(void);

/* gd test_bit_array.c ../src/bit_array.c  ../../c/bitwise/bitwise.c ../../c/pointers2/String.c ../../c/atoi_itoa/atoi_itoa.c -I ../include/ -I ../../c/atoi_itoa/ -I ../../c/bitwise/  -I ../../c/pointers2/ */

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
	CheckTest(BitArrSetAllOn_TestBasic(), "BitArrSetAllOn_TestBasic");
	CheckTest(BitArrFlipBit_TestBasic(), "BitArrFlipBit_TestBasic");
	CheckTest(BitArrSetBit_TestBasic(), "BitArrSetBit_TestBasic");
	CheckTest(BitArrGetBit_TestBasic(), "BitArrGetBit_TestBasic");
	CheckTest(BitArrCountOn_TestBasic(), "BitArrCountOn_TestBasic");
	CheckTest(BitArrCountOff_TestBasic(), "BitArrCountOff_TestBasic");
	CheckTest(BitArrMirror_TestBasic(), "BitArrMirror_TestBasic");
	CheckTest(BitArrRotateLeft_TestBasic(), "BitArrRotateLeft_TestBasic");
	CheckTest(BitArrRotateRight_TestBasic(), "BitArrRotateRight_TestBasic");
	CheckTest(BitArrToString_TestBasic(), "BitArrToString_TestBasic");

	return 0;
}

static test_status_t BitArrSetAllOn_TestBasic(void)
{
	bitarr_t bit_arr = 0x38;
	
	bit_arr = BitArrSetAllOn(bit_arr);
	
	if ((bitarr_t)(~(bitarr_t)0) == bit_arr)
	{
		return TEST_STATUS_SUCCESS;
	}
	
	return TEST_STATUS_FAILURE;
}

static test_status_t BitArrFlipBit_TestBasic(void)
{
	bitarr_t bit_arr = 0x38; /*... 0011 1000*/
	
	bit_arr = BitArrFlipBit(bit_arr, 2); /*... 0011 1100*/
	
	if (0x3C != bit_arr)
	{
		return TEST_STATUS_FAILURE;
	}
	
	bit_arr = BitArrFlipBit(bit_arr, 4); /*... 0010 1100*/
	
	if (0x2C != bit_arr)
	{
		return TEST_STATUS_FAILURE;
	}
	
	return TEST_STATUS_SUCCESS;
}

static test_status_t BitArrSetBit_TestBasic(void)
{
	bitarr_t bit_arr = 0x38; /*... 0011 1000*/
	
	bit_arr = BitArrSetBit(bit_arr, 2, 1); /*... 0011 1100*/
	
	if (0x3C != bit_arr)
	{
		return TEST_STATUS_FAILURE;
	}
	
	bit_arr = BitArrSetBit(bit_arr, 4, 0); /*... 0010 1100*/
	
	if (0x2C != bit_arr)
	{
		return TEST_STATUS_FAILURE;
	}
	
	bit_arr = BitArrSetBit(bit_arr, 1, 0); /*... 0010 1100*/
	
	if (0x2C != bit_arr)
	{
		return TEST_STATUS_FAILURE;
	}
	
	return TEST_STATUS_SUCCESS;
}

static test_status_t BitArrGetBit_TestBasic(void)
{
	bitarr_t bit_arr = 0x38; /*... 0011 1000*/
	
	int res  = BitArrGetBit(bit_arr, 2);
	
	if (0 != res)
	{
		return TEST_STATUS_FAILURE;
	}
	
	res  = BitArrGetBit(bit_arr, 3);
	
	if (1!= res)
	{
		return TEST_STATUS_FAILURE;
	}
	
	res  = BitArrGetBit(bit_arr, 4);
	
	if (1 != res)
	{
		return TEST_STATUS_FAILURE;
	}
	
	return TEST_STATUS_SUCCESS;
}

static test_status_t BitArrCountOn_TestBasic(void)
{
	bitarr_t bit_arr = 0x95381038; /* 1001 0101 0011 1000 0001 0000 0011 1000*/
	
	int res  = BitArrCountOn(bit_arr);
	
	if (11 != res)
	{
		return TEST_STATUS_FAILURE;
	}
	
	bit_arr = 0x3588888895381038; 	/* 	1001 0101 0011 1000 0001 0000 0011 1000
										0011 0101 1000 1000 1000 1000 1000 1000
									*/
	
	res  = BitArrCountOn(bit_arr);
	
	if (21 != res)
	{
		return TEST_STATUS_FAILURE;
	}
	
	return TEST_STATUS_SUCCESS;
}

static test_status_t BitArrCountOff_TestBasic(void)
{
	bitarr_t bit_arr = 0x95381038; /* 1001 0101 0011 1000 0001 0000 0011 1000*/
	
	int res  = BitArrCountOff(bit_arr);
	
	if ((64 - 11) != res)
	{
		return TEST_STATUS_FAILURE;
	}
	
	bit_arr = 0x3588888895381038; 	/* 	1001 0101 0011 1000 0001 0000 0011 1000
										0011 0101 1000 1000 1000 1000 1000 1000
									*/
	
	res  = BitArrCountOff(bit_arr);
	
	if ((64 - 21) != res)
	{
		return TEST_STATUS_FAILURE;
	}
	
	return TEST_STATUS_SUCCESS;
}

static test_status_t BitArrMirror_TestBasic(void)
{
	bitarr_t bit_arr = 0xb5888888f5381039; 	/* 	1111 0101 0011 1000 0001 0000 0011 1001
												1011 0101 1000 1000 1000 1000 1000 1000
											*/
	
	bitarr_t res  = BitArrMirror(bit_arr);
	
	size_t ans = 0x9C081CAF111111AD;
	
	if (ans != res)
	{
		return TEST_STATUS_FAILURE;
	}

	return TEST_STATUS_SUCCESS;
}

static test_status_t BitArrRotateLeft_TestBasic(void)
{
	bitarr_t bit_arr = 0xb5888888f5381039; 	/* 	1111 0101 0011 1000 0001 0000 0011 1001
												1011 0101 1000 1000 1000 1000 1000 1000
											*/
	
	bitarr_t res  = BitArrRotateLeft(bit_arr, 3*4);
	
	size_t ans = 0x88888f5381039b58;
	
	if (ans != res)
	{
		return TEST_STATUS_FAILURE;
	}

	return TEST_STATUS_SUCCESS;
}

static test_status_t BitArrRotateRight_TestBasic(void)
{
	bitarr_t bit_arr = 0xb5888888f5381039; 	/* 	1111 0101 0011 1000 0001 0000 0011 1001
												1011 0101 1000 1000 1000 1000 1000 1000
											*/
	
	bitarr_t res  = BitArrRotateRight(bit_arr, 3*4);
	
	size_t ans = 0x039b5888888f5381;
	
	if (ans != res)
	{
		return TEST_STATUS_FAILURE;
	}

	return TEST_STATUS_SUCCESS;
}

static test_status_t BitArrToString_TestBasic(void)
{
	bitarr_t bit_arr = 0xb5888888f5381039; 	/* 	1111 0101 0011 1000 0001 0000 0011 1001
												1011 0101 1000 1000 1000 1000 1000 1000
											*/
	
	char res[ENVIRONMENT_BIT+1] = {'0'};
	char ans[] = "1011010110001000100010001000100011110101001110000001000000111001";
	
	res[ENVIRONMENT_BIT] = '\0';
	
	BitArrToString(bit_arr, res);
	
	if (0 != strcmp(res, ans))
	{
		return TEST_STATUS_FAILURE;
	}

	return TEST_STATUS_SUCCESS;
}







