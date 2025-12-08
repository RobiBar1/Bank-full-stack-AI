#include <assert.h> /* for using asserts */
#include <stddef.h> /* for using size_t */
#include <stdio.h>  /* for using printf*/
#include <stdlib.h> /* Required for malloc and free */

void SwapInt(int* x, int* y)
{
	int tmp;
	
	assert(NULL != x && NULL != y);
	
	tmp = *x;
	*x = *y;
	*y = tmp; 
}

void CopyArray(const int* array, int* dest_array, size_t array_len)
{
	size_t i;
	
	assert(NULL != array && NULL != dest_array);

	for (i = 0; i < array_len; ++i)
	{
		dest_array[i] = array[i];
	}
}

void PrintTypesAddress(void)
{
	static int s_i = 7;
	int* ptr = &s_i;
	int i = 7;
	int* ptr2 = &i;
	int** ptr3 = &ptr2;
	int* ptr4 = (int*)malloc(sizeof(int));
	
	assert(NULL != ptr4);
	
	printf("DATA SEGMENT (Static Variables):\n");
	printf("Address of s_i:        %p\n", (void*)&s_i);
	printf("\n");

	printf("HEAP (Dynamic Memory):\n");
	printf("Value inside ptr4:     %p (Points to location in Heap)\n", (void*)ptr4);
	printf("\n");

	printf("STACK (Local Variables):\n");
	printf("Address of ptr:        %p\n", (void*)&ptr);
	printf("Address of i:          %p\n", (void*)&i);
	printf("Address of ptr2:       %p\n", (void*)&ptr2);
	printf("Address of ptr3:       %p\n", (void*)&ptr3);
	printf("Address of ptr4:       %p (The pointer variable itself)\n", (void*)&ptr4);

	if (ptr4 != NULL)
	{
		free(ptr4);
		ptr4 = NULL;
	}
}

void SwapTwoSize_t(size_t* one, size_t* other)
{
	size_t tmp;
	
	assert (NULL != one && NULL != other);

	tmp = *one;
	*one = *other;
	*other = tmp; 
}

void SwapTwoPtrSize_t(size_t** one, size_t** other)
{	
	SwapTwoSize_t((size_t*) one, (size_t*) other);
}



