#include <fcntl.h>
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

void RunPongLoop(int sem_id)
{
    struct sembuf sb_signal = {0};
    struct sembuf sb_wait = {0};
    int loop_counter = 0;

    sb_wait.sem_num = 1;
    sb_wait.sem_op = -1;

    sb_signal.sem_op = 1;
    while (loop_counter++ < 10)
    {
        semop(sem_id, &sb_wait, 1);

        printf("pong %d\n", loop_counter);
        sleep(1);

        semop(sem_id, &sb_signal, 1);
    }
}

void SemaphorePongActive()
{
    int sem_id = 0;

    sem_id = semget(ftok("/tmp", 'A'), 2, 0666);
    RunPongLoop(sem_id);
}

void NamedPipePingActive()
{
    char* fifo = "/tmp/fifo";
    char arr1[80] = {'\0'};
    int counter = 0;
    int fd = 0;

    while (1)
    {
        fd = open(fifo, O_RDONLY);

        read(fd, arr1, sizeof(arr1));

        printf("pong %d: %s\n", counter, arr1);
        sleep(4);
        close(fd);
    }
}

struct mesg_buffer
{
    long mesg_type;
    char mesg_text;
};

void ActiveMsgQue(int msgid, int num)
{
    struct mesg_buffer message = {0};
    msgrcv(msgid, &message, sizeof(message), num, 0);

    printf("Data Received is : %c \n", message.mesg_text);
}

void MsgQuePingActive()
{
    int msgid = 0;

    msgid = msgget(ftok("/tmp", 65), 0666);
    ActiveMsgQue(msgid, 4);
    ActiveMsgQue(msgid, 3);
    ActiveMsgQue(msgid, 2);
    ActiveMsgQue(msgid, 1);

    msgctl(msgid, IPC_RMID, NULL);
}

void SharedMemoryPongActive()
{
    const int shared_segment_size = 0x6400;
    char* shared_memory = NULL;
    int segment_id = 0;

    segment_id = shmget(1, shared_segment_size, 0666);
    shared_memory = (char*)shmat(segment_id, 0, 0);

    printf("%s\n", shared_memory);
    shmdt(shared_memory);

    shmctl(segment_id, IPC_RMID, 0);
}

int main()
{
    /*SemaphorePongActive();*/
    NamedPipePingActive();
    /*MsgQuePingActive();

    SharedMemoryPongActive();*/
    return 0;
}