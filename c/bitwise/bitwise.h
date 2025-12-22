#ifndef __BITWISE_H__
#define __BITWISE_H__

#include <stddef.h> /*size_t */

long Pow2(unsigned int x, unsigned int y);
int IsPower2Loop(unsigned int n);
int IsPower2NoLoop(unsigned int n);
int IncrementByOne(int num); /* no overflow protection */
void Print3BitsOn(unsigned int* num_arr, size_t size);
unsigned char ByteMirrorLoop(unsigned char byte);
unsigned char ByteMirrorNoLoop(unsigned char byte);
int Is2And6BitBothOn(unsigned char byte);
int Is2And6BitAtLeastOneOn(unsigned char byte);
unsigned char Swap3And5Bit(unsigned char byte);
unsigned int ClosestDivisible16(unsigned int num);
void Swap(int* one, int* other); /* no temp variable */ /** make 3 different variations **/
void Swap2(int* one, int* other); /* no temp variable */
void Swap3(int* one, int* other); /* no temp variable */
size_t CountBitsLoop(int num);
size_t CountBitsNoLoop(int num);
void PrintFloatBitwise(float fl);

#endif /*__BITWISE_H__*/ 
