/* private API */

#ifndef FILE_SYSTEM_PARSING_H
#define FILE_SYSTEM_PARSING_H

#include <stddef.h> /* size_t */

int EXT2Open(const char* device_num, const char* file_num);

int EXT2Read(int fd, char* buff, size_t byets);

#endif /* FILE_SYSTEM_PARSING_H */