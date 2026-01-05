/*
writer:  Robi
Checker: Heli
date:    05.01.2026
*/

#include <stdio.h>   /* printf, sprintf */
#include <assert.h>  /* assert */
#include <string.h>  /* string verification in tests */
#include <limits.h>  /* CHAR_BIT */
#include <stdlib.h>  /* malloc */
#define BASE(x) (x)

/* 1 */
int FlipDigitsInNum(int num)
{
    int new_num = 0;
    int is_negetive = 0; 

    if (num < 0)
    {
        is_negetive = 1;
        num = -num;
    }

    while (num > 0 && 0 == num % BASE(10))
    {
        num /= BASE(10);
    }

    while (num > 0)
    {
        new_num *= BASE(10);
        new_num += num % BASE(10);
        num /= BASE(10);
    }

    if (is_negetive)
    {
        new_num = -new_num;
    }

    return new_num;
}

/* 4. implemen after test */
unsigned char MirrorBits(unsigned char byte)
{
    unsigned char result = 0;
    int i = 0;

    for (i = 0; i < CHAR_BIT; ++i)
    {
        result = (result << 1) | (byte & 1);
        byte >>= 1;
    }

    return result;
}

/* 5 */
int FlipBit(int val, unsigned int n)
{
    return val ^ (1 << n);
}

/* 6. naive solution */
int CountOnBits(unsigned char byte)
{
    int count = 0;

    while (byte)
    {
        count += byte & 1;
        byte >>= 1;
    }

    return count;
}

/* 8 */
char RotateLeft(char byte, unsigned int nbits)
{
    unsigned char ubyte = (unsigned char)byte;

    nbits = nbits % CHAR_BIT;

    if (0 == nbits)
    {
        return byte;
    }

    return (char)((ubyte << nbits) | 
    			 (ubyte >> (CHAR_BIT - nbits)));
}

/* 10. */
void Swap(int** p1, int** p2)
{
    int* tmp = NULL;
    
    assert(NULL != p1);
    assert(NULL != p2);

    tmp = *p1;
    *p1 = *p2;
    *p2 = tmp;
}

/* 11 */
char* StrCat(char* dst, const char* src)
{
    char* start = dst;
    
    assert(NULL != dst);
    assert(NULL != src);
    
    while ('\0' != *dst)
    {
        ++dst;
    }
    while ('\0' != *src)
    {
        *dst++ = *src++;
    }
    
    *dst = '\0';
    
    return start;
}

char* StrCpy(char* dst, const char* src)
{
    char* start = dst;
    
    assert(NULL != dst);
    assert(NULL != src);

    while ('\0' != *src)
    {
        *dst++ = *src++;
    }
    
    *dst = '\0';
    
    return start;
}

char* Strncpy(char* dst, const char* src, size_t n_bytes)
{
    char* start = dst;
    
    assert(NULL != dst);
    assert(NULL != src);

    while (n_bytes > 0 && '\0' != *src)
    {
        *dst++ = *src++;
        --n_bytes;
    }
    
    while (n_bytes > 0)
    {
        *dst++ = '\0';
        --n_bytes;
    }
    
    return start;
}


size_t StrLen(const char* s)
{
    size_t count = 0;
    
    assert(NULL != s);

    while ('\0' != *s)
    {
        ++count;
        ++s; 
    }
    
    return count;
}

int StrCmp(const char* s1, const char* s2)
{
    assert(NULL != s1);
    assert(NULL != s2);
    
    while ('\0' != *s1 && '\0' != *s2 && *s1 == *s2)
    {
        ++s1;
        ++s2;
    }
    
        return (int)(*(unsigned char *)s1) - (int)(*(unsigned char *)s2);
}

/* 12 */
unsigned long GetNFibonacciElement(unsigned int n)
{
  	unsigned long prev1 = 1;
    unsigned long prev2 = 1;
    unsigned long current = 1;
    unsigned int i = 3;

    if (0 == n)
    {
        return 0;
    }

    if (n <= 2)
    {
        return 1;
    }

    for (; i <= n; ++i)
    {
        current = prev1 + prev2;
        prev2 = prev1;
        prev1 = current;
    }

    return current;
}

/* 13 */
static int CountDigits(int num)
{
    size_t counter = 0;
    while ( num > 0)
    {
        ++counter;
        num /= BASE(10);
    }
    
    return counter;
}

