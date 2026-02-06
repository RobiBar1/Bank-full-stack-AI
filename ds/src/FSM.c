#include <string.h> /* strlen  	   */
#include <stddef.h> /* size_t 	   */
#include <assert.h> /* assert 	   */
#include <errno.h>  /* errno  	   */


#include "stack.h"  /* StackCreate */


#define GREEN "\033[0;32m"
#define RED   "\033[0;31m"
#define RESET "\033[0m"

typedef struct
{
	state_t state;
	(*handler)(char ch);
}trans_t;

typedef enum {WAIT_FOR_NUM, WAIT_FOR_OP, ERROR, NUM_OF_STATES} state_t;
typedef int (*TransitionFunc)(char* input);

static stack_t* g_stack_nums = NULL;
static stack_t* g_stack_operators = NULL;
static  g_state = 0;
static trans_t g_trans ;
static states_t g_states_table[][NUM_OF_STATES] = 
{
/* ValidFunction return: */ /* -1 */  	/* 0 */ 		/* 1 */
							{ERROR,		WAIT_FOR_NUM,	WAIT_FOR_OP},   /* WAIT_FOR_NUM */
							{ERROR, 	WAIT_FOR_OP,	WAIT_FOR_NUM},  /* WAIT_FOR_OP  */
							{ERROR,		ERROR,	 		ERROR}, 		/* ERROR 	    */
};

static TransitionFunc g_transition_func_table[NUM_OF_STATES] = 
{WaitForNumTransition,		WaitForOpTransition,	ErrorTransition};

static int ErrorTransition(char* input)
{
	return ERROR;
}

static int WaitForOpTransition(char* input)
{
	return 1;
}

static int WaitForNumTransition(char* input)
{
	double val = 0;
	char* end = NULL;
	int curent_num = 0;
		 
	assert (NULL != input);
	
	val = (int)strtod(input, &end);
	
	while (val > 0)
	{
		
	}
	
	if (errno == ERANGE)
	{
		
	}
	else if (end = input)
	{
			
	}
	
	switch (ch)
	{
		case '0':
		case '1':
			return 1;
			break;
		default:
			return 0;
}

static HandleNext(char** input)
{
	char current = '\0';
	
}

static int HandleTransition(char* input)
{
	int status = 0;
	
	assert (NULL != input);
	
	/*g_stack_nums = StackCreate();
	g_stack_operators = StackCreate();
	*/
	
	status = (g_transition_func_table[WAIT_FOR_NUM](input));
	while ('\0' != input && ERROR != status)
	{
		HandleNext(&input);
	}
	
	status = (g_transition_func_table[status](input));
	
	
}

static int IsLetterValid(char ch)
{
	switch (ch)
	{
		case '0':
		case '1':
			return 1;
			break;
		default:
			return 0;
	}
}

static states_t Transition(states_t current_state, char ch)
{
	return g_states_table[current_state][ch - '0'];
}

static int IsValidString(char* input)
{
	states_t current_state = Q0;
	
	assert(NULL != input);
	
	while ('\0' != *input)
	{
		if(!IsLetterValid(*input))
		{
			return 0;
		}
		
		current_state = Transition(current_state, *input);
		++input;
	}
	
	return (current_state == Q2);
}
