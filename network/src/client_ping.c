/*
Writer:  Robi
Checker: ?
Date:    24.05.2026

comments:
for active UDP do on terminal:
"
gd -I include/ src/server_pong.c src/udp_lib.c -o pong
gd -I include/ src/client_ping.c src/udp_lib.c -o ping
"
then execute pong first.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "udp_tcp_lib.h"

#define PORT 8080
#define BUFFER_SIZE 1024
#define LOCAL_ADRESS "127.0.0.1"
#define LOCAL_ADRESS_LALA "10.10.1.170"
#define LOCAL_ADRESS_BROADCAST "255.255.255.255"

int ActiveUDP(void)
{
    struct sockaddr_in server_addr = {0};
    char buffer[BUFFER_SIZE] = {0};
    ssize_t bytes_received = 0;
    char msg[] = "Ping";
    int sockfd = 0;
    int i = 0;

    sockfd = CreateSocket(UDP);
    if (sockfd < 0)
    {
        return 1;
    }

    SetupClientAddress(&server_addr, LOCAL_ADRESS_LALA, PORT);
    for (; i < 5; ++i)
    {
        printf("Sending: %s\n", msg);
        SendUDPMessage(sockfd, &server_addr, msg);

        bytes_received =
            ReceiveUDPMessage(sockfd, &server_addr, buffer, BUFFER_SIZE);
        if (bytes_received > 0)
        {
            printf("Received: %s\n", buffer);
        }

        sleep(1);
    }

    CloseSocket(sockfd);

    return 0;
}

int ActiveTCP(void)
{
    struct sockaddr_in server_addr = {0};
    char buffer[BUFFER_SIZE] = {0};
    ssize_t bytes_received = 0;
    char msg[] = "Ping";
    int sockfd = 0;
    int i = 0;

    sockfd = CreateSocket(TCP);
    if (sockfd < 0)
    {
        return 1;
    }

    SetupClientAddress(&server_addr, "127.0.0.1", PORT);
    if (0 >
        connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)))
    {
        perror("Connection to server failed");
        CloseSocket(sockfd);

        return 1;
    }

    printf("Connected to TCP Pong server.\n");
    for (; i < 5; ++i)
    {
        printf("Sending: %s\n", msg);
        SendTCPMessage(sockfd, msg);

        bytes_received = ReceiveTCPMessage(sockfd, buffer, BUFFER_SIZE);
        if (bytes_received > 0)
        {
            printf("Received: %s\n", buffer);
        }

        sleep(1);
    }

    CloseSocket(sockfd);

    return 0;
}

static int ConfigForBroadcast(struct sockaddr_in* brodcast, int sockfd)
{
    int broadcastPermission = 1;

    brodcast->sin_family = AF_INET;
    brodcast->sin_addr.s_addr = INADDR_BROADCAST;
    brodcast->sin_port = htons(PORT);

    if (0 > setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST,
                       (char*)&broadcastPermission,
                       sizeof(broadcastPermission)))
    {
        perror("setsockopt fail");
        CloseSocket(sockfd);

        return 1;
    }

    return 0;
}

int ActiveUDPBrodcast(void)
{
    struct sockaddr_in brodcast = {0};
    char msg[] = "Robi say Hey";
    int sockfd = 0;
    int i = 0;

    sockfd = CreateSocket(UDP);
    if (sockfd < 0)
    {
        return 1;
    }

    if (1 == ConfigForBroadcast(&brodcast, sockfd))
    {
        return 1;
    }

    for (; i < 5; ++i)
    {
        printf("Sending: %s\n", msg);
        SendUDPMessage(sockfd, &brodcast, msg);

        sleep(1);
    }

    CloseSocket(sockfd);

    return 0;
}

int main()
{
    /*ActiveUDP();
    ActiveUDPBrodcast();*/
    ActiveTCP();

    return 0;
}