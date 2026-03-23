/*
Writer: Robi
Chcker: Shahar
Date:   23.03.2026
*/

#include <assert.h>    /* assert          */
#include <stddef.h>    /* NULL            */
#include <stdio.h>     /* fgets, fopen    */
#include <stdlib.h>    /* strtol          */
#include <string.h>    /* strstr          */
#include <sys/ipc.h>   /* ftok, IPC_CREAT */
#include <sys/sem.h>   /* semget, semop   */
#include <sys/types.h> /* key_t           */

#define ALLOC_FAIL -1
#define SUCCESS 0
#define EXIT 1

#define BUFFER_SIZE 64

typedef union semun_u
{
    int val;
    struct semid_ds* buf;
    unsigned short* array;
} semun_t;

int IsDLetter(const char* input)
{
    assert(NULL != input);

    return ('\0' != input[0] && ('D' == input[0] || 'd' == input[0]) &&
            ' ' == input[1]);
}

int IsILetter(const char* input)
{
    assert(NULL != input);

    return ('\0' != input[0] && ('I' == input[0] || 'i' == input[0]) &&
            ' ' == input[1]);
}

static void DoActionOnSema(const char* name, const char* input, int sem_id)
{
    struct sembuf sem_op = {0};
    semun_t sem_union = {0};
    char* str_end = NULL;
    int has_undo = 0;
    long number = 0;
    int val = 0;
    int is_d = 0;

    assert(NULL != input);

    is_d = IsDLetter(input);
    if (is_d || IsILetter(input))
    {
        number = strtol(&(input[2]), &str_end, 10);
        if (NULL != str_end && NULL != strstr(str_end, "[undo]"))
        {
            has_undo = SEM_UNDO;
        }

        sem_op.sem_num = 0;
        sem_op.sem_op = is_d ? (short)(-number) : (short)(number);
        sem_op.sem_flg = (short)has_undo;

        semop(sem_id, &sem_op, 1);
    }
    else if ('V' == input[0] || 'v' == input[0])
    {
        val = semctl(sem_id, 0, GETVAL, sem_union);
        printf("Semaphore val right now is: %d\n", val);
    }
    else if ('l' == input[0] || 'L' == input[0])
    {
        remove(name);
        semctl(sem_id, 0, IPC_RMID);
        exit(1);
    }
    else if ('X' == input[0] || 'x' == input[0])
    {
        exit(1);
    }
}

int SemManipulation(const char* name)
{
    char buffer[BUFFER_SIZE] = {0};
    semun_t sem_union = {0};
    FILE* file = NULL;
    int sem_id = 0;
    key_t key = 0;

    assert(NULL != name);

    file = fopen(name, "a");
    if (NULL == file)
    {
        return ALLOC_FAIL;
    }

    fclose(file);
    file = NULL;
    key = ftok(name, 'S');
    sem_id = semget(key, 1, IPC_CREAT | IPC_EXCL | 0666);
    if (0 <= sem_id)
    {
        semctl(sem_id, 0, SETVAL, sem_union);
    }
    else
    {
        sem_id = semget(key, 1, IPC_CREAT | 0666);
    }

    while (NULL != fgets(buffer, sizeof(buffer), stdin))
    {
        DoActionOnSema(name, buffer, sem_id);
    }

    return SUCCESS;
}

int main(int argc, char** argv)
{
    int status = 0;

    assert(NULL != argv);

    if (2 == argc)
    {
        status = SemManipulation(argv[1]);
    }

    return status;
}