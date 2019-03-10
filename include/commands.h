#ifndef COMMANDS_H
#define COMMANDS_H

#include <ctype.h>
#include <iostream>
#include "error.h"

using namespace std;

int exec_command(const char*, int);

int return_error(int);

#endif
