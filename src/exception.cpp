#include "../include/exception.h";

using namespace std;

/*
 * Error messages associated with the error codes of exception.h
 */
string ERR_MESSAGES[] = {
    "", // no error
    "unknown error.",
    "login required to perform this operation!",
    "file not found.",
    "cannot open the file.",
    "cannot allocate memory!",
    "invalid command.",
    "invalid arguments.",
    "access denied!",
    "path too long.",
    "file transfer failed.",
    "failed to run command.",
    "response too long!"
    "invalid address.",
    "connection to the server failed.",
    "cannot read data from the socket.",
    "cannot write data in the socket.",
    "cannot create the socket.",
    "the socket was not instantiated!",
    "cannot configure the socket.",
    "cannot accept a socket."
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
 * Print and return the message error associated with the exception
 * @method Exception::print_error
 */
std::string Exception::print_error(){
  if (code <= ERR_FIRST || code >= ERR_LAST){
    code = ERR_ERR_NOT_FOUND;
  }
  string err_str = ERR_MESSAGES[code - ERR_FIRST];
  cout << "Error: " << err_str << endl;
  return err_str;
}
