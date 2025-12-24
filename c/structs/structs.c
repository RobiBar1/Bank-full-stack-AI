#include <stdio.h>	  /* prints  */
#include <assert.h>	  /* asserts */
#include <string.h>   /* strcpy  */
#include <stdlib.h>   /* malloc  */

#define MAX2(a, b)       ((a) > (b) ? (a) : (b))
#define MAX3(a, b, c)    (MAX2(MAX2(a, b), c))
#define SIZEOF_VAR(var)  ((size_t)((char*)(&(var) + 1) - (char*)(&(var))))
#define SIZEOF_TYPE(type)  ((size_t)((type*)0 + 1))
#define UNUSED(x) (void)(x);
#define JUNK 0
#define EO_STRING_CHAR 1
typedef enum {SUCCESS, ALOCATE_MEMORY_FAIL, GENERAL_FAIL} status_t;

typedef void (*print_func) (void*);
typedef status_t (*add_amount) (void**, int);
typedef status_t (*clean_up) (void*);

typedef struct
{
	float mana_price;
	float falafel_balls;
} pita;

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

static status_t TestInt()
{
	status_t t = SUCCESS;
	int y = 40;
	int* ptr = &y;
	gp_t x_int_nptr;
	gp_t* gpt = &x_int_nptr;
	
	assert(NULL != gpt);
	
	printf("-------------------- Start TestInt ------------------------------------\n");
	
	IntInit(&x_int_nptr, ptr);
	
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

static status_t TestFloat()
{
	status_t t = SUCCESS;
	float y_float = 20.0;
    float* ptr_float = &y_float;		
	gp_t x_float_nptr;
	gp_t* gpt = &x_float_nptr;
	
	assert(NULL != gpt);
	
	printf("------------------ Start TestFloat -------------------------------------------\n");
	FloatInit(&x_float_nptr, ptr_float);
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
	size_t str_size = JUNK, num_digits = JUNK, pow10_to_divide = JUNK,
		   num_to_add_to_string = JUNK, index_to_enter_num = JUNK;
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
		
	concat_arr = strcpy(concat_arr, *str);
	pow10_to_divide = Pow10ForGetLeftNumber(num_digits);
	index_to_enter_num = str_size;
	while (pow10_to_divide >= 1)
	{
		num_to_add_to_string = num / pow10_to_divide;
		concat_arr[index_to_enter_num++] = (char)(num_to_add_to_string + '0');
		num -= (pow10_to_divide * num_to_add_to_string);
		pow10_to_divide /= 10;
	}
	
	concat_arr[index_to_enter_num] = '\0';
	free(*str); *str = concat_arr;
	
	return SUCCESS;
}


static status_t CleanString(void* str)
{
	assert(NULL != str);
	
	free(str); str = NULL;
	
	return SUCCESS;
}

static void StringInit(gp_t* gpt, char* str)
{	
	assert(NULL != gpt);
	assert(NULL != str);
	
	gpt->val = (void*)str;
	gpt->print = PrintString;
	gpt->add = AddString;
	gpt->clean = CleanString;
}

static status_t TestString()
{
	status_t t = SUCCESS;
	char* y_string = NULL;
	gp_t x_string_nptr = {JUNK};
	gp_t* gpt = &x_string_nptr;
	
	printf("-------------------- Start TestString ------------------------------------\n");
	y_string = (char*)malloc(sizeof(char) * 25);
	if (NULL == y_string)
	{
		printf("y_string malloc fail");
		
		return ALOCATE_MEMORY_FAIL;
	}
	
	strcpy(y_string, "errors is good practice.");
	PrintString((void*)y_string);
	
	AddString((void**)&y_string, 123);
	
	PrintString((void*)y_string);
	
    StringInit(gpt, y_string);
	gpt->print((void*)gpt->val);
	gpt->add((void*)&gpt->val, 10);
	gpt->print((void*)gpt->val);
	t = gpt->clean(gpt->val);	
	
	printf("TestString end with %d\n", t);
	printf("========================================================\n");
	printf("-------------------- End TestString ------------------------------------\n");
	
	return t;
}
 /* ------------------------------ End String functions ----------------------- */

/* ------------------------------ Start pita functions ----------------------- */

static void PrintPita(void* gpt_pita)
{
	assert(NULL != gpt_pita);
	
	printf("pita first number is: %f, and second number is: %f\n",
	 ((pita*)gpt_pita)->mana_price, ((pita*)gpt_pita)->falafel_balls);
}

static status_t AddPita(void** gpt_pita, int num2)
{
	assert(NULL != gpt_pita);
	assert(NULL != *gpt_pita);
	
    (*(pita**)gpt_pita)-> mana_price += num2;
    (*(pita**)gpt_pita)-> falafel_balls += num2;
    
    return SUCCESS;
}

static status_t CleanPita(void* num)
{
	UNUSED(num);
	
	return SUCCESS;
}

static void GptPitaInit(gp_t* gpt, pita* init_pita)
{	
	assert(NULL != gpt);
	assert(NULL != init_pita);
	
	gpt->val = (void*)init_pita;
	gpt->print = PrintPita;
	gpt->add = AddPita;
	gpt->clean = CleanPita;
}
static void InitPita(pita* x_pita, float mana_price, float falafel_balls)
{
	x_pita->mana_price = mana_price;
	x_pita->falafel_balls = falafel_balls;
}

static status_t TestPita()
{
	status_t t = SUCCESS;
	pita x_pita = {JUNK};
	float mana_price = 5.0, falafel_balls = 3.5;
	gp_t x_gpt_pita = {JUNK};
	gp_t* gpt = &x_gpt_pita;
	
	printf("-------------------- Start TestPita ------------------------------------\n");
	
	InitPita(&x_pita, mana_price, falafel_balls);
	
	GptPitaInit(&x_gpt_pita, &x_pita);
	
	gpt->print((void*)gpt->val);
	gpt->add((void*)&gpt->val, 10);
	gpt->print((void*)gpt->val);
	t = gpt->clean(gpt->val);
	printf("TestInt end with %d\n", t);
	printf("-------------------- End TestPita ------------------------------------\n");
	printf("========================================================\n");
	
	return t;
	
}
/* ------------------------------ End pita functions ----------------------- */

static void TestMacros()
{
	int x = 5;
	double y = 8.0;
	
	printf("-------------------- Start TestMacros ------------------------------------\n");
	printf("max2 (5, 2) is: %d\n", MAX2(5, 2));
	printf("max3 (5, 2, 8) is: %d\n", MAX3(5, 2, 8));
	printf("sizeof_Var int is: %lu\n", SIZEOF_VAR(x));
	printf("sizeof_Var double is: %lu\n", SIZEOF_VAR(y));
	printf("SIZEOF_TYPE float is: %lu\n", SIZEOF_TYPE(float));
	printf("SIZEOF_TYPE char is: %lu\n", SIZEOF_TYPE(char));
	printf("TestInt end with 0\n");
	printf("-------------------- End TestMacros ------------------------------------\n");
	printf("========================================================\n");
}
int main()
{	
	if (SUCCESS != TestInt())
	{
		return 1;
	}
	if (SUCCESS != TestFloat())
	{
		return 1;
	}
	if (SUCCESS != TestString())
	{
		return 1;
	}
	if(SUCCESS != TestPita())
	{
		return 1;
	}
	
	TestMacros();
	
	return 0;
}
