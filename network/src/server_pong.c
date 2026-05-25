/*
Writer:  Robi
Checker: No checkers
Date:    24.05.2026

comments:
gd -I include/ src/client_ping.c src/udp_lib.c -o ping

gd -I include/ src/server_pong.c src/udp_tcp_lib.c  -o pong


then execute pong first.
*/

#include <assert.h>  /* assert */
#include <pthread.h> /* pthread_create */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "udp_tcp_lib.h"

#define NUM_OF_CLIENTS_CAN_GET 5
#define NUM_OF_MSG_CAN_GET 5
#define BUFFER_SIZE 1024
#define PORT 8080
#define NUM_OF_THREADS 4

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
        printf("hey11\n");
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

void* ThreadFunction(void* param)
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

        pthread_create(&(thread_id[i]), NULL, ThreadFunction,
                       (void*)(&(socket_fd_arr[i])));
    }

    for (i = 0; i < NUM_OF_THREADS; ++i)
    {
        pthread_join(thread_id[i], 0);
    }

    CloseSocket(server_sockfd);

    return 0;
}

int main()
{
    ActiveUDP();
    /*ActiveTCP();
    ActiveTCPThreads();*/
    return 0;
}