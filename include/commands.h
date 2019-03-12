#ifndef COMMANDS_H
#define COMMANDS_H

#include <ctype.h>
#include <iostream>
#include <string>
#include "error.h"

using namespace std;

int exec_command(string, int);

int return_error(int);

#endif
