#include <stdio.h>

#include "vector.h"

#define COLOR_RED "\x1b[31m"
#define COLOR_GREEN "\x1b[32m"
#define COLOR_YELLOW "\x1b[33m"
#define COLOR_BLUE "\x1b[34m"
#define COLOR_RESET "\x1b[0m"


static void RunTestsVectorCreation()
{
	printf(COLOR_BLUE "Run tests for vector creation\n" COLOR_RESET);


	printf("Creating and destroying vector\n");
	VectorDestroy(VectorCreate(42, sizeof(int)));

	printf(COLOR_BLUE "Finished tests\n" COLOR_RESET);
}

static void RunTestsPushPop()
{
	size_t capacity = 4;
	int values[4] = {1, 2, 3, 4};
	size_t i = 0;
	vector_t* vector = NULL;
	size_t size = 0;

	printf(COLOR_BLUE "Run tests for vector creation\n" COLOR_RESET);

	printf("Creating vector\n");
	vector = VectorCreate(capacity, sizeof(int));

	if (0 != VectorGetSize(vector))
	{
		printf(COLOR_RED "FAIL: Vector is not empty!\n" COLOR_RESET);
	}
	else
	{
		printf(COLOR_GREEN "SUCCESS: Vector is empty\n" COLOR_RESET);
	}

	for (i = 0; i < capacity; ++i)
	{
		printf("Pushing %d\n", values[i]);
		VectorPushBack(vector, values + i);
	}

	size = VectorGetSize(vector);
	if (size != i)
	{
		printf(COLOR_RED "FAIL: Vector is of wrong size! Got %lu, expected %lu\n" COLOR_RESET,
			size, i);
	}
	else
	{
		printf(COLOR_GREEN "SUCCESS: Vector is of size %ld\n" COLOR_RESET, i);
	}

	for (i = 4; i > 0; --i)
	{
		printf("Popping %d\n", values[i - 1]);
		VectorPopBack(vector);
	}

	size = VectorGetSize(vector);
	if (i != size)
	{
		printf(COLOR_RED "FAIL: Vector is of wrong size! Got %lu, expected %lu\n" COLOR_RESET,
			size, i - 1);
	}
	else
	{
		printf(COLOR_GREEN "SUCCESS: Vector is empty\n" COLOR_RESET);
	}

	VectorDestroy(vector);

	printf(COLOR_BLUE "Finished tests\n" COLOR_RESET);
}

static void RunTestsVectorReserve()
{
	vector_t* vector = NULL;
	size_t vector_capacity = 0;

	printf(COLOR_BLUE "Run tests for vectorReserve\n" COLOR_RESET);

	vector = VectorCreate(1, sizeof(int));

	printf("Reserving 10 spaces\n");
	VectorReserve(vector, 10);
	vector_capacity = VectorGetCapacity(vector);
	if (vector_capacity != 10)
	{
		printf(COLOR_RED "FAIL: Vector capacity is wrong! Got %lu, expected %lu\n" COLOR_RESET,
			vector_capacity, 10lu);
	}
	else
	{
		printf(COLOR_GREEN "SUCCESS: Vector capacity is %lu\n" COLOR_RESET, vector_capacity);
	}

	printf("Reserving 1 space\n");
	VectorReserve(vector, 1);
	vector_capacity = VectorGetCapacity(vector);
	if (vector_capacity != 10)
	{
		printf(COLOR_RED "FAIL: Vector capacity is wrong! Got %lu, expected %lu\n" COLOR_RESET,
			vector_capacity, 10lu);
	}
	else
	{
		printf(COLOR_GREEN "SUCCESS: Vector capacity is %lu\n" COLOR_RESET, vector_capacity);
	}

	VectorDestroy(vector);

	printf(COLOR_BLUE "Finished tests\n" COLOR_RESET);
}

