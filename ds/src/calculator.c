/*
Writer: Robi
Chcker: Hen
Date:   09.02.2026
*/
#include <string.h> 	   /* strlen  	    */
#include <stddef.h> 	   /* size_t 	    */
#include <assert.h>  	   /* assert 	    */
#include <errno.h>  	   /* errno  	    */
#include <stdlib.h> 	   /* strtod, free  */

#include "stack.h"  	   /* StackCreate   */
#include "calculator.h"    /* our api 		*/

#define UNUSED(x) (void)(x)

/*------------------------- Enum - typedef declaretions -----------------------*/
typedef enum {WAIT_FOR_NUM, WAIT_FOR_OP, ERROR, NUM_OF_STATES} state_t;
typedef enum 
{
    CHAR_DIGIT,       
    CHAR_PLUS,
    CHAR_MINUS,
    CHAR_MULT,
    CHAR_DIV,
    CHAR_POW,
    CHAR_OPEN_PAREN,  
    CHAR_CLOSE_PAREN, 
    CHAR_SPACE,       
    CHAR_END,         
    CHAR_INVALID,     
    NUM_CHAR_TYPES
} char_type_t;
/*==========================================================================*/


/*--------------------- func pointers typedef declaretions ----------------*/
typedef status_t (*handler_func_t)(const char**);
typedef double (*calc_func_t)(double, double);
/*==========================================================================*/


/*------------------------- Struct declaretions -------------------------------*/
typedef struct
{
	state_t next_state;
	handler_func_t handler;
}transition_t;

typedef struct 
{
    int in_stack_prec;
    int out_stack_prec;
    calc_func_t calc_func;
} op_info_t;
/*==========================================================================*/


/*-------------------------------- LUTS declaretions -------------------------*/
static char_type_t g_char_type_lut[256] = {0};
static transition_t g_transition_lut[NUM_OF_STATES][NUM_CHAR_TYPES] = {0};
static op_info_t g_op_info_lut[256] = {0};
/*==========================================================================*/


/*--------------------------- Global declaretions -----------------------------*/
static stack_t* g_num_stack = NULL;
static stack_t* g_op_stack = NULL;
static state_t g_current_state = WAIT_FOR_NUM;
static status_t g_error_status = SUCCESS;
static int g_luts_initialized = 0;
static int g_paren_count = 0;
static int g_unary_flag = 0;
static double g_unary_mult = 1.0;
/*==========================================================================*/

/*---------------------------- Functions declare-------------------------------*/
static void ResetUnaryState(void);
static double Add(double a, double b);
static double Sub(double a, double b);
static double Mult(double a, double b);
static double Div(double a, double b);
static double Pow(double base, double exp);
static double DuumyFunc(double a, double b);
static status_t HandleNum(const char** input);
static status_t HandleNum(const char** input);
static status_t HandleUnaryPlus(const char** input);
static status_t HandleUnaryPlus(const char** input);
static status_t HandleUnaryMinus(const char** input);
static status_t HandleUnaryMinus(const char** input);
static status_t HandleOperator(const char** input);
static status_t HandleOpenParen(const char** input);
static status_t HandleCloseParen(const char** input);
static status_t HandleSpace(const char** input);
static status_t HandleEnd(const char** input);
static status_t HandleError(const char** input);
static status_t ComputeTopOperator(void);
static void PeekAndPop(stack_t* stack, void* val);
/*==========================================================================*/


/*------------------------------- Init-Functions ---------------------------*/

static status_t InitProgram(const char* expression)
{
    size_t expression_size = 0;
    
    assert (NULL != expression);
    
    expression_size = (strlen(expression) / 2) + 1;
    g_num_stack = StackCreate(expression_size, sizeof(double));
    if (NULL == g_num_stack)
    {
        return ALLOC_FAIL;
    }
    
    g_op_stack = StackCreate(expression_size, sizeof(char));
    if (NULL == g_op_stack)
    {
        StackDestroy(g_num_stack );
        g_num_stack = NULL;
        return ALLOC_FAIL;
    }
    
    g_current_state = WAIT_FOR_NUM;
    g_error_status = SUCCESS;
    g_paren_count = 0;
    ResetUnaryState();
    
    return SUCCESS;
}

