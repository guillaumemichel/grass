#ifndef ERROR_H
#define ERROR_H

#include <iostream>
#include <exception>
#include <string>

/*
 * - MAN to use custom exceptions
 * -- Throw excpetion with:
 * throw Exception(ERR_CODE)
 *
 * -- Normally use try/catch blocks and use print_error to print the associated message
 * try{
 *   throw Exception(ERR_INVALID_CMD);
 * } catch(Exception& e) {
 *   e.print_error();
 * }
 *
 * -- You are done, that was easy!
 *
 */


/**
 * Error codes associated with the error messages
 */
enum error_codes {
    ERR_FIRST = -128, // not an actual error but to set the first error number
    ERR_ERR_NOT_FOUND,
    ERR_LOGIN_REQUIRED,
    ERR_FILE_NOT_FOUND,
    ERR_CANNOT_OPEN_FILE,
    ERR_MEMORY_MALLOC,
    ERR_NULL_POINTER,
    ERR_INVALID_CMD,
    ERR_INVALID_ARGS,
    ERR_ACCESS_DENIED,
    ERR_PATH_TOO_LONG,
    ERR_TRANSFER_FAIL,
    ERR_FAIL_CMD,
    ERR_CD,
    ERR_SERVER_PATH_NOT_SET,
    ERR_RESPONSE_TOO_LONG,
    ERR_NETWORK_BAD_ADDRESS,
    ERR_NETWORK_CONNECTION_SERVER_FAILED,
    ERR_NETWORK_READ_SOCKET,
    ERR_NETWORK_WRITE_SOCKET,
    ERR_NETWORK_CREATE_SOCKET,
    ERR_NETWORK_SOCKET_NOT_CREATED,
    ERR_NETWORK_SOCKET_CONFIGURATION,
    ERR_NETWORK_ACCEPT_SOCKET,
    ERR_LAST // not an actual error but to have e.g. the total number of errors
};

/**
 * Exception class that contains an integer which is the error code
 */
class Exception : public std::exception
{
private:
  int code;

public:
  Exception(int);
  std::string print_error();
};

#endif
