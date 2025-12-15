#include <stddef.h> 	   /* size_t */
#include <unistd.h> 	  /* For using getcwd (user path to create there the file) */
#include <stdio.h>  	 /* for fgets() */

#include "String.h"     /* For StrNCmp */
#include "pointers2.h" /* for DeleteWhiteSpaces */

#define cmd_size 5
#define TRUE 'T'
#define FALSE 0; 

char input[BUFSIZ];
char to_exit = FALSE;

typedef void (*print_fun)(int);

typedef char (*cmd_compre)(const char*);

typedef logger_status_t (*cmd_fun)(const char*);

void Print(int a) 
{
    printf("number is: %d\n", a);
}

struct cmd
{
	string cmd_string;
	cmd_compre cmp_func;
	cmd_fun cmd_active;
}

struct print_me
{
    int current_value;
    print_fun operate; 
};

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
static logger_status_t LogerAppend(const char* file_name)
{
	File* file = NULL;
	
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


static logger_status_t LogerExit(const char* file_name) /* TODO- need free malocs*/
{
	to_exit = TRUE;
	
	return SUCCESS;
}
static logger_status_t LogerRemove(const char* file_name)
{
	assert(NULL != file_name);
	
	if (NULL == fopen(file_name, "w"))
	{
		return FILE_OPEN_ERROR;
	}
	
	return SUCCESS;
}

static logger_status_t LogerCount(const char* file_name)
{
	File* file = NULL;
	size_t counter = 0, c = 0;
	
	assert(NULL != file_name);
	
	if (NULL == (file = fopen(file_name, "r")))
	{
		return FILE_OPEN_ERROR;
	}
	
	while (EOF != (c = fgetc(file)) && c >= 0)
	{
		++counter;
	}
	
	if (EOF == c)
	{
		printf("num of line is: %lu\n", counter);
		if(fclose(file) == EOF)
		{
			return FILE_CLOSE_ERROR;
		}
		
		return SUCCESS;
	}
	else
	{
		fclose(file);
		return FILE_READ_ERROR;
	}
}

/*---------------------------- end action commends functions ---------------------------- */

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

static logger_status_t LogerHeadAppend(const char *filename) 
{
    FILE *original_file = NULL;
    FILE *temp_file = NULL;
    logger_status_t check = SUCCESS;
    //char buffer[BUFSIZ];
    const char *temp_filename = "temp_text.txt";
	
	assert(NULL != file_name);
	
    original_file = fopen(filename, "r");
    temp_file = fopen(temp_filename, "w+");

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

/*---------------------------- Compre functions ---------------------------- */
char IsCount(const char* line)
{	
	char string1_cmp[] = "-count ";
	char string2_cmp[] = "-count\n";
	
	assert(NULL != line);
	
	if (0 == StrNCmp(line, string1_cmp, sizeof(string1_cmp)) || 
		0 == StrNCmp(line, string2_cmp, sizeof(string1_cmp)))
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
		0 == StrNCmp(line, string2_cmp, sizeof(string1_cmp)))
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
		0 == StrNCmp(line, string2_cmp, sizeof(string1_cmp)))
	{
		return TRUE;
	}
	
	return FALSE;
}

char IsFirstLine(const char* line)
{
	char string1_cmp[] = "< ";
	char string2_cmp[] = "<\n";
	
	assert(NULL != line);
	
	if (0 == StrNCmp(line, string1_cmp, sizeof(string1_cmp)) || 
		0 == StrNCmp(line, string2_cmp, sizeof(string1_cmp)))
	{
		return TRUE;
	}
	
	return FALSE;
}

char IsAppend(const char* line)
{
	return TRUE;
}

/*---------------------------- end Compre functions ----------------------------*/

/*-------------------------------- Helper functions ----------------------------*/
static cmd* initStrctCmd(void)
{
	size_t cmd_index = 0;
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

static void ActiveCmd(cmd* comend, const char* file_name)
{
	logger_status_t status = SUCCESS;
	
	assert(NULL != file_name);
	
	status = comend.cmd_active(file_name);
	if (SUCCESS == status)
	{	
		print("SUCCESS");	
	}
	
	return status;
}

static void OpenPrint(void)
{
	printf("Welcome to Mini Loger\n");
	printf("please enter your input:\n");
	printf("------------------------------------------------------------\n");
	printf("'-exit'/'-count'/'-remove'/'<'string''/'string':\n");
	printf("------------------------------------------------------------\n");
}

/*--------------------------------end  Helper functions ----------------------------*/

/*--------------------------------main logger function----------------------------*/
logger_status_t Logger(const char* file_name)
{
	unsigned char user_want_to_keep_progrem = 1;
	cmd* arr = NULL, start_arr = NULL;
	
	assert(NULL != file_name);
	
	arr = initStrctCmd();
	start_arr = arr;
	while(to_exit)
	{
		OpenPrint();
		arr = start_arr;
		fgets(input, sizeof(input), stdin);
		input = RemoveWhiteSpaces(input);
		while(True)
		{	
			if (FALSE != (*arr).cmp_func(*arr.cmd_string))
			{
				if (SUCCESS == ActiveCmd(*arr, file_name));
				break;
			}
			
			++arr;
		}
	}
	
	
}
/*--------------------------------end main logger function--------------------*/
