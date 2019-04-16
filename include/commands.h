#ifndef COMMANDS_H
#define COMMANDS_H

#include "exception.h"
#include <ctype.h>
#include <iostream>
#include <string>
#include <cstring>

using namespace std;

int exec_command(string, int);

int return_error(int);

#endif
