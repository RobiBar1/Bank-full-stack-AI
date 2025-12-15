#include <stddef.h> /* size_t */
#include <unistd.h> /* For using getcwd (user path to create there the file) */
#include <stdio.h>  /* for fgets() */

#define cmd_size 5
#define True 'T'
#define False 0; 

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

static void OpenPrint(void)
{
	printf("Welcome to Mini Loger\n");
	printf("please enter your input:\n");
	printf("------------------------------------------------------------\n");
	printf("'-exit'/'-count'/'-remove'/'<'string''/'string':\n");
	printf("------------------------------------------------------------\n");
}

void PrependToTextFile(const char *filename, const char *string_to_prepend) 
{
    FILE *original_file NULL;
    FILE *temp_file = NULL;
    char buffer[];
    const char *temp_filename = "temp_text.tmp";
	
	assert(NULL != file_name);
	assert(NULL != string_to_prepend);
	
    original_file = fopen(filename, "r");
    temp_file = fopen(temp_filename, "w");

    if (original_file == NULL || temp_file == NULL) 
    {
        perror("Error opening files");
        if (original_file) fclose(original_file);
        if (temp_file) fclose(temp_file);
        return;
    }

    fprintf(temp_file, "%s", string_to_prepend);

    while (NULL != fgets(buffer, MAX_LINE_LENGTH, original_file)) 
    {
        fputs(buffer, temp_file);
    }

    fclose(original_file);
    fclose(temp_file);

    remove(filename);
    rename(temp_filename, filename);
    
    printf("Prepended text to '%s'.\n", filename);

char IsCount(const char* line)
{
	StrNCmp(input)
}

char IsExit(const char* line)
{
	
}

char IsRemove(const char* line)
{
	
}

char IsFirstLine(const char* line)
{
	
}

char IsAppend(const char* line)
{
	return True;
}

static cmd* initStrctCmd(void)
{
	size_t cmd_index = 0;
	cmd* arr = (cmd*)malloc(sizeof(cmd) * cmd_size);
	
	cmd count = {"-count", IsCount, LogerCount};
	cmd exit = {"-exit", IsExit, /* enter exit function */};
	cmd remove = {"-remove", IsRemove, LogerRemove};
	cmd first_line = {"<", IsFirstLine, /* enter first_line function */};
	cmd append = {"append(defult)", IsAppend , /* enter first_line function */};
	
	
	arr[0] = count;	  	
	arr[1] = exit;
	arr[2] = remove;
	arr[3] = first_line;
	arr[4] = append;
	
	return arr;
}

static void ActiveCmd(cmd* comend, const char* file_name)
{
	if (SUCCESS == comend.cmd_active(file_name))
	{
		print("SUCCESS");
	}
}
logger_status_t Logger(const char* file_name)
{
	unsigned char user_want_to_keep_progrem = 1;
	char input[BUFSIZ];
	cmd* arr = NULL, start_arr = NULL;
	
	assert(NULL != file_name);
	
	arr = initStrctCmd();
	start_arr = arr;
	while(user_want_to_keep_progrem)
	{
		OpenPrint();
		arr = start_arr;
		fgets(input, sizeof(input), stdin);
		/* clean white space; */
		
		while(True)
		{	
			if (False != *arr.cmp_func(*arr.cmd_string))
			{
				ActiveCmd(*arr, file_name);
				break;
			}
			
			++arr;
		}
	}
}
