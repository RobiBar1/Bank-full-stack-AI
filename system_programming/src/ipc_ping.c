#include <fcntl.h>
#include <mqueue.h>
#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#define MAX 10

union semun
{
    struct semid_ds* buf;
    unsigned short* array;
    int val;
};

void RunPingLoop(int sem_id)
{
    struct sembuf sb_signal = {0};
    struct sembuf sb_wait = {0};
    int loop_counter = 0;

    sb_wait.sem_op = -1;

    sb_signal.sem_num = 1;
    sb_signal.sem_op = 1;
    while (loop_counter++ < 10)
    {
        semop(sem_id, &sb_wait, 1);

        printf("ping %d\n", loop_counter);
        sleep(1);

        semop(sem_id, &sb_signal, 1);
    }
}

void SemaphorePingActive()

{
    union semun sem_arg = {0};
    int sem_id = {0};

    sem_id = semget(ftok("/tmp", 'A'), 2, 0666 | IPC_CREAT);

    sem_arg.val = 1;
    semctl(sem_id, 0, SETVAL, sem_arg);

    sem_arg.val = 0;
    semctl(sem_id, 1, SETVAL, sem_arg);

    RunPingLoop(sem_id);

    semctl(sem_id, 0, IPC_RMID, sem_arg);
}

void RunParentLoop(int read_fd, int write_fd)
{
    int loop_counter = 0;
    char token = 'x';

    while (loop_counter++ < 4)
    {
        ++token;
        write(write_fd, &token, sizeof(char));

        printf("ping %d\n", loop_counter);
        sleep(1);

    /*         read(read_fd, &token, sizeof(char));
     */    }
}

void RunChildLoop(int read_fd, int write_fd)
{
    int loop_counter = 0;
    char token = 'x';

    while (loop_counter++ < 10)
    {
        if (0 == read(read_fd, &token, sizeof(char)))
        {
            printf("done\n");
            return;
        }

        printf("pong %d %c\n", loop_counter, token);

    /*         write(write_fd, &token, sizeof(char));
     */    }
}

void PipePingPongActive()
{
    int parent_to_child[2] = {0};
    int child_to_parent[2] = {0};
    pid_t pid = 0;

    pipe(parent_to_child);
    pipe(child_to_parent);

    pid = fork();
    if (pid > 0)
    {
        close(parent_to_child[0]); /*read*/
        close(child_to_parent[1]); /*write*/

        RunParentLoop(child_to_parent[0], parent_to_child[1]);

        close(parent_to_child[1]);
        close(child_to_parent[0]);
        wait(NULL);
    }
    else if (0 == pid)
    {
        close(parent_to_child[1]);
        close(child_to_parent[0]);

        RunChildLoop(parent_to_child[0], child_to_parent[1]);

        close(parent_to_child[0]);
        close(child_to_parent[1]);
    }
}

void NamedPipePingActive()
{
    int fd = 0;
    int counter = 0;
    char arr1[80] = "hey there";

    char* fifo = "/tmp/fifo";
    mkfifo(fifo, 0666);

    while (1)
    {
        fd = open(fifo, O_WRONLY);

        write(fd, arr1, strlen(arr1) + 1);
        printf("ping %d: %s\n", counter, arr1);
        ++(arr1[counter]);
        sleep(1);
        close(fd);
    }
}

struct mesg_buffer
{
    long mesg_type;
    char mesg_text;
};

void MsgQuePingActive()
{
    struct mesg_buffer message = {0};
    int counter = 0;
    int msgid = 0;

    msgid = msgget(ftok("/tmp", 65), 0666 | IPC_CREAT);

    while (counter++ < 4)
    {
        message.mesg_type = counter;
        message.mesg_text = 'a' + counter;

        msgsnd(msgid, &message, sizeof(message), 0);

        printf("Data %d send is : %c \n", counter, message.mesg_text);
    }
}

void SharedMemoryPingActive()
{
    const int shared_segment_size = 0x6400;
    struct shmid_ds shmbuffer = {0};
    char* shared_memory = NULL;
    int segment_size = 0;
    int segment_id = 0;

    segment_id = shmget(1, shared_segment_size, 0666 | IPC_CREAT);

    shared_memory = (char*)shmat(segment_id, 0, 0);
    /* *shared_memory = 'a';*/
    shmctl(segment_id, IPC_STAT, &shmbuffer);
    segment_size = shmbuffer.shm_segsz;
    printf("segment size : % d\n", segment_size);

    sprintf(shared_memory, "Hello, world.");

    shmdt(shared_memory);
}

int main()
{

    /*SemaphorePingActive();*/
    PipePingPongActive();
    /*NamedPipePingActive();*/
    /*MsgQuePingActive();

    SharedMemoryPingActive();*/

    return 0;
}