static void InitOpInfoLut(void)
{
    int i = 0;
    
    for (i = 0; i < 256; ++i)
    {
        g_op_info_lut[i].in_stack_prec = 0;
        g_op_info_lut[i].out_stack_prec = 0;
        g_op_info_lut[i].calc_func = DuumyFunc;
    }
    
    g_op_info_lut['+'].in_stack_prec = 10;
    g_op_info_lut['+'].out_stack_prec = 10;
    g_op_info_lut['+'].calc_func = Add;
    
    g_op_info_lut['-'].in_stack_prec = 10;
    g_op_info_lut['-'].out_stack_prec = 10;
    g_op_info_lut['-'].calc_func = Sub;
    
    g_op_info_lut['*'].in_stack_prec = 20;
    g_op_info_lut['*'].out_stack_prec = 20;
    g_op_info_lut['*'].calc_func = Mult;
    
    g_op_info_lut['/'].in_stack_prec = 20;
    g_op_info_lut['/'].out_stack_prec = 20;
    g_op_info_lut['/'].calc_func = Div;
    
    g_op_info_lut['^'].in_stack_prec = 30;
    g_op_info_lut['^'].out_stack_prec = 31;
    g_op_info_lut['^'].calc_func = Pow;
    
    g_op_info_lut['('].in_stack_prec = 0;
    g_op_info_lut['('].out_stack_prec = 99;
    g_op_info_lut['('].calc_func = DuumyFunc;
}

static void InitTransitionLut(void)
{
    int i = 0;
    int j = 0;
    
    for (; i < NUM_OF_STATES; ++i)
    {
        for (j = 0; j < NUM_CHAR_TYPES; ++j)
        {
            g_transition_lut[i][j].next_state = ERROR;
            g_transition_lut[i][j].handler = HandleError;
        }
    }
    
    /*WAIT_FOR_NUM states*/
    g_transition_lut[WAIT_FOR_NUM][CHAR_DIGIT].next_state = WAIT_FOR_OP;
    g_transition_lut[WAIT_FOR_NUM][CHAR_DIGIT].handler = HandleNum;
    
    g_transition_lut[WAIT_FOR_NUM][CHAR_PLUS].next_state = WAIT_FOR_NUM;
    g_transition_lut[WAIT_FOR_NUM][CHAR_PLUS].handler = HandleUnaryPlus;
    
    g_transition_lut[WAIT_FOR_NUM][CHAR_MINUS].next_state = WAIT_FOR_NUM;
    g_transition_lut[WAIT_FOR_NUM][CHAR_MINUS].handler = HandleUnaryMinus;
    
    g_transition_lut[WAIT_FOR_NUM][CHAR_OPEN_PAREN].next_state = WAIT_FOR_NUM;
    g_transition_lut[WAIT_FOR_NUM][CHAR_OPEN_PAREN].handler = HandleOpenParen;
    
    g_transition_lut[WAIT_FOR_NUM][CHAR_SPACE].next_state = WAIT_FOR_NUM;
    g_transition_lut[WAIT_FOR_NUM][CHAR_SPACE].handler = HandleSpace;
    
    /*WAIT_FOR_OP states*/
    g_transition_lut[WAIT_FOR_OP][CHAR_PLUS].next_state = WAIT_FOR_NUM;
    g_transition_lut[WAIT_FOR_OP][CHAR_PLUS].handler = HandleOperator;
    
    g_transition_lut[WAIT_FOR_OP][CHAR_MINUS].next_state = WAIT_FOR_NUM;
    g_transition_lut[WAIT_FOR_OP][CHAR_MINUS].handler = HandleOperator;
    
    g_transition_lut[WAIT_FOR_OP][CHAR_MULT].next_state = WAIT_FOR_NUM;
    g_transition_lut[WAIT_FOR_OP][CHAR_MULT].handler = HandleOperator;
    
    g_transition_lut[WAIT_FOR_OP][CHAR_DIV].next_state = WAIT_FOR_NUM;
    g_transition_lut[WAIT_FOR_OP][CHAR_DIV].handler = HandleOperator;
    
    g_transition_lut[WAIT_FOR_OP][CHAR_POW].next_state = WAIT_FOR_NUM;
    g_transition_lut[WAIT_FOR_OP][CHAR_POW].handler = HandleOperator;
    
    g_transition_lut[WAIT_FOR_OP][CHAR_CLOSE_PAREN].next_state = WAIT_FOR_OP;
    g_transition_lut[WAIT_FOR_OP][CHAR_CLOSE_PAREN].handler = HandleCloseParen;
    
    g_transition_lut[WAIT_FOR_OP][CHAR_SPACE].next_state = WAIT_FOR_OP;
    g_transition_lut[WAIT_FOR_OP][CHAR_SPACE].handler = HandleSpace;
    
    g_transition_lut[WAIT_FOR_OP][CHAR_END].next_state = WAIT_FOR_OP;
    g_transition_lut[WAIT_FOR_OP][CHAR_END].handler = HandleEnd;
}

