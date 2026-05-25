#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "udp_tcp_lib.h"

int CreateSocket(enum Protocol pro)
{
    int sockfd = pro == UDP ? socket(AF_INET, SOCK_DGRAM, 0)
                            : socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Socket creation failed");
    }

    return sockfd;
}

void CloseSocket(int socketfd) { close(socketfd); }

void SetupServerAddress(struct sockaddr_in* addr, int port)
{
    assert(NULL != addr);

    addr->sin_family = AF_INET;
    addr->sin_addr.s_addr = INADDR_ANY;
    addr->sin_port = htons(port);
}

void SetupClientAddress(struct sockaddr_in* addr, const char* ip, int port)
{
    assert(NULL != addr);
    assert(NULL != ip);

    addr->sin_family = AF_INET;
    addr->sin_port = htons(port);
    if (0 >= inet_pton(AF_INET, ip, &addr->sin_addr))
    {
        perror("Invalid address or address not supported");
    }
}

ssize_t SendUDPMessage(int sockfd, const struct sockaddr_in* dest_addr,
                       const char* msg)
{
    assert(NULL != dest_addr);
    assert(NULL != msg);

    return sendto(sockfd, msg, strlen(msg), 0,
                  (const struct sockaddr*)dest_addr, sizeof(*dest_addr));
}

ssize_t ReceiveUDPMessage(int sockfd, struct sockaddr_in* src_addr,
                          char* buffer, size_t buffer_size)
{
    socklen_t len = sizeof(struct sockaddr);

    assert(NULL != src_addr);
    assert(NULL != buffer);

    return recvfrom(sockfd, buffer, buffer_size - 1, 0,
                    (struct sockaddr*)src_addr, &len);
}

int AcceptTCPConnection(int server_sockfd, struct sockaddr_in* client_addr)
{
    socklen_t len = sizeof(*client_addr);
    int client_sockfd = 0;

    assert(NULL != client_addr);

    client_sockfd = accept(server_sockfd, (struct sockaddr*)client_addr, &len);
    if (0 > client_sockfd)
    {
        perror("Accept failed");
    }

    return client_sockfd;
}

ssize_t SendTCPMessage(int sockfd, const char* msg)
{
    assert(NULL != msg);

    return send(sockfd, msg, strlen(msg), 0);
}

ssize_t ReceiveTCPMessage(int sockfd, char* buffer, size_t buffer_size)
{
    assert(NULL != buffer);

    return recv(sockfd, buffer, buffer_size - 1, 0);
}