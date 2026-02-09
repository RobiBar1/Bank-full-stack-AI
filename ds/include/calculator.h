#ifndef __CALCULATOR_H__
#define __CALCULATOR_H__

#include <stddef.h>/*size_t*/

typedef enum {SUCCESS, INVALID_EXPRESSION, MATH_ERROR, ALLOC_FAIL} status_t;

status_t Calculator(const char* expression, double* res);

#endif /*__CALCULATOR_H__*/
