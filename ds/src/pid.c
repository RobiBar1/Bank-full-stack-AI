#include <time.h>
#include <unistd.h>

#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

typedef struct ilrd_uid
{
    size_t counter;
    pid_t pid;
    time_t time;
    unsigned char ip[16];  
} ilrd_uid_t;

ilrd_uid_t bad_uid = {0};  

ilrd_uid_t CreateUID(void) 
{
    static size_t counter = 0;
    ilrd_uid_t new_uid = {0};
    int s = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in a = {AF_INET, 0x5000, {0x08080808}}, b;
    socklen_t l = sizeof(b);
    
    time(&new_uid.time);
    if (-1 == new_uid.time) 
    {
        return bad_uid;
    }
    
    ++counter;
    new_uid.counter = counter;
    new_uid.pid = getpid();
    
    connect(s, (void*)&a, sizeof(a));
    getsockname(s, (void*)&b, &l);
    close(s); 
    
    strcpy((char*)new_uid.ip, inet_ntoa(b.sin_addr));
    
    return new_uid; 
}
