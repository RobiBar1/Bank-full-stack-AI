#ifndef __STRNIG_H__
#define __STRING_H__
#include <stddef.h> /*size_t type */

size_t StrLen(const char *str);
int StrCmp(const char *s1, const char *s2);
char* StrCpy(char* dst, char* src);
char* StrNCpy(char dst, const char* restrict src, size_t n_bytes);
int StrNCmp(const char s1, const char s2, size_t n); 
int StrCaseCmp(const char* s1, const char* s2); 
char* StrChr(const char* str, int c);
char* StrDup(const char* str); 
char* StrCat(char* dst, const char* src); 
char* StrNCat(char* dst, const char src, size_t n_bytes);
char* StrStr(const char* haystack, const char* needle);
size_t StrSpn(const char* str, const char* accept); 
char *StrTok(char* str, const char* delim);

#endif /*__STRING_H__*/