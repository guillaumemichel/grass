#ifndef ERROR_H
#define ERROR_H

#include <iostream>
#include <ctype.h>

/** Error codes that are used in the program*/
enum error_codes {
    ERR_FIRST = -128, // not an actual error but to set the first error number
    ERR_ERR_NOT_FOUND,
    ERR_LOGIN_REQUIRED,
    ERR_LAST // not an actual error but to have e.g. the total number of errors
};

extern
const char * const ERR_MESSAGES[];

void print_error(int);

#endif
