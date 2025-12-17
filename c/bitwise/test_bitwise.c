# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <strings.h>
# include <assert.h>

# include "bitwise.h"

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"

#define UNUSED(x) ((void)x)

typedef enum {TEST_STATUS_SUCCESS, TEST_STATUS_FAILURE} test_status_t;


/* gd test_bitwise.c bitwise.c */

static void CheckTest(test_status_t result, char* name);

static test_status_t CheckPowBasic();
static test_status_t CheckPowLarger();
static test_status_t CheckPowXIs0();
static test_status_t CheckPowYIs0();
static test_status_t CheckIsPower2Loop_IsPower();
static test_status_t CheckIsPower2Loop_IsNotPower();
static test_status_t CheckIsPower2NotLoop_IsPower();
static test_status_t CheckIsPower2NotLoop_IsNotPower();
static test_status_t CheckIncrement_Basic();
static test_status_t CheckIncrement_Negative();
static void CheckPrint3BitsOn();
static test_status_t ByteMirrorLoop_Simple();
static test_status_t ByteMirrorLoop_One();
static test_status_t ByteMirrorLoop_Interesting();
static test_status_t ByteMirrorNoLoop_Simple();
static test_status_t ByteMirrorNoLoop_One();
static test_status_t ByteMirrorNoLoop_Interesting();
static test_status_t Is2And6BitBothOn_MultipleOn();
static test_status_t Is2And6BitBothOn_34();
static test_status_t Is2And6BitBothOn_OnlyOne();
static test_status_t Is2And6BitAtLeastOneOn_MultipleOn();
static test_status_t Is2And6BitAtLeastOneOn_34();
static test_status_t Is2And6BitAtLeastOneOn_OnlyOne();
static test_status_t Is2And6BitAtLeastOneOn_Neither();
static test_status_t Swap3And5Bit_Basic();
static test_status_t Swap3And5Bit_Basic2();
static test_status_t ClosestDivisible16_Basic();
static test_status_t ClosestDivisible16_Small();
static test_status_t Swap_Basic();
static test_status_t Swap_WithNeg();
static test_status_t Swap2_Basic();
static test_status_t Swap2_WithNeg();
static test_status_t Swap3_Basic();
static test_status_t Swap3_WithNeg();
static test_status_t CountBitsLoop_Basic();
static test_status_t CountBitsNoLoop_Basic();
static void CheckPrintFloatBitwise();

