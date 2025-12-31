#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <stdint.h>  
#include "bit_array.h" /* own header */

#define TOTAL_BITS ((size_t)(sizeof(bitarr_t) * CHAR_BIT))
#define ALL_ONES   ((bitarr_t)~(bitarr_t)0)

#if (SIZE_MAX == 0xFFFFFFFFFFFFFFFF)
    #define BITARR_64
#elif (SIZE_MAX == 0xFFFFFFFF)
    #define BITARR_32
#else
    #error "Unsupported platform: SIZE_MAX is not 32/64"
#endif

#ifdef BITARR_64
    #define ALT_A ((bitarr_t)0xAAAAAAAAAAAAAAAA)
    #define ALT_5 ((bitarr_t)0x5555555555555555)
#else /* BITARR_32 */
    #define ALT_A ((bitarr_t)0xAAAAAAAA)
    #define ALT_5 ((bitarr_t)0x55555555)
#endif


static void BuildExpectedBitString(bitarr_t bit_arr, char *out)
{
    size_t i = 0;
    for (i = 0; i < TOTAL_BITS; ++i)
    {
        bitarr_t mask = (bitarr_t)1 << (TOTAL_BITS - 1 - i);
        out[i] = (bit_arr & mask) ? '1' : '0';
    }
    out[TOTAL_BITS] = '\0';
}


void TestBitArrSetAllOn(bitarr_t bit_arr)
{
    bit_arr = BitArrSetAllOn(bit_arr);
    if(ALL_ONES == bit_arr)
    {
        printf("PASSED: From: %ld to: %ld \n", (long)ALL_ONES, (long)bit_arr);
    }
    else
    {
        printf("FAILED: From: %ld to: %ld\n", (long)bit_arr, (long)bit_arr);
    }
}

void TestBitArrSetAllOff(bitarr_t bit_arr)
{
    bit_arr = BitArrSetAllOff(bit_arr);
    if(0 == bit_arr)
    {
        printf("PASSED: From: %ld to: %ld \n", (long)bit_arr, (long)bit_arr);
    }
    else
    {
        printf("FAILED: From: %ld to: %ld \n", (long)bit_arr, (long)bit_arr);
    }
}

void TestBitArrGetBit(bitarr_t bit_arr, size_t index, int expected)
{
    int result = BitArrGetBit(bit_arr, index);

    if (result == expected)
    {
        printf("PASSED: BitArrGetBit(%ld, %ld) = %d\n",
               (long)bit_arr, (long)index, result);
    }
    else
    {
        printf("FAILED: BitArrGetBit(%ld, %ld) = %d (expected %d)\n",
               (long)bit_arr, (long)index, result, expected);
    }
}

void TestBitArrSetBit(bitarr_t bit_arr,
                      size_t index,
                      int boolean_value,
                      bitarr_t expected)
{
    bitarr_t result = BitArrSetBit(bit_arr, index, boolean_value);

    if (result == expected)
    {
        printf("PASSED: SetBit(%ld, %ld, %d) -> %ld\n",
               (long)bit_arr,
               (long)index,
               boolean_value,
               (long)result);
    }
    else
    {
        printf("FAILED: SetBit(%ld, %ld, %d) -> %ld (expected %ld)\n",
               (long)bit_arr,
               (long)index,
               boolean_value,
               (long)result,
               (long)expected);
    }
}

void TestBitArrFlipBit(bitarr_t bit_arr,
                      size_t index,
                      bitarr_t expected)
{
    bitarr_t result = BitArrFlipBit(bit_arr, index);

    if (result == expected)
    {
        printf("PASSED: FlipBit(%ld, %ld) -> %ld\n",
               (long)bit_arr,
               (long)index,
               (long)result);
    }
    else
    {
        printf("FAILED: FlipBit(%ld, %ld) -> %ld(expected %ld)\n",
               (long)bit_arr,
               (long)index,
               (long)result,
               (long)expected);
    }
}

void TestBitArrCountOn(bitarr_t bit_arr, size_t expected)
{
    size_t result = BitArrCountOn(bit_arr);

    if (result == expected)
    {
        printf("PASSED: CountOn(%ld) -> %ld\n",
               (long)bit_arr,
               (long)result);
    }
    else
    {
        printf("FAILED: CountOn(%ld) -> %ld (expected %ld)\n",
               (long)bit_arr,
               (long)result,
               (long)expected);
    }
}

void TestBitArrCountOff(bitarr_t bit_arr, size_t expected)
{
    size_t result = BitArrCountOff(bit_arr);

    if (result == expected)
    {
        printf("PASSED: CountOff(%ld) -> %ld \n",
               (long)bit_arr,
               (long)result);
    }
    else
    {
        printf("FAILED: CountOff(%ld) -> %ld (expected %ld)\n",
              (long)bit_arr,
              (long)result,
              (long)expected);
    }
}

void TestBitArrRotateLeft(bitarr_t bit_arr, size_t num_rotations, bitarr_t expected)
{
    bitarr_t got = BitArrRotateLeft(bit_arr, num_rotations);

    if (got == expected)
        printf("PASSED BitArrRotateLeft(): Got: %ld Expected: %ld\n",
               (long)got, (long)expected);
    else
        printf("FAILED BitArrRotateLeft(): Got: %ld Expected: %ld\n",
               (long)got, (long)expected);
}

