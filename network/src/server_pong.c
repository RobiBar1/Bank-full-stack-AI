/*
Writer:  Robi
Checker: Eden
Date:    26.05.2026

comments:
gd -I include/ src/server_pong.c src/udp_tcp_lib.c  -o pong
gd -I include/ src/client_ping.c src/udp_tcp_lib.c  -o ping

then execute pong first.
*/

#include <assert.h>  /* assert */
#include <pthread.h> /* pthread_create */
#include <stdio.h>   /*  */
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

#include "udp_tcp_lib.h"

#define NUM_OF_CLIENTS_CAN_GET 5
#define NUM_OF_MSG_CAN_GET 5
#define BUFFER_SIZE 1024
#define PORT 8080
#define NUM_OF_THREADS 4
#define INPUT1 "ping"
#define INPUT2 "quit"
#define OUTPUT1 "pong"

#define MAX(a, b) (((a) > (b)) ? (a) : (b))

int ActiveUDP(void)
{
    struct sockaddr_in server_addr = {0};
    struct sockaddr_in client_addr = {0};
    char buffer[BUFFER_SIZE] = {0};
    ssize_t bytes_received = 0;
    int sockfd = 0;
    int i = 0;

    sockfd = CreateSocket(UDP);
    if (sockfd < 0)
    {
        return 1;
    }

    SetupServerAddress(&server_addr, PORT);
    if (0 >
        bind(sockfd, (const struct sockaddr*)&server_addr, sizeof(server_addr)))
    {
        perror("Bind failed");
        CloseSocket(sockfd);

        return 1;
    }

    while (i++ < NUM_OF_MSG_CAN_GET)
    {
        bytes_received =
            ReceiveUDPMessage(sockfd, &client_addr, buffer, BUFFER_SIZE);
        printf("hey\n");
        if (0 < bytes_received)
        {
            printf("Received: %s\n", buffer);
            SendUDPMessage(sockfd, &client_addr, "Pong");
        }
    }

    CloseSocket(sockfd);

    return 0;
}

int ActiveTCP(void)
{
    struct sockaddr_in server_addr = {0};
    struct sockaddr_in client_addr = {0};
    char buffer[BUFFER_SIZE] = {0};
    ssize_t bytes_received = 0;
    int server_sockfd = 0;
    int client_sockfd = 0;

    server_sockfd = CreateSocket(TCP);
    if (0 > server_sockfd)
    {
        return 1;
    }

    SetupServerAddress(&server_addr, PORT);
    if (bind(server_sockfd, (const struct sockaddr*)&server_addr,
             sizeof(server_addr)) < 0)
    {
        perror("Bind failed");
        CloseSocket(server_sockfd);

        return 1;
    }

    if (0 > listen(server_sockfd, NUM_OF_CLIENTS_CAN_GET))
    {
        perror("Listen failed");
        CloseSocket(server_sockfd);

        return 1;
    }

    printf("TCP Pong server listening on port %d\n", PORT);
    client_sockfd = AcceptTCPConnection(server_sockfd, &client_addr);
    if (0 > client_sockfd)
    {
        CloseSocket(server_sockfd);

        return 1;
    }

    printf("Client connected!\n");

    while (1)
    {
        bytes_received = ReceiveTCPMessage(client_sockfd, buffer, BUFFER_SIZE);
        if (bytes_received <= 0)
        {
            printf("Client disconnected or error occurred.\n");
            CloseSocket(client_sockfd);
            CloseSocket(server_sockfd);

            return 0;
        }

        printf("Received: %s\n", buffer);
        SendTCPMessage(client_sockfd, "Pong");
    }

    return 0;
}

void* DoTcpRoutine(void* param)
{
    char buffer[BUFFER_SIZE] = {0};
    ssize_t bytes_received = 0;
    int client_sockfd = 0;

    assert(NULL != param);

    client_sockfd = *((int*)param);
    printf("start working on socket fd %d.\n", client_sockfd);
    while (1)
    {
        bytes_received = ReceiveTCPMessage(client_sockfd, buffer, BUFFER_SIZE);
        if (bytes_received <= 0)
        {
            printf("Client disconnected or error occurred.\n");
            CloseSocket(client_sockfd);

            return 0;
        }

        printf("Received: %s\n", buffer);
        SendTCPMessage(client_sockfd, "Pong");
    }
}

int ActiveTCPThreads(void)
{
    pthread_t thread_id[NUM_OF_THREADS] = {0};
    int socket_fd_arr[NUM_OF_THREADS] = {0};
    struct sockaddr_in server_addr = {0};
    struct sockaddr_in client_addr = {0};
    int server_sockfd = 0;
    int client_sockfd = 0;
    int i = 0;

    server_sockfd = CreateSocket(TCP);
    if (0 > server_sockfd)
    {
        return 1;
    }

    SetupServerAddress(&server_addr, PORT);
    if (bind(server_sockfd, (const struct sockaddr*)&server_addr,
             sizeof(server_addr)) < 0)
    {
        perror("Bind failed");
        CloseSocket(server_sockfd);

        return 1;
    }

    if (0 > listen(server_sockfd, NUM_OF_CLIENTS_CAN_GET))
    {
        perror("Listen failed");
        CloseSocket(server_sockfd);

        return 1;
    }

    printf("TCP Pong server listening on port %d\n", PORT);
    for (; i < NUM_OF_THREADS; ++i)
    {
        socket_fd_arr[i] = AcceptTCPConnection(server_sockfd, &client_addr);
        printf("Client connected %d!\n", client_sockfd);

        pthread_create(&(thread_id[i]), NULL, DoTcpRoutine,
                       (void*)(&(socket_fd_arr[i])));
    }

    for (i = 0; i < NUM_OF_THREADS; ++i)
    {
        pthread_join(thread_id[i], 0);
    }

    CloseSocket(server_sockfd);

    return 0;
}

