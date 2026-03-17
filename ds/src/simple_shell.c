/*
 * Writer:	Robi
 * Checker: Omer
 * Date:	15.03.2026
 */

#define _POSIX_C_SOURCE 200809L

#include <assert.h>    /* assert */
#include <stdio.h>     /* BUFSIZ */
#include <stdlib.h>    /* malloc, free */
#include <string.h>    /* strcmp */
#include <sys/types.h> /* pid_t */
#include <sys/wait.h>  /* waitpid */
#include <unistd.h>    /* fork, gethostname*/

#include "simple_shell.h"

#define COLOR_GREEN "\033[1;32m"
#define COLOR_BLUE "\033[1;34m"
#define COLOR_RESET "\033[0m"
#define HOST_NAME_MAX (64)
#define PATH_MAX (4096)

enum shell_status
{
    SUCCESS,
    ERROR,
    EXIT
};

typedef enum shell_status shell_status_t;

/*============================= HELPER FUNCTIONS =============================*/
static shell_status_t Read(char* buffer)
{
    size_t size = 0;

    assert(NULL != buffer);

    if (NULL == fgets(buffer, BUFSIZ, stdin))
    {
        return ERROR;
    }

    size = strlen(buffer);
    if (0 < size)
    {
        buffer[size - 1] = '\0';
    }

    return SUCCESS;
}

static void Tokenize(char* buffer, char** tokens)
{
    char* token = NULL;
    char* delims = " ";
    size_t i = 0;

    assert(NULL != buffer);
    assert(NULL != tokens);

    token = strtok(buffer, delims);

    while (NULL != token)
    {
        tokens[i++] = token;
        token = strtok(NULL, delims);
    }

    tokens[i] = NULL;
}

static shell_status_t EvalFork(char* buffer)
{
    char* tokens[BUFSIZ] = {0};
    pid_t pid = 0;

    assert(NULL != buffer);

    Tokenize(buffer, tokens);

    if (NULL == tokens[0])
    {
        return SUCCESS;
    }

    pid = fork();

    if (0 == pid)
    {
        execvp(tokens[0], tokens);
        perror(tokens[0]);
        exit(1);
    }
    else if (0 < pid)
    {
        waitpid(pid, NULL, 0);
    }
    else
    {
        printf("Fork error!");
        exit(1);
    }
    return SUCCESS;
}

static shell_status_t EvalSystem(char* buffer)
{
    assert(NULL != buffer);

    return (system(buffer));
}

static shell_status_t Eval(char* buffer, option_t option)
{
    assert(NULL != buffer);

    if (0 == strcmp(buffer, "exit"))
    {
        return EXIT;
    }

    switch (option)
    {
    case FORK:
        return EvalFork(buffer);
    case SYSTEM:
        return EvalSystem(buffer);
    default:
        return ERROR;
    }
}

static void PrintPrefix(void)
{
    char* user = getenv("USER");
    char hostname[HOST_NAME_MAX] = {0};
    char cwd[PATH_MAX] = {0};

    gethostname(hostname, HOST_NAME_MAX) ? strcpy(hostname, "HostName_UnKnown")
                                         : NULL;

    NULL == getcwd(cwd, PATH_MAX) ? strcpy(cwd, "CWD_UnKnown") : NULL;

    printf(COLOR_GREEN "%s@%s" COLOR_RESET ":", user, hostname);
    printf(COLOR_BLUE "%s" COLOR_RESET "$ ", cwd);
}

/*======================== H.file implementations ========================*/
int REPLShell(option_t option)
{
    shell_status_t status = SUCCESS;
    char buffer[BUFSIZ] = {0};

    assert(FORK == option || SYSTEM == option);

    while (SUCCESS == status)
    {
        PrintPrefix();
        if (ERROR == Read(buffer))
        {
            printf("Error\n");
        }
        else
        {
            status = Eval(buffer, option);
        }
    }

    return 0;
}