#ifndef COMMANDS_H
#define COMMANDS_H

#include "exception.h"
#include <ctype.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <string>
#include <sstream>
#include <algorithm>

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
const string str_nodata = "__nodata__";


string exec_command(string, unsigned int);

string tokenize_ip(string);

int return_error(int);

#endif