static void RunTestsVectorGrowth()
{
	vector_t* vector = NULL;
	int value = 0;
	size_t capacity = 4;
	size_t vector_capacity = 0;
	size_t i = 0;

	printf(COLOR_BLUE "Run tests for Vector growth\n" COLOR_RESET);

	vector = VectorCreate(capacity, sizeof(int));

	vector_capacity = VectorGetCapacity(vector);
	if (vector_capacity != capacity)
	{
		printf(COLOR_RED "FAIL: Vector capacity is wrong! Got %lu, expected %lu\n" COLOR_RESET,
			vector_capacity, capacity);
	}
	else
	{
		printf(COLOR_GREEN "SUCCESS: Vector capacity is %lu\n" COLOR_RESET, capacity);
	}

	printf("Pushing elements\n");
	for (i = 0; i <= capacity; ++i)
	{
		VectorPushBack(vector, &value);
	}

	capacity *= 2;
	vector_capacity = VectorGetCapacity(vector);
	if (vector_capacity != capacity)
	{
		printf(COLOR_RED "FAIL: Vector capacity is wrong! Got %lu, expected %lu\n" COLOR_RESET,
			vector_capacity, capacity);
	}
	else
	{
		printf(COLOR_GREEN "SUCCESS: Vector capacity is %lu\n" COLOR_RESET, capacity);
	}

	printf("Popping elements\n");
	for (i = 0; i <= VectorGetSize(vector); ++i)
	{
		VectorPopBack(vector);
	}

	vector_capacity = VectorGetCapacity(vector);
	if (vector_capacity != capacity)
	{
		printf(COLOR_RED "FAIL: Vector capacity is wrong! Got %lu, expected %lu\n" COLOR_RESET,
			vector_capacity, capacity);
	}
	else
	{
		printf(COLOR_GREEN "SUCCESS: Vector capacity is %lu\n" COLOR_RESET, capacity);
	}


	VectorDestroy(vector);

	printf(COLOR_BLUE "Finished tests\n" COLOR_RESET);

}

static void RunTestsVectorShrinkage()
{
	vector_t* vector = NULL;
	int value = 0;
	size_t current_capacity = 0;

	printf(COLOR_BLUE "Run Tests for vector shrinkage\n" COLOR_RESET);

	vector = VectorCreate(4, sizeof(int));

	printf("Pushing element\n");
	VectorPushBack(vector, &value);

	printf("Shrinking vector\n");
	VectorShrink(vector);

	current_capacity = VectorGetCapacity(vector);
	if (3 != current_capacity)
	{
		printf(COLOR_RED "FAIL: Vector is not of right capacity! Got %lu, expected 3\n" COLOR_RESET,
			current_capacity);
	}
	else
	{
		printf(COLOR_GREEN "SUCCESS: Vector capacity is 3\n" COLOR_RESET);
	}

	printf("Pushing element\n");
	VectorPushBack(vector, &value);

	printf("Shrinking vector\n");
	VectorShrink(vector);

	current_capacity = VectorGetCapacity(vector);
	if (3 != current_capacity)
	{
		printf(COLOR_RED "FAIL: Vector is not of right capacity! Got %lu, expected 3\n" COLOR_RESET,
			current_capacity);
	}
	else
	{
		printf(COLOR_GREEN "SUCCESS: Vector capacity is 3\n" COLOR_RESET);
	}

	VectorDestroy(vector);

	printf(COLOR_BLUE "Finished Tests\n" COLOR_RESET);
}

static void RunTestsVectorAccess()
{
	vector_t* vector = NULL;
	size_t size = 5;
	size_t values[] = {0, 1, 2, 3, 4};
	size_t i = 0;
	size_t* ptr = NULL;

	printf(COLOR_BLUE "Run tests for vector shrinkage\n" COLOR_RESET);

	vector = VectorCreate(size, sizeof(long));

	printf("Pushing elements\n");
	for (i = 0; i < size; ++i)
	{
		VectorPushBack(vector, values + i);
	}

	printf("Accessing elements\n");
	for (i = 0; i < size; i++)
	{
		ptr = VectorGetAccess(vector, i);
		if (*ptr != values[i])
		{
			printf(COLOR_RED "FAIL: Got different element! Got %lu, expected %lu\n" COLOR_RESET,
				*ptr, values[i]);
		}
		else
		{
			printf(COLOR_GREEN "SUCCESS: Got %lu\n" COLOR_RESET, *ptr);
		}
	}

	printf("Modifying elements\n");
	for (i = 0; i < size; i++)
	{
		ptr = VectorGetAccess(vector, i);
		*ptr += 1;
	}

	printf("Accessing elements after modifying\n");
	for (i = 0; i < size; i++)
	{
		ptr = VectorGetAccess(vector, i);
		if (*ptr != values[i] + 1)
		{
			printf(COLOR_RED "FAIL: Got different element! Got %lu, expected %lu\n" COLOR_RESET,
				*ptr, values[i] + 1);
		}
		else
		{
			printf(COLOR_GREEN "SUCCESS: Got %lu\n" COLOR_RESET, *ptr);
		}
	}

	VectorDestroy(vector);

	printf(COLOR_BLUE "Finished Tests\n" COLOR_RESET);
}

int main()
{
	RunTestsVectorCreation();
	RunTestsPushPop();
	RunTestsVectorReserve();
	RunTestsVectorGrowth();
	RunTestsVectorShrinkage();
	RunTestsVectorAccess();

	return 0;
}

