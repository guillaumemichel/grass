#include "../include/error.h"

using namespace std;

/**
 * Stores and prints the given errors of the program
 */


/** Array containing the strings messages associated to the error codes */
const char * const ERR_MESSAGES[] = {
    "", // no error
    "Unknown error",
    "Login required to perform this operation",
};


/**
 * Prints a given error
 * @method print_error
 * @param  err         Number associated with the error to be printed
 */
void print_error(int err){
  if (err <= ERR_FIRST || err >= ERR_LAST){
    err = ERR_ERR_NOT_FOUND;
  }
  cout << "Error: " << ERR_MESSAGES[err - ERR_FIRST] << endl;
}
