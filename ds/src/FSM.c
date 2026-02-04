#include <string.h> /* strlen */
#include <stddef.h> /* size_t */
#include <assert.h> /* assert */
#include <stdio.h>  /* printf */


#define GREEN "\033[0;32m"
#define RED   "\033[0;31m"
#define RESET "\033[0m"


typedef enum {Q0, Q1, Q2, Q3, NUM_OF_STATES} states_t;

static states_t g_states_table[][2] = 
{
	/* input 0 */    /* input 1 */
	{Q1,			 Q3}, /* q0 */
	{Q2, 			 Q1}, /* q1 */
	{Q2,			 Q1}, /* q2 */
	{Q3,			 Q3}  /* q3 */
};

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
