#include <stddef.h>   /* size_t */
#include <stdio.h>   /* printf */
#include <assert.h> /* asserting */

#define TRUE 1 
#define FALSE 0
#define JUNK 10
#define UNUSED(x) void(x)
#define BIT2_AND_BIT6 34
#define ones_zeros_mask 0x55555555
#define pairs_one_zeros 0x33333333
#define for_groups_one_zeros 0x0F0F0F0F
#define eight_groups_one_zeros 0x00FF00FF
#define sixteen_groups_one_zeros 0x3F
#define mask_one ((unsigned int)1)
#define MASK_ODD_BITS  0xAA
#define MASK_PAIR_LEFT  0xCC
#define MASK_NIBBLE_UPPER 0xF0 

long Pow2(unsigned int x, unsigned int y)
{
	x = x << y;
	
	return x;
}

int IsPower2Loop(unsigned int n)
{
	int count = 0;
	
	if (0 == n)
	{
		return 0;
	}
	
    while (n > 0) 
    {
        if (n & 1) 
        {
            ++count;
        }
        
        n = n >> 1;
    }
    
    return (1 == count) ? TRUE : FALSE;
}

int IsPower2NoLoop(unsigned int n)
{
	return (n != 0) && (0 == (n & (n - 1)));   
}

int IncrementByOne(int n) /* no overflow protection */
{
	int m = 1;

    while (n & m)
    {
        n = n ^ m; 
        m = m << 1;
    }

    n = n ^ m;
    
    return n;
}

void Print3BitsOn(unsigned int* num_arr, size_t size)
{
	size_t i = 0, count = JUNK;
	unsigned int n = 0, temp = JUNK;
	
	assert(NULL != num_arr);
		
	for (; i < size; ++num_arr, ++i) 
	{
        n = *num_arr;
		count = 0;
		temp = n;
        
        while (temp > 0)
        {
        	/* Brian algoritem for run n time while n = numbres of 1's */
            temp = temp & (temp - 1); 
            ++count;
        }

        if (3 == count) 
        {
            printf("%u\n", n);
        }
    }
}

unsigned char ByteMirrorLoop(unsigned char n)
{
	unsigned char reversed = 0;
	size_t i = 0;
	
    for (; i < 8; ++i)
    {
        if ((n >> i) & 1)
        {      
        	reversed |= (1 << (7 - i));
        }
    }
    
    return reversed;
}

unsigned char ByteMirrorNoLoop(unsigned char byte)
{
    byte = ((byte & MASK_NIBBLE_UPPER) >> 4) | ((byte & for_groups_one_zeros) << 4);
	byte = ((byte & MASK_PAIR_LEFT) >> 2) | ((byte & pairs_one_zeros) << 2);
	byte = ((byte & MASK_ODD_BITS) >> 1) | ((byte & ones_zeros_mask) << 1);
    
    return byte;
}

int Is2And6BitBothOn(unsigned char byte)
{
	unsigned char mask = BIT2_AND_BIT6;
	return (byte & mask) == mask;
}

int Is2And6BitAtLeastOneOn(unsigned char byte)
{
	unsigned char mask_2 = 2;
	unsigned char mask_32 = 32;
	return mask_2 & byte || mask_32 & byte;
}

unsigned char Swap3And5Bit(unsigned char byte)
{
	unsigned char mask_3 = 4;
	unsigned char mask_5 = 16;
	
	unsigned char temp_3 = JUNK; 
	unsigned char temp_5 = JUNK;
	unsigned char temp_3_not = JUNK;
	unsigned char temp_5_not = JUNK;
	
	temp_3 = mask_3 & byte;
	temp_5 = mask_5 & byte;
	
	temp_3_not = ~temp_3;
	temp_5_not = ~temp_5;
	
	byte &= temp_3_not;
	byte &= temp_5_not;
	
	temp_3 = temp_3 << 2;
	temp_5 = temp_5 >> 2;
	
	byte |= temp_3;
	byte |= temp_5;
	
	return byte;
}
unsigned int ClosestDivisible16(unsigned int num)
{
	unsigned int mask = 15;
	return ~mask & num; 
}

void Swap(int* one, int* other) /* no temp variable */ /** make 3 different variations **/
{
	assert(NULL != one);
	assert(NULL != other);
	
	/* XOR with him self is allways 0 so it wont work for that case */
	if (one == other)
	{
		return;
	}
	
	*one = *one ^ *other;
    *other = *one ^ *other;
    *one = *one ^ *other;
}
void Swap2(int* one, int* other) /* no temp variable */
{
	assert(NULL != one);
	assert(NULL != other);
	
	*one += *other;
	*other = *one - *other; 
	*one -= *other;
}
void Swap3(int* one, int* other) /* no temp variable */
{   
    assert(NULL != one);
	assert(NULL != other);
	
	*one -= *other;
	*other = *one + *other;	
	*one = *other - *one;
}

size_t CountBitsLoop(int n)
{
	size_t count = 0;
	
    while (n > 0) 
    {
        if (n & mask_one) 
        {
            count++;
        }
        
        n = n >> mask_one;
    }
    
    return count;
}

/*=====================================================
 * 0x55555555 = 01010101.. (Selects every other bit).
 * 0x33333333 = 00110011.. (Selects pairs of bits).
 * 0x0F0F0F0F = 00001111.. (Selects groups of 4 bits).
 * ...
 *=====================================================*/
size_t CountBitsNoLoop(int n)
{
	/* using Hamming Weight/Population Count (2 names for same same algo) */
    n = (n & ones_zeros_mask) + ((n >> 1) & ones_zeros_mask);
	n = (n & pairs_one_zeros) + ((n >> 2) & pairs_one_zeros);
	n = (n + (n >> 4)) & for_groups_one_zeros;
	n = (n + (n >> 8)) & eight_groups_one_zeros;
    return (n + (n >> 16)) & sixteen_groups_one_zeros;
}
void PrintFloatBitwise(float f)
{
    unsigned int n = *((unsigned int*)&f), bit = JUNK;
	int i = 31;
	
    printf("Original Float: %f\n", f);
    printf("Binary Layout:  ");

    for (; i >= 0; --i) 
    {
        bit = (n >> i) & 1;
        printf("%u", bit);

        if (31 == i) printf(" "); 
        if (23 == i) printf(" "); 
    }
    
    printf("\n");
    
    printf("Legend:         S EEEEEEEE MMMMMMMMMMMMMMMMMMMMMMM\n");
    printf("                ^ ^        ^\n");
    printf("             Sign Exponent Mantissa (Fraction)\n\n");
}