int main()
{
	UNUSED(CheckTest);
	
	printf("---- TEST Pow2 ----\n");
	CheckTest(CheckPowBasic(), "CheckPowBasic()");
	CheckTest(CheckPowLarger(), "CheckPowLarger()");
	CheckTest(CheckPowXIs0(), "CheckPowXIs0()");
	CheckTest(CheckPowYIs0(), "CheckPowYIs0()");
	
	printf("---- TEST IsPower2Loop ----\n");
	CheckTest(CheckIsPower2Loop_IsPower(), "CheckIsPower2Loop_IsPower()");
	CheckTest(CheckIsPower2Loop_IsNotPower(), "CheckIsPower2Loop_IsNotPower()");
	
	printf("---- TEST IsPower2NoLoop ----\n");
	CheckTest(CheckIsPower2NotLoop_IsPower(), "CheckIsPower2NotLoop_IsPower()");
	CheckTest(CheckIsPower2NotLoop_IsNotPower(), "CheckIsPower2NotLoop_IsNotPower()");
	
	printf("---- TEST Increment ----\n");
	CheckTest(CheckIncrement_Basic(), "CheckIncrement_Basic()");
	CheckTest(CheckIncrement_Negative(), "CheckIncrement_Negative()");
	
	printf("---- TEST Print3BitsOn ----\n");
	CheckPrint3BitsOn();
	
	printf("---- TEST ByteMirrorLoop ----\n");
	CheckTest(ByteMirrorLoop_Simple(), "ByteMirrorLoop_Simple()");
	CheckTest(ByteMirrorLoop_One(), "ByteMirrorLoop_One()");
	CheckTest(ByteMirrorLoop_Interesting(), "ByteMirrorLoop_Interesting()");
	
	printf("---- TEST ByteMirrorNoLoop ----\n");
	CheckTest(ByteMirrorNoLoop_Simple(), "ByteMirrorNoLoop_Simple()");
	CheckTest(ByteMirrorNoLoop_One(), "ByteMirrorNoLoop_One()");
	CheckTest(ByteMirrorNoLoop_Interesting(), "ByteMirrorNoLoop_Interesting()");
	
	printf("---- TEST Is2And6BitBothOn ----\n");
	CheckTest(Is2And6BitBothOn_MultipleOn(), "Is2And6BitBothOn_MultipleOn()");
	CheckTest(Is2And6BitBothOn_34(), "Is2And6BitBothOn_34()");
	CheckTest(Is2And6BitBothOn_OnlyOne(), "Is2And6BitBothOn_OnlyOne()");
	
	printf("---- TEST Is2And6BitAtLeastOneOn ----\n");
	CheckTest(Is2And6BitAtLeastOneOn_MultipleOn(), "Is2And6BitAtLeastOneOn_MultipleOn()");
	CheckTest(Is2And6BitAtLeastOneOn_34(), "Is2And6BitAtLeastOneOn_34()");
	CheckTest(Is2And6BitAtLeastOneOn_OnlyOne(), "Is2And6BitAtLeastOneOn_OnlyOne()");
	CheckTest(Is2And6BitAtLeastOneOn_Neither(), "Is2And6BitAtLeastOneOn_Neither()");
	
	printf("---- TEST Swap3And5Bit ----\n");
	CheckTest(Swap3And5Bit_Basic(), "Swap3And5Bit_Basic()");
	CheckTest(Swap3And5Bit_Basic2(), "Swap3And5Bit_Basic2()");
	
	printf("---- TEST ClosestDivisible16 ----\n");
	CheckTest(ClosestDivisible16_Basic(), "ClosestDivisible16_Basic()");
	CheckTest(ClosestDivisible16_Small(), "ClosestDivisible16_Small()");
	
	printf("---- TEST Swap ----\n");
	CheckTest(Swap_Basic(), "Swap_Basic()");
	CheckTest(Swap_WithNeg(), "Swap_WithNeg()");
	
	printf("---- TEST Swap2 ----\n");
	CheckTest(Swap2_Basic(), "Swap2_Basic()");
	CheckTest(Swap2_WithNeg(), "Swap2_WithNeg()");
	
	printf("---- TEST Swap3 ----\n");
	CheckTest(Swap3_Basic(), "Swap3_Basic()");
	CheckTest(Swap3_WithNeg(), "Swap3_WithNeg()");
	
	printf("---- TEST CountBitsLoop ----\n");
	CheckTest(CountBitsLoop_Basic(), "CountBitsLoop_Basic()");
	
	printf("---- TEST CountBitsNoLoop ----\n");
	CheckTest(CountBitsNoLoop_Basic(), "CountBitsNoLoop_Basic()");
	
	printf("---- TEST CheckPrintFloatBitwise ----\n");
	CheckPrintFloatBitwise();

	return 0;	
}


static void CheckTest(test_status_t result, char* name)
{
	if (result == TEST_STATUS_SUCCESS)
	{
		printf("-> %s: ", name);
		printf(GREEN "PASSED\n" RESET);
	}
	else 
	{
		printf(RED "-> %s: FAILED\n" RESET, name);
	}
}

static test_status_t CheckPowBasic()
{
	unsigned int x = 13;
	unsigned int y = 2;
	
	if (52 == Pow2(x,y))
	{
		return TEST_STATUS_SUCCESS;
	}
	
	return TEST_STATUS_FAILURE;
}

static test_status_t CheckPowLarger()
{
	unsigned int x = 42;
	unsigned int y = 5;
	
	if (1344 == Pow2(x,y))
	{
		return TEST_STATUS_SUCCESS;
	}
	
	return TEST_STATUS_FAILURE;
}

static test_status_t CheckPowXIs0()
{
	unsigned int x = 0;
	unsigned int y = 5;
	
	if (0 == Pow2(x,y))
	{
		return TEST_STATUS_SUCCESS;
	}
	
	return TEST_STATUS_FAILURE;
}

static test_status_t CheckPowYIs0()
{
	unsigned int x = 15;
	unsigned int y = 0;
	
	if (15 == Pow2(x,y))
	{
		return TEST_STATUS_SUCCESS;
	}
	
	return TEST_STATUS_FAILURE;
}

