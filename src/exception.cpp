#include "../include/exception.h";

using namespace std;

string ERR_MESSAGES[] = {
    "", // no error
    "Unknown error",
    "Login required to perform this operation",
    "File not found",
    "Invalid command",
    "Invalid arguments"
};

Exception::Exception(int c){
  code = c;
}

void Exception::print_error(){
  if (code <= ERR_FIRST || code >= ERR_LAST){
    code = ERR_ERR_NOT_FOUND;
  }
  cout << "Error: " << ERR_MESSAGES[code - ERR_FIRST] << endl;
}
