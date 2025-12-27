#include <stdio.h>
#include <limits.h>
#include <assert.h>

#include "String.h"
#include "atoi_itoa.h"

static int g_fail = 0;

/*
gd -I ../pointers2 atoi_itoa.c test_atoi_itoa.c ../pointers2/String.c 
*/

static void PassedCompareInt(int expected, int received, const char *test_name)
{
    if (expected != received)
    {
        printf("FAILED: %s\n  expected: %d\n  received: %d\n", test_name, expected, received);
        ++g_fail;
    }
    else
    {
        /* printf("PASSED: %s\n", test_name); */
    }
}

static void PassedCompareString(const char *expected, const char *received, const char *test_name)
{
    if (0 != StrCmp(expected, received))
    {
        printf("FAILED: %s\n  expected: \"%s\"\n  received: \"%s\"\n", test_name, expected, received);
        ++g_fail;
    }
    else
    {
        /* printf("PASSED: %s\n", test_name); */
    }
}

static void TestOneItoa(int value, unsigned char base, const char *expected, const char *name)
{
    char buf[80];
    char *ret = Itoa(value, buf, base);

    if (ret != buf)
    {
        printf("FAILED: %s (return pointer not buf)\n", name);
        ++g_fail;
        return;
    }

    PassedCompareString(expected, buf, name);
}

static void TestItoa(void)
{
    /* base 10 */
    TestOneItoa(0, 10, "0", "Itoa(0, base10)");
    TestOneItoa(7, 10, "7", "Itoa(7, base10)");
    TestOneItoa(567, 10, "567", "Itoa(567, base10)");
    TestOneItoa(-567, 10, "-567", "Itoa(-567, base10)");

    /* boundaries base 10 (assuming 32-bit int; if different, adjust) */
    TestOneItoa(INT_MAX, 10, "2147483647", "Itoa(INT_MAX, base10)");
    TestOneItoa(INT_MIN, 10, "-2147483648", "Itoa(INT_MIN, base10)");

    /* base 2 */
    TestOneItoa(5, 2, "101", "Itoa(5, base2)");
    TestOneItoa(-5, 2, "-101", "Itoa(-5, base2)");
    TestOneItoa(8, 2, "1000", "Itoa(8, base2)");

    /* base 16 (your digits[] are lowercase!) */
    TestOneItoa(0, 16, "0", "Itoa(0, base16)");
    TestOneItoa(15, 16, "f", "Itoa(15, base16)");
    TestOneItoa(16, 16, "10", "Itoa(16, base16)");
    TestOneItoa(255, 16, "ff", "Itoa(255, base16)");
    TestOneItoa(-255, 16, "-ff", "Itoa(-255, base16)");
    TestOneItoa(123, 16, "7b", "Itoa(123, base16)");

    /* base 36 */
    TestOneItoa(35, 36, "z", "Itoa(35, base36)");
    TestOneItoa(36, 36, "10", "Itoa(36, base36)");
    TestOneItoa(71, 36, "1z", "Itoa(71, base36)");
}

/* helper for Atoi tests */
static void TestOneAtoi(const char *s, unsigned char base, int expected, const char *name)
{
    int got = Atoi(s, base);
    PassedCompareInt(expected, got, name);
}