static void InitCharTypeLut(void)
{
    int i = 0;
    
    for (; i < 256; ++i)
    {
        g_char_type_lut[i] = CHAR_INVALID;
    }
    
    for (i = '0'; i <= '9'; ++i)
    {
        g_char_type_lut[i] = CHAR_DIGIT;
    }
    
    g_char_type_lut['.'] = CHAR_DIGIT;
    
    g_char_type_lut['+'] = CHAR_PLUS;
    g_char_type_lut['-'] = CHAR_MINUS;
    g_char_type_lut['*'] = CHAR_MULT;
    g_char_type_lut['/'] = CHAR_DIV;
    g_char_type_lut['^'] = CHAR_POW;
    
    g_char_type_lut['('] = CHAR_OPEN_PAREN;
    g_char_type_lut[')'] = CHAR_CLOSE_PAREN;
    
    g_char_type_lut[' '] = CHAR_SPACE;
    g_char_type_lut['\0'] = CHAR_END;
    g_char_type_lut['\n'] = CHAR_END;
    g_char_type_lut['='] = CHAR_END;
}



static void InitAllLuts(void)
{
    InitCharTypeLut();
    InitTransitionLut();
    InitOpInfoLut();
    g_luts_initialized = 1;
}
/*==========================================================================*/


/* ----------------------------- Helper functions --------------------------*/
static status_t FlushStacks(double* result)
{
    assert (NULL != result);
    
    if (0 != g_paren_count)
    {
        return INVALID_EXPRESSION;
    }
    
    while (!StackIsEmpty(g_op_stack))
    {
        g_error_status = ComputeTopOperator();
        if (SUCCESS != g_error_status)
        {
            return g_error_status;
        }
    }
    
    PeekAndPop(g_num_stack, result);
    
    return SUCCESS;
}

static void PeekAndPop(stack_t* stack, void* val)
{
	assert (NULL != stack);
	
    StackPeek(stack, val);
    StackPop(stack);   
}

static status_t ComputeTopOperator(void)
{
    double right = 0.0;
    double left = 0.0;
    double result = 0.0;
    char op = '\0';

    PeekAndPop(g_num_stack, &right);
    PeekAndPop(g_num_stack, &left);
    PeekAndPop(g_op_stack, &op);
    
    result = g_op_info_lut[(unsigned char)op].calc_func(left, right); 
    
    StackPush(g_num_stack, &result);
    
    return g_error_status;
}

static status_t ProcessOperator(char new_op)
{
    char top_op = '\0';
    int new_prec = 0;
    int top_prec = 0;
    int stop_loop = 0;

    new_prec = g_op_info_lut[(unsigned char)new_op].out_stack_prec;
    
    while (!StackIsEmpty(g_op_stack) && !stop_loop)
    {
        StackPeek(g_op_stack, &top_op);
        top_prec = g_op_info_lut[(unsigned char)top_op].in_stack_prec;
        
        if (top_prec >= new_prec)
        {
            g_error_status = ComputeTopOperator();
            if (SUCCESS != g_error_status)
            {
                return g_error_status;
            }
        }
        else
        {
            ++stop_loop;
        }
    }
    
    StackPush(g_op_stack, &new_op);
    
    return SUCCESS;
}

static void ResetUnaryState(void)
{
    g_unary_flag = 0;
    g_unary_mult = 1.0;
}

static status_t ProcessExpression(const char* expression)
{
    char_type_t char_type = CHAR_INVALID;
    transition_t transition = {0, NULL};
    
    assert(NULL != expression);
    
    while (SUCCESS == g_error_status && char_type != CHAR_END)
    {
        char_type = g_char_type_lut[(unsigned char)*expression];
        transition = g_transition_lut[g_current_state][char_type];
        g_error_status = transition.handler(&expression);  
        g_current_state = transition.next_state;
    }
    
    return g_error_status;
}

/*==========================================================================*/


/*---------------------------- Operators math functions ----------------------*/
static double DuumyFunc(double a, double b)
{
	UNUSED(a);
	UNUSED(b);
	return 0.0;
}

static double Add(double a, double b)
{
    return a + b;
}

static double Mult(double a, double b)
{
    return a * b;
}
static double Pow(double base, double exp)
{
    double result = 1.0;
    int i = 0;
    int is_negetive = 0;
    int n = (int)exp;
    
    if (exp < 0)
    {
    	n = n - n - n;
    	++is_negetive;
    }
    
    for (i = 0; i < n; ++i)
    {
        result *= base;
    }
    
    return (is_negetive ? 1.0 / result : result);
}

static double Div(double a, double b)
{
    if (0.0 == b)
    {
        g_error_status = MATH_ERROR;
        
        return 0.0;
    }
    
    return a / b;
}

static double Sub(double a, double b)
{
    return a - b;
}
/*==========================================================================*/


