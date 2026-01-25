/*
Writer:  Robi
Checker: Omer
Date: 	 19.01.2026
*/

#include <time.h>   	/* time_t, time					 */
#include <unistd.h>		/* close, getpid			     */

#include <assert.h> 	/* assert 						 */
#include <string.h> 	/* strcmp, memset, strcpy		 */

#include <arpa/inet.h>  /* struct sockaddr_in, socklen_t */
#include <sys/socket.h> /* socket, connect, getsockname  */

#include "uid.h"  		/* our api 						 */

#define IP_SIZE 16

ilrd_uid_t bad_uid = {0};  

int IsILRDUIDEqual(const ilrd_uid_t* first, const ilrd_uid_t* second)
{
	assert (NULL != first);
	assert (NULL != second);
	
	return first->counter == second->counter && first->pid == second->pid && 
		   first->time == second->time && 0 == 
		   strcmp((const char*)first->ip, (const char*)second->ip);
}


ilrd_uid_t ILRDUIDCreate(void) 
{
    static size_t counter = 0;
    ilrd_uid_t new_uid = {0};
    int s = -1;
	struct sockaddr_in a;
	struct sockaddr_in b;
	socklen_t l = sizeof(b);

	a.sin_family = AF_INET;
	a.sin_port = htons(80);
	a.sin_addr.s_addr = 0x08080808;
	memset(a.sin_zero, 0, sizeof(a.sin_zero));
    
    time(&new_uid.time);
    if (-1 == new_uid.time) 
    {
        return bad_uid;
    }
    
    s = socket(AF_INET, SOCK_DGRAM, 0);
    if (-1 == s)
    {
        return bad_uid;
    }
    
    if (-1 == connect(s, (void*)&a, sizeof(a)))
    {
        close(s);
        return bad_uid;
    }
    
    if (-1 == getsockname(s, (void*)&b, &l))
    {
        close(s);
        return bad_uid;
    }
    
    if (-1 == close(s))
    {
        return bad_uid;
    } 
    
    strcpy((char*)new_uid.ip, inet_ntoa(b.sin_addr));
    
    new_uid.pid = getpid();
    ++counter;
    new_uid.counter = counter;
    
    return new_uid; 
}
