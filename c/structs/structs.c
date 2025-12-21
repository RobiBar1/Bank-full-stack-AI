#include <stdio.h>	  /* prints  */
#include <assert.h>	  /* asserts */
#include <string.h>   /* strcpy  */
#include <stdlib.h>   /* malloc  */

#define UNUSED(x) (void)(x);
#define JUNK 0
#define EO_STRING_CHAR 1
typedef enum {SUCCESS, ALOCATE_MEMORY_FAIL, GENERAL_FAIL}status_t;

typedef void (*print_func) (void*);
typedef status_t (*add_amount) (void**, int);
typedef status_t (*clean_up) (void*);


typedef struct generic_print_val
{
	void* val;
	print_func print;
	add_amount add;
	clean_up clean;
}gp_t ;

static void PrintInt(void* num);
static status_t AddInt(void** num1, int num2);
static status_t CleanInt(void* num);
/* --------------------------------- Int functions -------------------------- */

static void PrintInt(void* num)
{
	assert(NULL != num);
	
	printf("my num is: %d\n", *(int*)num);
}

static status_t AddInt(void** num1, int num2)
{
	assert(NULL != num1);
	assert(NULL != *num1);
	
    (**(int**)num1) += num2;
    
    return SUCCESS;
}

static status_t CleanInt(void* num)
{
	UNUSED(num);
	
	return SUCCESS;
}

static void IntInit(gp_t* gpt, int* num)
{	
	assert(NULL != gpt);
	assert(NULL != num);
	
	gpt->val = (void*)num;
	gpt->print = PrintInt;
	gpt->add = AddInt;
	gpt->clean = CleanInt;
}

static status_t TestInt(gp_t* gpt)
{
	status_t t = SUCCESS;
	
	assert(NULL != gpt);
	
	printf("-------------------- Start TestInt ------------------------------------\n");
	gpt->print((void*)gpt->val);
	gpt->add((void*)&gpt->val, 10);
	gpt->print((void*)gpt->val);
	t = gpt->clean(gpt->val);
	
	printf("TestInt end with %d\n", t);
	printf("-------------------- End TestInt ------------------------------------\n");
	printf("========================================================\n");
	
	return t;
	
}
/* ----------------------------- End Int functions -------------------------- */

/* --------------------------------- float functions ------------------------ */
static void PrintFloat(void* num)
{
	assert(NULL != num);	
	
	printf("my num is: %f\n", *(float*)num);
}

static status_t AddFloat(void** num1, int num2)
{
	assert(NULL != num1);
	assert(NULL != *num1);
	
    (**(float**)num1) += num2;
    
    return SUCCESS;
}

static status_t CleanFloat(void* num)
{
	UNUSED(num);
	
	return SUCCESS;
}

static void FloatInit(gp_t* gpt, float* num)
{	
	assert(NULL != gpt);
	assert(NULL != num);
	
	gpt->val = (void*)num;
	gpt->print = PrintFloat;
	gpt->add = AddFloat;
	gpt->clean = CleanFloat;
}

static status_t TestFloat(gp_t* gpt)
{
	status_t t = SUCCESS;
	
	assert(NULL != gpt);
	
	printf("------------------ Start TestFloat -------------------------------------------\n");
	gpt->print((void*)gpt->val);
	gpt->add((void*)&gpt->val, 10);
	gpt->print((void*)gpt->val);
	t = gpt->clean(gpt->val);
	printf("TestFloat end with %d\n", t);
	printf("-------------------- End TestFloat ------------------------------------\n");
	printf("========================================================\n");
	
	return t;
}
/*------------------------------ End float functions ----------------------- */

/* -------------------------------- String functions ------------------------ */
static void PrintString(void* str)
{
	assert(NULL != str);
	
	printf("my string is: %s\n", (char*)str);
}

static size_t CountDigits(int num)
{
	size_t o_count = 0;
	
	if (num < 0)
	{
		num *= -1;
	}
	
	while(num > 0)
	{
		++o_count;
		num /= 10;
	}
	
	return o_count;
}

static size_t Pow10ForGetLeftNumber(size_t num_of_digits)
{
	size_t pow10_to_return = 1;
	
	while (num_of_digits-- > 1)
	{
		pow10_to_return *= 10;
	}
	
	return pow10_to_return;
} 

static status_t AddString(void** str, int num)
{
	size_t str_size = JUNK, num_digits = JUNK, pow10_to_divide = JUNK;
	char* concat_arr = NULL;
	
	assert(NULL != str);
	assert(NULL != *str);
	
	str_size = strlen(*str);     
	num_digits = CountDigits(num);
	concat_arr = (char*)malloc(sizeof(char) * (str_size + num_digits + EO_STRING_CHAR));
	if (NULL == concat_arr)
	{
		return ALOCATE_MEMORY_FAIL;
	}

	strcpy(concat_arr, *str);
	pow10_to_divide = Pow10ForGetLeftNumber(num_digits);
	
	while (pow10_to_divide > 1)
	{
		
	}
	
	
}
/*

static status_t CleanString(void* str)
{
	assert(NULL != str);
	
	free(str); str = NULL;
	
	return SUCCESS;
}

static void StringInit(gp_t* gpt, float* num)
{	
	assert(NULL != gpt);
	assert(NULL != num);
	
	gpt->val = (void*)num;
	gpt->print = PrintFloat;
	gpt->add = AddFloat;
	gpt->clean = CleanFloat;
}

static void TestString(gp_t* gpt)
{
	status_t t = SUCCESS;
	
	assert(NULL != gpt);
	
	printf("-------------------- Start TestString ------------------------------------\n");
	gpt->print((void*)gpt->val);
	gpt->add((void*)&gpt->val, 10);
	gpt->print((void*)gpt->val);
	t = gpt->clean(gpt->val);
	printf("TestString end with %d\n", t);
	printf("-------------------- End TestString ------------------------------------\n");
	printf("========================================================\n");
	
	return t;
}
 ------------------------------ End String functions ----------------------- */

int main()
{
    int y = 40;
    float y_float = 20.0;
    char* y_string = NULL;
    
    int* ptr = &y;
    float* ptr_float = &y_float;		
	gp_t x_int;
	gp_t x_float;
	
	y_string = (char*)malloc(sizeof(char) * 25);
	if (NULL == y_string)
	{
		printf("y_string malloc fail");
		return 1;
	}
	
	strcpy(y_string, "errors is good practice.");
	PrintString((void*)y_string);
	
	IntInit(&x_int, ptr);
	if (SUCCESS != TestInt(&x_int))
	{
		return 1;
	}
	
	FloatInit(&x_float, ptr_float);
	if (SUCCESS != TestFloat(&x_float))
	{
		return 1;
	}
	
	
		
	return 0;
}
