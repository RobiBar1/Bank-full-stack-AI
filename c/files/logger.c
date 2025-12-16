#include <stddef.h> 	   /* size_t */
#include <unistd.h> 	  /* For using getcwd (user path to create there the file) */
#include <stdio.h>  	 /* for fgets() */
#include <assert.h>
#include <stdlib.h>

#include "String.h"     /* For StrNCmp */
#include "pointers2.h" /* for DeleteWhiteSpaces */
#include "logger.h"

#define cmd_size 5
#define TRUE 'T'
#define FALSE 0
#define UNUSED(x) (void)x

char IsCount(const char* line);
static logger_status_t CopyFileToFile(FILE* src, FILE* dst);
static logger_status_t LogerAppend(const char* file_name);

char input[BUFSIZ];
char to_exit = FALSE;

typedef void (*print_fun)(int);

typedef char (*cmd_compre)(const char*);

typedef logger_status_t (*cmd_fun)(const char*);



void Print(int a) 
{
    printf("number is: %d\n", a);
}

typedef struct 
{
	char* cmd_string;
	cmd_compre cmp_func;
	cmd_fun cmd_active;
}cmd;

typedef struct 
{
    int current_value;
    print_fun operate; 
}print_me;

/*---------------------------- not review functions ---------------------------- */
void StructInitAndPrint(void)
{
	size_t size = 10, i = 0;
	print_me* arr = (print_me*)malloc(sizeof(print_me) * size);
	if (NULL == arr)
	{
		printf("alocate memory problem");
		return;
	}
	
	for (; i < size; ++i)
	{
		arr[i].current_value = i + 1;
		arr[i].operate = Print;
	}
	
	for (i = 0; i < size; ++i)
	{
		arr[i].operate(arr[i].current_value);
	}
	
	free(arr); arr = NULL;
}
/*---------------------------- end not review functions ---------------------------- */

/*---------------------------- action commends functions ---------------------------- */
static logger_status_t LogerHeadAppend(const char *filename) 
{
    FILE *original_file = NULL;
    FILE *temp_file = NULL;
    logger_status_t check = SUCCESS;
    const char *temp_filename = "temp_text.txt";
	
	assert(NULL != filename);
	
    original_file = fopen(filename, "r");
    temp_file = fopen(temp_filename, "a+");

    if (NULL == original_file || NULL == temp_file) 
    {
        if (original_file)
        {
        	fclose(original_file);
        }
        if (temp_file)
        {
        	fclose(temp_file);
        }
        
        return FILE_OPEN_ERROR;
    }
	
	fputs(input, temp_file);
	check = CopyFileToFile(original_file, temp_file);
	if (SUCCESS != check)
	{
		fclose(original_file);
		fclose(temp_file);
		return check;
	}
    
    if(EOF == fclose(original_file))
	{
		return FILE_CLOSE_ERROR;
	}
	
	original_file = fopen(filename, "w");
	if (NULL == original_file) 
    {
    	fclose(temp_file);
    	
        return FILE_OPEN_ERROR;
    }
    
    fseek(temp_file, 0, SEEK_SET);
    check = CopyFileToFile(temp_file, original_file);
    if (SUCCESS != check)
	{
		fclose(original_file);
		fclose(temp_file);
		return check;
	}
	
    if(EOF == fclose(original_file))
	{
		return FILE_CLOSE_ERROR;
	}
	if(EOF == fclose(temp_file))
	{
		return FILE_CLOSE_ERROR;
	}
	
    remove(temp_filename);
    printf("Prepended text to '%s'.\n", filename);
    
    return SUCCESS;
}

static logger_status_t LogerAppend(const char* file_name)
{
	FILE* file = NULL;
	
	assert(NULL != file_name);
	
	if (NULL == (file = fopen(file_name, "a")))
	{
		return FILE_OPEN_ERROR;
	}
	
	if (EOF == fputs(input, file))
	{
		fclose(file);
		return FILE_WRITE_ERROR; 
	}
	
	if(fclose(file) == EOF)
	{
		return FILE_CLOSE_ERROR;
	}
		
	return SUCCESS;
}


static logger_status_t LogerExit(const char* file_name)
{
	UNUSED(file_name);
	to_exit = TRUE;
	
	return SUCCESS;
}
static logger_status_t LogerRemove(const char* file_name)
{
	assert(NULL != file_name);
	
	if (0 != remove(file_name))
	{
		return FILE_OPEN_ERROR;
	}
	
	return SUCCESS;
}

static logger_status_t LogerCount(const char* file_name)
{
	FILE* file = NULL;
	size_t counter = 0;
	char buffer[BUFSIZ];
	
	assert(NULL != file_name);
	
	if (NULL == (file = fopen(file_name, "r")))
	{
		return FILE_OPEN_ERROR;
	}
	while (NULL != fgets(buffer, BUFSIZ, file)) 
    {
        ++counter;
    	printf("line num %lu is: %s\n", counter, buffer);
    }
	printf("---------------------------------------------\n");
	printf("num of line is: %lu\n", counter);
	printf("---------------------------------------------\n");
	if(fclose(file) == EOF)
	{
		return FILE_CLOSE_ERROR;
	}
	
	return SUCCESS;
}