char* IntToString(int num, char* out_str)
{
    int is_negetive = 0, num_digits = 1;
	
	assert(NULL != out_str);
	
    if (num < 0)
    {
        ++num_digits;
        num = -num;
        ++is_negetive;
    }

    num_digits += CountDigits(num);
    out_str[--num_digits] = '\0';

    while (num > 0)
    {
        --num_digits;
        out_str[num_digits] = (num % BASE(10)) + '0';
        num /= BASE(10);
    }
    if (is_negetive)
    {
        out_str[num_digits	] = '-';
    }
    
    return out_str;
}

char* IntToStringUsingSprintf(int num, char *buffer)
{
	assert(NULL != buffer);

	sprintf(buffer, "%d", num);
	
	return buffer;
}

/* 14 */
int MultiplyBy8UsingShift(int num)
{   
    return num << 3;
}

/* 15 */
void SwapTmp(int* num1, int* num2)
{
    int tmp = 0;
    
    assert(NULL != num1);
    assert(NULL != num2);

    tmp = *num1;
    *num1 = *num2;
    *num2 = tmp;
}

void SwapByAdd(int* num1, int* num2)
{
    assert(NULL != num1);
    assert(NULL != num2);

    *num1 += *num2;
    *num2 = *num1 - *num2;
    *num1 -= *num2;
}

/* i add it after real test */
void SwapWithXor(int *num1, int *num2)
{
    assert(NULL != num1);
    assert(NULL != num2);

    if (num1 != num2)
    {
        *num1 ^= *num2;
        *num2 ^= *num1;
        *num1 ^= *num2;
    }
}

void SwapByMinus(int* num1, int* num2)
{
    assert(NULL != num1);
    assert(NULL != num2);

    *num1 -= *num2;
    *num2 = *num1 + *num2;
    *num1 = *num2 - *num1; 
}

/*----------------------- Tests -------------------------*/
void TestFlipDigitsInNum(void)
{
    printf("Testing FlipDigitsInNum...\n");
    assert(85 == FlipDigitsInNum(58));
    assert(-25 == FlipDigitsInNum(-520));
    assert(321 == FlipDigitsInNum(123));
    assert(1 == FlipDigitsInNum(100));
    assert(0 == FlipDigitsInNum(0));
    assert(-1 == FlipDigitsInNum(-1));
    assert(987654321 == FlipDigitsInNum(123456789));
    printf("FlipDigitsInNum: PASSED\n\n");
}

void TestMirrorBits(void)
{
    printf("Testing MirrorBits...\n");
    assert(0x00 == MirrorBits(0x00));
    assert(0xFF == MirrorBits(0xFF));
    assert(0x80 == MirrorBits(0x01));
    assert(0x01 == MirrorBits(0x80));
    assert(0xB4 == MirrorBits(0x2D));
    printf("MirrorBits: PASSED\n\n");
}

void TestFlipBit(void)
{
    printf("Testing FlipBit...\n");
    assert(1 == FlipBit(0, 0));
    assert(0 == FlipBit(1, 0));
    assert(3 == FlipBit(1, 1));
    assert(0x01 == FlipBit(0x00, 0)); 
    assert(6 == FlipBit(2, 2));
    printf("FlipBit: PASSED\n\n");
}

void TestCountOnBits(void)
{
    printf("Testing CountOnBits...\n");
    assert(0 == CountOnBits(0x00));
    assert(8 == CountOnBits(0xFF));
    assert(1 == CountOnBits(0x01));
    assert(4 == CountOnBits(0x0F));
    assert(2 == CountOnBits(0x03));
    printf("CountOnBits: PASSED\n\n");
}

void TestRotateLeft(void)
{
    printf("Testing RotateLeft...\n");
    assert((char)0x02 == RotateLeft((char)0x01, 1));
    assert((char)0x80 == RotateLeft((char)0x01, 7));
    assert((char)0x01 == RotateLeft((char)0x01, 8));
    assert((char)0x01 == RotateLeft((char)0x80, 1));
    assert((char)0x81 == RotateLeft((char)0xC0, 1));
    printf("RotateLeft: PASSED\n\n");
}

void TestSwapIntPointers(void)
{
    int a = 5;
    int b = 6;
    int *p1 = &a;
    int *p2 = &b;

    printf("Testing SwapIntPointers...\n");
    Swap(&p1, &p2);
    assert(5 == a && 6 == b);
    assert(6 == *p1 && 5 == *p2);
    assert(p1 == &b && p2 == &a);
    printf("SwapIntPointers: PASSED\n\n");
}

