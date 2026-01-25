#ifndef _UID_H_
#define _UID_H_

#include <time.h>
#include <unistd.h>
#include <arpa/inet.h>

struct ilrd_uid
{
    size_t counter;
    pid_t pid;
    time_t time;
    unsigned char ip[16];  
};

typedef struct ilrd_uid ilrd_uid_t;

extern ilrd_uid_t bad_uid;

/**
 * @desc - Creates a UID 
 * @return a uid or bad uid if fails
 * @time complexity - O(1)
*/
ilrd_uid_t ILRDUIDCreate(void);


/**
 * @desc checks if one is equal to other
 * @param[in] first - the first uid to compare with 
 * @param[in] second - the other uid to compare with.
 * @return 1 if the uid are equal, otherwise 0
 * @complexity: O(1)
 */
int IsILRDUIDEqual(const ilrd_uid_t* first, const ilrd_uid_t* second);

#endif /* _UID_H_ */
