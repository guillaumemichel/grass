#ifndef ERROR_H
#define ERROR_H

#include <iostream>
#include <exception>

enum error_codes {
    ERR_FIRST = -128, // not an actual error but to set the first error number
    ERR_ERR_NOT_FOUND,
    ERR_LOGIN_REQUIRED,
    ERR_FILE_NOT_FOUND,
    ERR_INVALID_CMD,
    ERR_INVALID_ARGS,
    ERR_LAST // not an actual error but to have e.g. the total number of errors
};

class Exception : public std::exception
{
private:
  int code;

public:
  Exception(int);
  void print_error();
};

#endif