static void TestAtoi(void)
{
    /* basic decimal */
    TestOneAtoi("0", 10, 0, "Atoi(\"0\",10)");
    TestOneAtoi("7", 10, 7, "Atoi(\"7\",10)");
    TestOneAtoi("-567", 10, -567, "Atoi(\"-567\",10)");
    TestOneAtoi("567", 10, 567, "Atoi(\"567\",10)");

    /* leading spaces / tabs */
    TestOneAtoi("   12", 10, 12, "Atoi(\"   12\",10)");
    TestOneAtoi("\t\t-34", 10, -34, "Atoi(\"\\t\\t-34\",10)");

    /* stops at first non-digit */
    TestOneAtoi("123abc", 10, 123, "Atoi(\"123abc\",10) stops at a");
    TestOneAtoi("-99xyz", 10, -99, "Atoi(\"-99xyz\",10) stops at x");

    /* base 16 (lowercase expected by your CharToInt) */
    TestOneAtoi("ff", 16, 255, "Atoi(\"ff\",16)");
    TestOneAtoi("-ff", 16, -255, "Atoi(\"-ff\",16)");
    TestOneAtoi("7b", 16, 123, "Atoi(\"7b\",16)");

    /* base 2 */
    TestOneAtoi("101", 2, 5, "Atoi(\"101\",2)");
    TestOneAtoi("-101", 2, -5, "Atoi(\"-101\",2)");

    /* base 36 */
    TestOneAtoi("z", 36, 35, "Atoi(\"z\",36)");
    TestOneAtoi("1z", 36, 71, "Atoi(\"1z\",36)");

    /* edge cases */
    TestOneAtoi("", 10, 0, "Atoi(\"\",10) empty");
    TestOneAtoi("   ", 10, 0, "Atoi(\"   \",10) spaces only");

    /* tricky cases that SHOULD be handled (good for catching bugs) */
    TestOneAtoi("00012", 10, 12, "Atoi(\"00012\",10) leading zeros");

    /* digit '0' inside number (very important edge) */
    TestOneAtoi("1010", 2, 10, "Atoi(\"1010\",2) includes zeros");

    /* invalid for base: standard behavior is to stop when digit >= base
       Example: \"19\" base 8 should give 1 (stop at 9)
       If your implementation doesn't check digit < base, this test will FAIL (and that's useful). */
    TestOneAtoi("19", 8, 1, "Atoi(\"19\",8) stop at 9 (digit>=base)");
    TestOneAtoi("-2147483648", 10, INT_MIN, "Atoi(\"-2147483648\",10) INT_MIN");

    /* uppercase letters: your CharToInt currently does NOT support uppercase
       So \"FF\" base16 would ideally be 255, but with your code it may fail.
       Uncomment if you want to enforce uppercase support.
    */
    /* TestOneAtoi("FF", 16, 255, "Atoi(\"FF\",16) uppercase"); */
}

static void TestPrintIntersectionFirstTwoWithExclusionThird(void)
{
	printf("PrintIntersectionFirstTwoWithExclusionThird\narr1: aabbccddee\narr2: abcde\narr3: ab\n");
	PrintIntersectionFirstTwoWithExclusionThird("aabbccddee" ,10, "abcde" , 5, "ab" , 2);
}


static int TestAtoiVsSystem(const char* str)
{
	int my_result = 0;
	int system_result = 0;
	
	assert(NULL != str);
	
	my_result = Atoi10(str);
	system_result = atoi(str);
	
	if (my_result == system_result)
	{
		return 1;
	}
	
	return 0;
}

void TestAtoiEdgeCases(void)
{
	printf("=== Testing Atoi10 vs System atoi ===\n");
	printf("Testing \"0\": %s\n", TestAtoiVsSystem("0") ? "PASS" : "FAIL");
	printf("Testing \"123\": %s\n", TestAtoiVsSystem("123") ? "PASS" : "FAIL");
	printf("Testing \"-123\": %s\n", TestAtoiVsSystem("-123") ? "PASS" : "FAIL");
	printf("Testing \"  42\": %s\n", TestAtoiVsSystem("  42") ? "PASS" : "FAIL");
	printf("Testing \"\\t-99\": %s\n", TestAtoiVsSystem("\t-99") ? "PASS" : "FAIL");
	printf("Testing \"2147483647\": %s\n", TestAtoiVsSystem("2147483647") ? "PASS" : "FAIL");
	printf("Testing \"-2147483648\": %s\n", TestAtoiVsSystem("-2147483648") ? "PASS" : "FAIL");
	printf("Testing \"123abc\": %s\n", TestAtoiVsSystem("123abc") ? "PASS" : "FAIL");
	printf("Testing \"\": %s\n", TestAtoiVsSystem("") ? "PASS" : "FAIL");
	printf("Testing \"   \": %s\n", TestAtoiVsSystem("   ") ? "PASS" : "FAIL");
	printf("Testing \"00012\": %s\n", TestAtoiVsSystem("00012") ? "PASS" : "FAIL");
	printf("Testing \"-0\": %s\n", TestAtoiVsSystem("-0") ? "PASS" : "FAIL");
	printf("=== End of Atoi Comparison Tests ===\n\n");
}
int main(void)
{
    TestItoa();
    TestAtoi();
    TestAtoiEdgeCases();
	TestPrintIntersectionFirstTwoWithExclusionThird();
    printf("\nSUMMARY: %s (%d failed)\n", (g_fail == 0) ? "ALL PASSED" : "SOME FAILED", g_fail);
    return (g_fail == 0) ? 0 : 1;
}