void TestBitArrRotateRight(bitarr_t bit_arr, size_t num_rotations, bitarr_t expected)
{
    bitarr_t got = BitArrRotateRight(bit_arr, num_rotations);

    if (got == expected)
        printf("PASSED BitArrRotateRight(): Got: %ld Expected: %ld\n",
              (long)got, (long)expected);
    else
        printf("FAILED BitArrRotateRight(): Got: %ld Expected: %ld\n",
               (long)got, (long)expected);
}

void TestBitArrMirror(bitarr_t bit_arr, bitarr_t expected)
{
    bitarr_t got = BitArrMirror(bit_arr);

    if (got == expected)
        printf("PASSED BitArrMirror(): Got: %ld Expected: %ld\n",
               (long)got, (long)expected);
    else
        printf("FAILED BitArrMirror(): Got: %ld Expected: %ld\n",
               (long)got, (long)expected);
}

void TestBitArrToString(bitarr_t bit_arr, char* dest, const char* expected)
{
    BitArrToString(bit_arr, dest);

    if (0 == strcmp(dest, expected))
    {
        printf("PASSED: Got: %s Expected: %s\n", dest, expected);
    }
    else
    {
        printf("FAILED: Got: %s Expected: %s\n", dest, expected);
    }
}

/* ------------------ main (updated only where needed) ------------------ */

int main(void)
{
    bitarr_t value = (bitarr_t)0x0009;
    bitarr_t value2 = (bitarr_t)0x0000;
    size_t total_bits = TOTAL_BITS;

    char array[(sizeof(bitarr_t) * CHAR_BIT) + 1];
    char expected_str[(sizeof(bitarr_t) * CHAR_BIT) + 1];

    /* init arrays (C89) */
    memset(array, 0, sizeof(array));
    memset(expected_str, 0, sizeof(expected_str));

    TestBitArrSetAllOn(value);
    TestBitArrSetAllOff(value2);

    TestBitArrGetBit(value, 0, 1);
    TestBitArrGetBit(value, 1, 0);

    TestBitArrSetBit(0, 0, 1, 1);
    TestBitArrSetBit(0, 3, 1, (bitarr_t)1 << 3);
    TestBitArrSetBit(8, 7, 0, 8);
    TestBitArrSetBit(16, 4, 0, 0);

    TestBitArrFlipBit(0, 3, 8);
    TestBitArrFlipBit(8, 3, 0);

    TestBitArrFlipBit(0, 0, 1);
    TestBitArrFlipBit(1, 0, 0);

    TestBitArrFlipBit(10, 1, 8);
    TestBitArrFlipBit(10, 3, 2);

    TestBitArrCountOn(0, 0);
    TestBitArrCountOff(0, total_bits);

    TestBitArrCountOn(ALL_ONES, total_bits);
    TestBitArrCountOff(ALL_ONES, 0);

    TestBitArrCountOn((bitarr_t)1, 1);
    TestBitArrCountOff((bitarr_t)1, total_bits - 1);

    TestBitArrCountOn(3, 2);
    TestBitArrCountOn(5, 2);
    TestBitArrCountOn(10, 2);

    TestBitArrCountOff(3, total_bits - 2);
    TestBitArrCountOff(5, total_bits - 2);
    TestBitArrCountOff(10, total_bits - 2);

    TestBitArrCountOn(ALT_A, total_bits / 2);
    TestBitArrCountOff(ALT_A, total_bits / 2);
    TestBitArrCountOn(ALT_5, total_bits / 2);
    TestBitArrCountOff(ALT_5, total_bits / 2);

    /* Rotate Left */
    TestBitArrRotateLeft((bitarr_t)0x1,  (size_t)0x2, (bitarr_t)0x4);
    TestBitArrRotateLeft((bitarr_t)0x1,  (size_t)0x1, (bitarr_t)0x2);
    TestBitArrRotateLeft((bitarr_t)0xA5, (size_t)0x8, (bitarr_t)0xA500);

    /* Rotate Right */
    TestBitArrRotateRight((bitarr_t)0x2,  (size_t)0x1, (bitarr_t)0x1);
    TestBitArrRotateRight((bitarr_t)0x1,  (size_t)0x1,
                          (bitarr_t)0x1 << (TOTAL_BITS - 1));
    /* portable expected for both 32/64 */
    TestBitArrRotateRight((bitarr_t)0xA5, (size_t)0x8,
                          (bitarr_t)0xA5 << (TOTAL_BITS - 8));

    /* Mirror */
    TestBitArrMirror((bitarr_t)0x1,
                     (bitarr_t)0x1 << (TOTAL_BITS - 1));
    TestBitArrMirror((bitarr_t)0xF,
                     (bitarr_t)0xF << (TOTAL_BITS - 4));

    /* ToString: build expected string according to width, then use your test as-is */
    BuildExpectedBitString((bitarr_t)0x113, expected_str);
    TestBitArrToString((bitarr_t)0x113, array, expected_str);

    return 0;
}


