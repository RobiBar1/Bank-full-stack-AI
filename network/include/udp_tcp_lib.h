#ifndef __UDP_UTILS_H__
#define __UDP_UTILS_H__

#include <netinet/in.h>
#include <stddef.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h> /* close */

enum Protocol
{
    UDP,
    TCP
};

/* */

/*
 * @details: Creates socket.
 *
 * @param:
 * pro - type of protocol want create socket for.
 *
 * @return: Socket file descriptor, or -1 on error.
 */
int CreateSocket(enum Protocol pro);

/*
 * @details: close socket.
 *
 * @param:
 * socketfd - socket file discriptor to close.
 */

inline void CloseSocket(int socketfd) { close(socketfd); }

/*
 * @details: Config sockaddr_in structure (ipv4/6, port, big/little endian)for a
 * server listening on the specified port with
 *
 * @param:
 * addr - The struct to config, should enter as all init to 0, cant be null.
 * port - The port the server will isten on.
 */
void SetupServerAddress(struct sockaddr_in* addr, int port);

/*
 * @details: Config sockaddr_in structure (ipv4/6, port, big/little endian) for
 * a client connecting to a specific IP and port.
 *
 * @param:
 * addr - The struct to config, should enter as all init to 0, cant be null.
 * ip - The ip of the server i want connect with, cant be null.
 * port - The port the sever listen on.
 */
void SetupClientAddress(struct sockaddr_in* addr, const char* ip, int port);

/*
 * @details: Sends a message using UDP.
 *
 * @param:
 * sockfd - Socket File Descriptor that was open before.
 * dest_addr - Contain the ip and the port of the dest.
 * msg - Buffer with the data that want to snd.
 *
 * @return:
 * the number of bytes sent, or -1 on error.
 */
ssize_t SendUDPMessage(int sockfd, const struct sockaddr_in* dest_addr,
                       const char* msg);

/*
 * @details: receive a message using UDP.
 *
 * @param:
 * sockfd - Socket File Descriptor that was open before.
 * src_addr - Contain the ip and the port of the src.
 * msg - Buffer with the data that want to snd.
 *
 * @return:
 * the number of bytes sent, or -1 on error.
 */
/* Receives a message via UDP. Returns the number of bytes received, or -1 on
 * error. */
ssize_t ReceiveUDPMessage(int sockfd, struct sockaddr_in* src_addr,
                          char* buffer, size_t buffer_size);

/*
 * @details: Accepts an incoming TCP connection.
 *
 * @param:
 * server_sockfd - The listening socket file descriptor of the server.
 * client_addr - Pointer to a sockaddr_in structure that will be populated with
 * the connecting client's address details. Cannot be NULL.
 *
 * @return:
 * The new socket file descriptor for the accepted client connection, or -1 on
 * error.
 */
int AcceptTCPConnection(int server_sockfd, struct sockaddr_in* client_addr);

/*
 * @details: Sends a message over an established TCP connection.
 *
 * @param:
 * sockfd - The socket file descriptor associated with the active TCP
 * connection.
 * msg - Pointer to the null-terminated string containing the data
 * to send. Cannot be NULL.
 *
 * @return:
 * The number of bytes sent, or -1 on error.
 */
ssize_t SendTCPMessage(int sockfd, const char* msg);

/*
 * @details: Receives a message over an established TCP connection.
 *
 * @param:
 * sockfd - The socket file descriptor associated with the active TCP
 * connection.
 *
 * buffer - Pointer to the memory buffer where the received data will be stored.
 * Cannot be NULL.
 *
 * buffer_size - The total size of the allocated buffer in
 * bytes.
 *
 * @return:
 * The number of bytes received, 0 if the peer has performed an orderly
 * shutdown, or -1 on error.
 */
ssize_t ReceiveTCPMessage(int sockfd, char* buffer, size_t buffer_size);

#endif /* UDP_UTILS_H */