/*----------------------------- Handler functions ------------------------*/
static status_t HandleCloseParen(const char** input)
{
    char top_op = '\0';
    int found_open = 0;
    
    assert(NULL != input);
    assert(NULL != *input);
    
    if (0 >= g_paren_count)
    {
        g_error_status = INVALID_EXPRESSION;
        
        return INVALID_EXPRESSION;
    }
    
    while (!StackIsEmpty(g_op_stack) && !found_open && SUCCESS == g_error_status)
    {
        StackPeek(g_op_stack, &top_op);
        if ('(' == top_op)
        {
            StackPop(g_op_stack);
            --g_paren_count;
            ++found_open;
        }
        else
        {
            g_error_status = ComputeTopOperator();
        }
    }
    
    if (!found_open)
    {
        return INVALID_EXPRESSION;
    }
    
    ++(*input);
    
    return SUCCESS;
}

static status_t HandleEnd(const char** input)
{
    UNUSED(input);
    
    if (g_unary_flag)
    {
        g_error_status = INVALID_EXPRESSION;
        return INVALID_EXPRESSION;
    }
    
    return SUCCESS;
}

static status_t HandleOpenParen(const char** input)
{
    char paren = '(';
    char op = '\0';
    double zero = 0.0;
    
    assert(NULL != input);
    assert(NULL != *input);
    
    if (g_unary_flag)
    {
        StackPush(g_num_stack, &zero);
        op = (g_unary_mult < 0) ? '-' : '+';
        
        g_error_status = ProcessOperator(op);
        if (SUCCESS != g_error_status)
        {
            return g_error_status;
        }
        
        ResetUnaryState();
    }
    
    StackPush(g_op_stack, &paren);
    ++g_paren_count;
    ++(*input);
    
    return SUCCESS;
}

static status_t HandleOperator(const char** input)
{    
    assert(NULL != input);
    assert(NULL != *input);
    
    g_error_status = ProcessOperator(**input);
    
    if (SUCCESS != g_error_status)
    {
        return g_error_status;
    }
    
    ++(*input);
    
    return SUCCESS;
}

static status_t HandleUnaryMinus(const char** input)
{
	char op = '*';
	
    assert(NULL != input);
    assert(NULL != *input);

    if (g_unary_flag)
    {
        g_current_state = ERROR;
        g_error_status = INVALID_EXPRESSION;
        return INVALID_EXPRESSION;;
    }
    
	g_unary_mult = -1.0;
    StackPush(g_num_stack, &g_unary_mult);
    StackPush(g_op_stack, &op);
    g_unary_flag = 1;

    ++(*input);

    return SUCCESS;   
}

static status_t HandleUnaryPlus(const char** input)
{
	char op = '*';
	
    assert(NULL != input);
    assert(NULL != *input);

    if (g_unary_flag)
    {
        g_current_state = ERROR;
        g_error_status = INVALID_EXPRESSION;
        return INVALID_EXPRESSION;;
    }

    g_unary_mult = 1.0;
    StackPush(g_num_stack, &g_unary_mult);
    StackPush(g_op_stack, &op);
    g_unary_flag = 1;

    ++(*input);

    return SUCCESS;
}

static status_t HandleNum(const char** input)
{
    double val = 0.0;
    char* end_ptr = NULL;

    assert(NULL != input);
    assert(NULL != *input);
    
    errno = 0;
    val = strtod(*input, &end_ptr);
    
    if (end_ptr == *input)
    {
        g_current_state = ERROR;
        g_error_status = INVALID_EXPRESSION;
        return INVALID_EXPRESSION;
    }

    ResetUnaryState();
    
    *input = end_ptr;
    StackPush(g_num_stack, &val);

    return SUCCESS;    
}

static status_t HandleSpace(const char** input)
{
    assert (NULL != input);
    assert (NULL != *input);
    
    ++(*input);
    
    return SUCCESS;
}

static status_t HandleError(const char** input)
{
    UNUSED(input);
    g_error_status = INVALID_EXPRESSION;
    
    return INVALID_EXPRESSION;
}
/*==========================================================================*/


static void Cleanup(void)
{
    StackDestroy(g_num_stack );
    StackDestroy(g_op_stack);
    g_num_stack  = NULL;
    g_op_stack = NULL;
}

status_t Calculator(const char* expression, double* res)
{
    assert (NULL != expression);
    assert (NULL != res);
    
    g_error_status = InitProgram(expression);
    if (SUCCESS != g_error_status)
    {
        return g_error_status;   
    }
    if (!g_luts_initialized)
    {
        InitAllLuts();
    }
    
    (void)ProcessExpression(expression);
    if (SUCCESS != g_error_status)
    {
        Cleanup();
        return g_error_status;
    }
    
    g_error_status = FlushStacks(res);
    Cleanup();

    return g_error_status;
}