void TestStrLen(void)
{
    printf("Testing StrLen...\n");
    assert(0 == StrLen(""));
    assert(5 == StrLen("hello"));
    assert(11 == StrLen("hello world"));
    printf("StrLen: PASSED\n\n");
}

void TestStrCmp(void)
{
    printf("Testing StrCmp...\n");
    assert(0 == StrCmp("hello", "hello"));
    assert(0 > StrCmp("abc", "abd"));
    assert(0 < StrCmp("abd", "abc"));
    assert(0 > StrCmp("", "a"));
    assert(0 < StrCmp("a", ""));
    printf("StrCmp: PASSED\n\n");
}

void TestStrCpy(void)
{
    char dst[20] = {0};

    printf("Testing StrCpy...\n");
    StrCpy(dst, "hello");
    assert(0 == strcmp(dst, "hello"));
    StrCpy(dst, "");
    assert(0 == strcmp(dst, ""));
    printf("StrCpy: PASSED\n\n");
}

void TestStrncpy(void)
{
    char dst[20] = {0};

    printf("Testing Strncpy...\n");
    Strncpy(dst, "hello", 3);
    assert(0 == strncmp(dst, "hel", 3));
    Strncpy(dst, "ab", 5);
    assert(0 == strcmp(dst, "ab"));
    printf("Strncpy: PASSED\n\n");
}

void TestStrCat(void)
{
    char dst[20] = "hello";

    printf("Testing StrCat...\n");
    StrCat(dst, " world");
    assert(0 == strcmp(dst, "hello world"));
    printf("StrCat: PASSED\n\n");
}

void TestGetNFibonacciElement(void)
{
    printf("Testing GetNFibonacciElement...\n");
    assert(0 == GetNFibonacciElement(0));
    assert(1 == GetNFibonacciElement(1));
    assert(1 == GetNFibonacciElement(2));
    assert(2 == GetNFibonacciElement(3));
    assert(3 == GetNFibonacciElement(4));
    assert(5 == GetNFibonacciElement(5));
    assert(8 == GetNFibonacciElement(6));
    assert(13 == GetNFibonacciElement(7));
    assert(21 == GetNFibonacciElement(8));
    printf("GetNFibonacciElement: PASSED\n\n");
}

void TestIntToString(void)
{
    char buffer[32] = {0};

    printf("Testing IntToString...\n");
    IntToStringUsingSprintf(123, buffer);
    assert(0 == strcmp(buffer, "123"));
    IntToStringUsingSprintf(-456, buffer);
    assert(0 == strcmp(buffer, "-456"));
    IntToStringUsingSprintf(0, buffer);
    assert(0 == strcmp(buffer, "0"));
    printf("IntToString: PASSED\n\n");
}

void TestMultiplyBy8(void)
{
    printf("Testing MultiplyBy8UsingShift...\n");
    assert(0 == MultiplyBy8UsingShift(0));
    assert(8 == MultiplyBy8UsingShift(1));
    assert(80 == MultiplyBy8UsingShift(10));
    assert(-8 == MultiplyBy8UsingShift(-1));
    assert(800 == MultiplyBy8UsingShift(100));
    printf("MultiplyBy8UsingShift: PASSED\n\n");
}

void TestSwapFunctions(void)
{
    int a = 5;
    int b = 10;

    printf("Testing SwapWithTemp...\n");
    SwapTmp(&a, &b);
    assert(10 == a && 5 == b);

    a = 5; b = 10;
    printf("Testing SwapWithXor...\n");
    SwapWithXor(&a, &b);
    assert(10 == a && 5 == b);

    a = 5; b = 10;
    printf("Testing SwapWithArithmetic...\n");
    SwapByAdd(&a, &b);
    assert(10 == a && 5 == b);

    printf("All Swap functions: PASSED\n\n");
}

int main(void)
{
    printf("=== Running All Tests ===\n\n");

    TestFlipDigitsInNum();
    TestMirrorBits();
    TestFlipBit();
    TestCountOnBits();
    TestRotateLeft();
    TestSwapIntPointers();
    TestStrLen();
    TestStrCmp();
    TestStrCpy();
    TestStrncpy();
    TestStrCat();
    TestGetNFibonacciElement();
    TestIntToString();
    TestMultiplyBy8();
    TestSwapFunctions();

    printf("=== All Tests Passed! ===\n");

    return 0;
}