/*---------------------------- end action commends functions ---------------------------- */



/*---------------------------- Compre functions ---------------------------- */
char IsCount(const char* line)
{	
	char string1_cmp[] = "-count ";
	char string2_cmp[] = "-count\n";
	
	assert(NULL != line);
	
	if (0 == StrNCmp(line, string1_cmp, sizeof(string1_cmp)) || 
		0 == StrNCmp(line, string2_cmp, sizeof(string2_cmp)))
	{
		return TRUE;
	}
	
	return FALSE;
}

char IsExit(const char* line)
{
	char string1_cmp[] = "-exit ";
	char string2_cmp[] = "-exit\n";
	
	assert(NULL != line);
	
	if (0 == StrNCmp(line, string1_cmp, sizeof(string1_cmp)) || 
		0 == StrNCmp(line, string2_cmp, sizeof(string2_cmp)))
	{
		return TRUE;
	}
	
	return FALSE;
}

char IsRemove(const char* line)
{	
	char string1_cmp[] = "-remove ";
	char string2_cmp[] = "-remove\n";
	
	assert(NULL != line);
	
	if (0 == StrNCmp(line, string1_cmp, sizeof(string1_cmp)) || 
		0 == StrNCmp(line, string2_cmp, sizeof(string2_cmp)))
	{
		return TRUE;
	}
	
	return FALSE;
}

char IsFirstLine(const char* line)
{
	char string1_cmp[] = "< ";
	char string2_cmp[] = "< \n";
	
	assert(NULL != line);
	
	if (0 == StrNCmp(line, string1_cmp, sizeof(string1_cmp) - 1) || 
		0 == StrNCmp(line, string2_cmp, sizeof(string2_cmp) - 1))
	{
		return TRUE;
	}
	
	return FALSE;
}

char IsAppend(const char* line)
{
	UNUSED(line);
	return TRUE;
}

/*---------------------------- end Compre functions ----------------------------*/

/*-------------------------------- Helper functions ----------------------------*/
static logger_status_t CopyFileToFile(FILE* src, FILE* dst)
{
	char buffer[BUFSIZ];
	
	assert(NULL != src);
	assert(NULL != dst);
	
	while (NULL != fgets(buffer, BUFSIZ, src)) 
    {
        fputs(buffer, dst);
    }
    
    return SUCCESS;
}

static cmd* initStrctCmd(void)
{
	cmd* arr = (cmd*)malloc(sizeof(cmd) * cmd_size);
	
	cmd count = {"-count ", IsCount, LogerCount};
	cmd exit = {"-exit ", IsExit, LogerExit};
	cmd remove = {"-remove ", IsRemove, LogerRemove};
	cmd first_line = {"< ", IsFirstLine, LogerHeadAppend};
	cmd append = {"append(defult)", IsAppend , LogerAppend};
	
	arr[0] = count;	  	
	arr[1] = exit;
	arr[2] = remove;
	arr[3] = first_line;
	arr[4] = append;
	
	return arr;
}

static logger_status_t ActiveCmd(cmd* comend, const char* file_name)
{
	logger_status_t status = SUCCESS;
	
	assert(NULL != file_name);
	
	status = comend->cmd_active(file_name);
	if (SUCCESS == status)
	{	
		printf("SUCCESS used of commend: %s\n", comend->cmd_string);
	}
	
	return status;
}

static void OpenPrint(void)
{
	printf("please enter your input:\n");
	printf("------------------------------------------------------------\n");
	printf("'-exit'/'-count'/'-remove'/'<'string''/'string':\n");
	printf("------------------------------------------------------------\n");
}

/*--------------------------------end  Helper functions ----------------------------*/

/*--------------------------------main logger function----------------------------*/
logger_status_t Logger(const char* file_name)
{
	cmd* arr = NULL, *start_arr = NULL;
	char* line_cursor = NULL;
	logger_status_t code_status = SUCCESS;
	
	assert(NULL != file_name);
	
	printf("Welcome to Mini Loger\n");
	arr = initStrctCmd();
	start_arr = arr;
	while(!to_exit)
	{
		OpenPrint();
		arr = start_arr;
		fgets(input, sizeof(input), stdin);
		line_cursor = RemoveWhiteSpaces((char*)input);
		while(TRUE)
		{	
			if (FALSE != (*arr).cmp_func(line_cursor))
			{
				code_status = ActiveCmd(arr, file_name);
				if (SUCCESS != code_status)
				{
					to_exit = TRUE;
				}
				
				break;
			}
			
			++arr;
		}
	}
	
	if(NULL != start_arr)
	{
		free(start_arr); start_arr = NULL;
	}
	
	return code_status;
}
/*--------------------------------end main logger function--------------------*/
