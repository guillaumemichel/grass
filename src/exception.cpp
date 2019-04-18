#include "../include/exception.h";

using namespace std;

/*
 * Error messages associated with the error codes of exception.h
 */
string ERR_MESSAGES[] = {
    "", // no error
    "Unknown error",
    "Login required to perform this operation",
    "File not found",
    "Cannot open the file",
    "Cannot allocate memory",
    "Invalid command",
    "Invalid arguments",
    "Invalid address",
    "Network error : connection to the server failed",
    "Cannot read data from the socket",
    "Cannot write data in the socket",
    "Cannot create the socket",
    "The socket was not instantiated",
    "Cannot configure the socket",
    "Cannot accept a socket"
};

/**
 * Builder of the class Exception that define the error code of the exception
 * @method Exception::Exception
 * @param  c                    Error code of the given exception
 */
Exception::Exception(int c){
  code = c;
}

/**
 * Print the message error associated with the exception
 * @method Exception::print_error
 */
void Exception::print_error(){
  if (code <= ERR_FIRST || code >= ERR_LAST){
    code = ERR_ERR_NOT_FOUND;
  }
  cout << "Error: " << ERR_MESSAGES[code - ERR_FIRST] << endl;
}
