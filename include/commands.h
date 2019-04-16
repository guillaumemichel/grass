#ifndef COMMANDS_H
#define COMMANDS_H

#include <ctype.h>
#include <iostream>
#include <cstring>
#include <string>
#include "exception.h"

using namespace std;

const string str_login  = "login";
const string str_pass   = "pass";
const string str_ping   = "ping";
const string str_ls     = "ls";
const string str_cd     = "cd";
const string str_mkdir  = "mkdir";
const string str_rm     = "rm";
const string str_get    = "get";
const string str_put    = "put";
const string str_grep   = "grep";
const string str_date   = "date";
const string str_whoami = "whoami";
const string str_w      = "w";
const string str_logout = "logout";
const string str_exit   = "exit";
const string str_bye    = "bye!";

std::string exec_command(string, int);

int return_error(int);

#endif