static test_status_t CheckIsPower2Loop_IsPower()
{
	unsigned int x = 16;
	
	if (1 == IsPower2Loop(x))
	{
		return TEST_STATUS_SUCCESS;
	}
	
	return TEST_STATUS_FAILURE;
}

static test_status_t CheckIsPower2Loop_IsNotPower()
{
	unsigned int x = 15;
	
	if (0 == IsPower2Loop(x))
	{
		return TEST_STATUS_SUCCESS;
	}
	
	return TEST_STATUS_FAILURE;
}

static test_status_t CheckIsPower2NotLoop_IsPower()
{
	unsigned int x = 16;
	
	if (1 == IsPower2NoLoop(x))
	{
		return TEST_STATUS_SUCCESS;
	}
	
	return TEST_STATUS_FAILURE;
}

static test_status_t CheckIsPower2NotLoop_IsNotPower()
{
	unsigned int x = 15;
	
	if (0 == IsPower2NoLoop(x))
	{
		return TEST_STATUS_SUCCESS;
	}
	
	return TEST_STATUS_FAILURE;
}

static test_status_t CheckIncrement_Basic()
{
	int x = 11;
	x = IncrementByOne(x);
	
	if (12 == x)
	{
		return TEST_STATUS_SUCCESS;
	}
	
	return TEST_STATUS_FAILURE;
}

static test_status_t CheckIncrement_Negative()
{
	int x = -11;
	x = IncrementByOne(x);
	
	if (-10 == x)
	{
		return TEST_STATUS_SUCCESS;
	}
	
	return TEST_STATUS_FAILURE;
}

static void CheckPrint3BitsOn()
{
	unsigned int num_arr[] = {
		(unsigned int)12,
	 	(unsigned int)11, 
	 	(unsigned int)14, 
	 	(unsigned int)15
	};
	Print3BitsOn(num_arr, 4);
}

static test_status_t ByteMirrorLoop_Simple()
{
	unsigned char mirrored = ByteMirrorLoop(2);
	
	if (64 == mirrored)
	{
		return TEST_STATUS_SUCCESS;
	}
	
	return TEST_STATUS_FAILURE;
}

static test_status_t ByteMirrorLoop_One()
{
	unsigned char mirrored = ByteMirrorLoop(1);
	
	if (128 == mirrored)
	{
		return TEST_STATUS_SUCCESS;
	}
	
	return TEST_STATUS_FAILURE;
}

static test_status_t ByteMirrorLoop_Interesting()
{
	unsigned char mirrored = ByteMirrorLoop(35);
	
	if (196 == mirrored)
	{
		return TEST_STATUS_SUCCESS;
	}
	
	return TEST_STATUS_FAILURE;
}

static test_status_t ByteMirrorNoLoop_Simple()
{
	unsigned char mirrored = ByteMirrorNoLoop(2);
	
	if (64 == mirrored)
	{
		return TEST_STATUS_SUCCESS;
	}
	
	return TEST_STATUS_FAILURE;
}

static test_status_t ByteMirrorNoLoop_One()
{
	unsigned char mirrored = ByteMirrorNoLoop(1);
	
	if (128 == mirrored)
	{
		return TEST_STATUS_SUCCESS;
	}
	
	return TEST_STATUS_FAILURE;
}

static test_status_t ByteMirrorNoLoop_Interesting()
{
	unsigned char mirrored = ByteMirrorNoLoop(35);
	
	if (196 == mirrored)
	{
		return TEST_STATUS_SUCCESS;
	}
	
	return TEST_STATUS_FAILURE;
}

static test_status_t Is2And6BitBothOn_MultipleOn()
{
	
	if (Is2And6BitBothOn(46))
	{
		return TEST_STATUS_SUCCESS;
	}
	
	return TEST_STATUS_FAILURE;
}

static test_status_t Is2And6BitBothOn_34()
{
	
	if (Is2And6BitBothOn(34))
	{
		return TEST_STATUS_SUCCESS;
	}
	
	return TEST_STATUS_FAILURE;
}

static test_status_t Is2And6BitBothOn_OnlyOne()
{
	if (!Is2And6BitBothOn(33))
	{
		return TEST_STATUS_SUCCESS;
	}
	
	return TEST_STATUS_FAILURE;
}

