#include <stdio.h>  /* for printf use */
#include <assert.h> /* for asserts use */
#include <stdlib.h> /* for mallocs use */

void printArray(size_t* arr, size_t size)
{
	size_t i;
	
	printf("print array:\n");
	for(i = 0; i < size; ++i)
	{
		printf("index number: %lu have value-> %lu\n", i, arr[i]);
	}
	
	printf("end print array:\n");
}


void InitJosefCircle(size_t* arr, size_t amount_of_pepole)
{
	size_t i;
	
	assert(NULL != arr);
	
	arr[0] = 0;
	for (i = 1; i < amount_of_pepole - 1; ++i)
	{
		arr[i] = i + 1;
	}
	
	arr[i] = 1;
}

void KillManAndSwapTarget(size_t* arr, size_t killer_index)
{
	size_t get_his_kill;
	
	assert(NULL != arr);
	
	if (arr[arr[killer_index]] == arr[killer_index])
	{
		arr[arr[killer_index]] = 0;	
	}
	else
	{
		get_his_kill = arr[arr[killer_index]];
		arr[arr[killer_index]] = 0;
		arr[killer_index] = get_his_kill;
	}
}

size_t JosefGameLoop(size_t* arr, size_t amount_of_pepole)
{
	size_t i = 1, circle_winner = 0, count_circles = 0;
	
	assert(NULL != arr);
	
	while ( i < amount_of_pepole) 
	{
		if (0 != arr[i])
		{
			KillManAndSwapTarget(arr, i);
			if (0 == arr[i])
			{
				circle_winner = i;
				break; 
			}
			
			i = arr[i];
		}
		
		if (circle_winner)
		{
			break;
		}
		
		if (count_circles > amount_of_pepole * 10) /* for test only */
		{
			break;
		}
		
		count_circles++;
	}
	
	return circle_winner;
}

size_t Josephus(size_t amount_of_pepole) /* first man do first kill */
{
	size_t *arr = NULL;
	
	if (amount_of_pepole <= 1)
	{
		return amount_of_pepole;
	}
	
	amount_of_pepole++;
	arr = (size_t*)malloc(sizeof(size_t) * amount_of_pepole);
	if (NULL == arr)
	{
		printf("alocate memory problem");
		return 0;
	}
	
	InitJosefCircle(arr, amount_of_pepole);
	
	return JosefGameLoop(arr, amount_of_pepole);
}


int ToLower(int ch)
{
	if (ch >= 'A' && ch <= 'Z')
	{
		ch += 32; 
	}
	
	return ch;
}

void EnvironmentVariblesPrint(const char* envp[])
{
	printf("%s", envp[0]);
	printf("%s", envp[1]);
}

void AllDataTypesPrint(void)
{
    char char_;
    signed char signed_char;
    unsigned char unsigned_char;

    short short_;
    short int short_int;
    signed short signed_short;
    signed short int signed_short_int;
    unsigned short unsigned_short;
    unsigned short int unsigned_short_int;

    int int_;
    signed signed_;
    signed int signed_int;
    unsigned unsigned_;
    unsigned int unsigned_int;

    long long_;
    long int long_int;
    signed long signed_long;
    signed long int signed_long_int;
    unsigned long unsigned_long;
    unsigned long int unsigned_long_int; 

    /* (unsigned long) cast for maximum C89 compatibility */
    
    printf("--- CHAR ---\n");
    printf("char:                   %lu bytes\n", (unsigned long)sizeof(char_));
    printf("signed char:            %lu bytes\n", (unsigned long)sizeof(signed_char));
    printf("unsigned char:          %lu bytes\n", (unsigned long)sizeof(unsigned_char));
    printf("\n");

    printf("--- SHORT ---\n");
    printf("short:                  %lu bytes\n", (unsigned long)sizeof(short_));
    printf("short int:              %lu bytes\n", (unsigned long)sizeof(short_int));
    printf("signed short:           %lu bytes\n", (unsigned long)sizeof(signed_short));
    printf("signed short int:       %lu bytes\n", (unsigned long)sizeof(signed_short_int));
    printf("unsigned short:         %lu bytes\n", (unsigned long)sizeof(unsigned_short));
    printf("unsigned short int:     %lu bytes\n", (unsigned long)sizeof(unsigned_short_int));
    printf("\n");

    printf("--- INT ---\n");
    printf("int:                    %lu bytes\n", (unsigned long)sizeof(int_));
    printf("signed:                 %lu bytes\n", (unsigned long)sizeof(signed_));
    printf("signed int:             %lu bytes\n", (unsigned long)sizeof(signed_int));
    printf("unsigned:               %lu bytes\n", (unsigned long)sizeof(unsigned_));
    printf("unsigned int:           %lu bytes\n", (unsigned long)sizeof(unsigned_int));
    printf("\n");

    printf("--- LONG ---\n");
    printf("long:                   %lu bytes\n", (unsigned long)sizeof(long_));
    printf("long int:               %lu bytes\n", (unsigned long)sizeof(long_int));
    printf("signed long:            %lu bytes\n", (unsigned long)sizeof(signed_long));
    printf("signed long int:        %lu bytes\n", (unsigned long)sizeof(signed_long_int));
    printf("unsigned long:          %lu bytes\n", (unsigned long)sizeof(unsigned_long));
    printf("unsigned long int:      %lu bytes\n", (unsigned long)sizeof(unsigned_long_int));
    printf("\n");
}