static int InitBigServer(int* fd_tcp, int* fd_udp, struct sockaddr* server_addr,
                         int* max_fd, fd_set* master)
{
    *fd_tcp = CreateSocket(TCP);
    if (*fd_tcp < 0)
    {
        return 1;
    }

    *fd_udp = CreateSocket(UDP);
    if (*fd_udp < 0)
    {
        CloseSocket(*fd_tcp);

        return 1;
    }

    if (0 > bind(*fd_tcp, server_addr, sizeof(*server_addr)) ||
        0 > bind(*fd_udp, server_addr, sizeof(*server_addr)) ||
        0 > listen(*fd_tcp, NUM_OF_CLIENTS_CAN_GET))
    {
        perror("bind or listen failed");
        CloseSocket(*fd_tcp);
        CloseSocket(*fd_udp);

        return 1;
    }

    FD_ZERO(master);

    FD_SET(*fd_tcp, master);
    FD_SET(*fd_udp, master);
    FD_SET(STDIN_FILENO, master);

    *max_fd = MAX(*fd_tcp, *fd_udp) + 1;

    return 0;
}

static void STDInHandle(int max_fd)
{
    char rec_buf[BUFFER_SIZE] = {0};
    int i = 0;

    read(0, rec_buf, BUFFER_SIZE);

    if (0 == memcmp(rec_buf, INPUT1, 4))
    {
        printf("%s\n", OUTPUT1);
    }
    else if (0 == memcmp(rec_buf, INPUT2, 4))
    {
        for (i = 0; i < max_fd; ++i)
        {
            close(i);
        }

        exit(0);
    }
}

static void TCPAccept(int new_fd, int fd_tcp, struct sockaddr* their_addr,
                      fd_set* master, int* max_fd)
{
    socklen_t len = 0;

    len = sizeof(struct sockaddr_in);
    new_fd = accept(fd_tcp, their_addr, &len);
    if (-1 == new_fd)
    {
        perror("accept");

        return;
    }

    FD_SET(new_fd, master);
    *max_fd = MAX(*max_fd, new_fd) + 1;
}

static void UDPHandle(int fd_udp, struct sockaddr* their_addr)
{
    char rec_buf[BUFFER_SIZE] = {0};
    char* OUTPUT1 = "pong";
    socklen_t len = 0;
    int numbytes = 0;

    len = sizeof(struct sockaddr_in);
    numbytes = recvfrom(fd_udp, rec_buf, BUFFER_SIZE - 1, 0, their_addr, &len);
    if (-1 == numbytes)
    {
        perror("recvfrom\n");
        exit(1);
    }

    rec_buf[numbytes] = '\0';
    printf("%s\n", rec_buf);

    if (-1 == sendto(fd_udp, OUTPUT1, 4, 0, their_addr, len))
    {
        perror("sendto\n");
        exit(1);
    }
}

static void TCPHandle(int i, fd_set* master)
{
    char rec_buf[BUFFER_SIZE] = {0};
    char* OUTPUT1 = "pong";
    int numbytes = 0;

    numbytes = recv(i, rec_buf, BUFFER_SIZE - 1, 0);
    if (0 == numbytes)
    {
        printf("tcp client num %i dies\n", i);
        FD_CLR(i, master);
        close(i);
    }
    else
    {
        rec_buf[numbytes] = 0;
        printf("%s\n", rec_buf);

        if (-1 == send(i, OUTPUT1, sizeof(OUTPUT1), 0))
        {
            perror(OUTPUT1);
        }
    }
}

int PingPongServer()
{
    struct sockaddr_in server_addr = {0};
    struct sockaddr_in their_addr = {0};
    struct timeval tv = {0};
    fd_set master = {0};
    fd_set temp = {0};
    int fd_udp = 0;
    int fd_tcp = 0;
    int max_fd = 0;
    int new_fd = 0;
    int i = 0;

    SetupServerAddress(&server_addr, PORT);
    InitBigServer(&fd_tcp, &fd_udp, (struct sockaddr*)&server_addr, &max_fd,
                  &master);
    printf("Server ready and run on port: %d\n", PORT);
    fflush(stdin);
    while (1)
    {
        temp = master;
        tv.tv_sec = 7;
        if (0 == select(max_fd, &temp, NULL, NULL, &tv))
        {
            printf("nothing recive\n");

            continue;
        }

        for (i = 0; i < max_fd; ++i)
        {
            if (FD_ISSET(i, &temp))
            {
                if (0 == i)
                {
                    STDInHandle(max_fd);
                }
                else if (i == fd_tcp)
                {
                    TCPAccept(new_fd, fd_tcp, (struct sockaddr*)(&their_addr),
                              &master, &max_fd);
                }
                else if (i == fd_udp)
                {
                    UDPHandle(fd_udp, (struct sockaddr*)(&their_addr));
                }
                else
                {
                    TCPHandle(i, &master);
                }
            }
        }
    }

    return 0;
}

int main()
{
    /*ActiveUDP();
    ActiveTCP();
    ActiveTCPThreads();*/
    PingPongServer();

    return 0;
}