static test_status_t Is2And6BitAtLeastOneOn_MultipleOn()
{
	
	if (Is2And6BitAtLeastOneOn(46))
	{
		return TEST_STATUS_SUCCESS;
	}
	
	return TEST_STATUS_FAILURE;
}

static test_status_t Is2And6BitAtLeastOneOn_34()
{
	
	if (Is2And6BitAtLeastOneOn(34))
	{
		return TEST_STATUS_SUCCESS;
	}
	
	return TEST_STATUS_FAILURE;
}

static test_status_t Is2And6BitAtLeastOneOn_OnlyOne()
{
	
	if (Is2And6BitAtLeastOneOn(33))
	{
		return TEST_STATUS_SUCCESS;
	}
	
	return TEST_STATUS_FAILURE;
}

static test_status_t Is2And6BitAtLeastOneOn_Neither()
{
	
	if (!Is2And6BitAtLeastOneOn(5))
	{
		return TEST_STATUS_SUCCESS;
	}
	
	return TEST_STATUS_FAILURE;
}

static test_status_t Swap3And5Bit_Basic()
{
	if (56 == Swap3And5Bit(44))
	{
		return TEST_STATUS_SUCCESS;
	}
	
	return TEST_STATUS_FAILURE;
}

static test_status_t Swap3And5Bit_Basic2()
{
	
	if (27 == Swap3And5Bit(15))
	{
		return TEST_STATUS_SUCCESS;
	}
	
	return TEST_STATUS_FAILURE;
}

static test_status_t ClosestDivisible16_Basic()
{
	if (16 == ClosestDivisible16(27))
	{
		return TEST_STATUS_SUCCESS;
	}
	
	return TEST_STATUS_FAILURE;
}

static test_status_t ClosestDivisible16_Small()
{
	if (0 == ClosestDivisible16(14))
	{
		return TEST_STATUS_SUCCESS;
	}
	
	return TEST_STATUS_FAILURE;
}

static test_status_t Swap_Basic()
{
	int x = 7;
	int y = 8;
	Swap(&x, &y);
	
	if ((8 == x) && (7 == y))
	{
		return TEST_STATUS_SUCCESS;
	}
	
	return TEST_STATUS_FAILURE;
}

static test_status_t Swap_WithNeg()
{
	int x = 7;
	int y = -8;
	Swap(&x, &y);
	
	if ((-8 == x) && (7 == y))
	{
		return TEST_STATUS_SUCCESS;
	}
	
	return TEST_STATUS_FAILURE;
}

static test_status_t Swap2_Basic()
{
	int x = 7;
	int y = 8;
	Swap2(&x, &y);
	
	if ((8 == x) && (7 == y))
	{
		return TEST_STATUS_SUCCESS;
	}
	
	return TEST_STATUS_FAILURE;
}

static test_status_t Swap2_WithNeg()
{
	int x = 7;
	int y = -8;
	Swap2(&x, &y);
	
	if ((-8 == x) && (7 == y))
	{
		return TEST_STATUS_SUCCESS;
	}
	
	return TEST_STATUS_FAILURE;
}

static test_status_t Swap3_Basic()
{
	int x = 8;
	int y = 7;
	Swap3(&x, &y);
	
	if ((7 == x) && (8 == y))
	{
		return TEST_STATUS_SUCCESS;
	}
	
	return TEST_STATUS_FAILURE;
}

static test_status_t Swap3_WithNeg()
{
	int x = 7;
	int y = -8;
	Swap3(&x, &y);
	
	if ((-8 == x) && (7 == y))
	{
		return TEST_STATUS_SUCCESS;
	}
	
	return TEST_STATUS_FAILURE;
}

static test_status_t CountBitsLoop_Basic()
{
	size_t x = CountBitsLoop(43);
	
	if (4 == x)
	{
		return TEST_STATUS_SUCCESS;
	}
	
	return TEST_STATUS_FAILURE;
}

static test_status_t CountBitsNoLoop_Basic()
{
	size_t x = CountBitsNoLoop(43);
	
	if (4 == x)
	{
		return TEST_STATUS_SUCCESS;
	}
	
	return TEST_STATUS_FAILURE;
}

static void CheckPrintFloatBitwise()
{
	float f = 62.7;
	PrintFloatBitwise(f);
}





