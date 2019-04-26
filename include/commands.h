#ifndef COMMANDS_H
#define COMMANDS_H

#include <ctype.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <string>
#include <sstream>
#include <algorithm>
#include <errno.h>


#include "exception.h"
#include "Configuration.h"
#include "AuthenticationService.h"

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
const string str_pwd    = "pwd";
const string str_bye    = "bye!";
const string str_nodata = "__nodata__";

class Commands {
public:

    explicit Commands(const Configuration config);

    string exec(string, unsigned int);

    static string call_cmd(string);

private:

    Configuration conf;

    AuthenticationService auth;

    string path;

    string remove_spaces(string);

    string remove_front_spaces(string);

    string sanitize(string full_cmd, unsigned int socket);

    void require_parameters(string);

    void require_no_parameters(string);

    void check_hostname(string);

    void check_filename(string);

    void check_path(string);

    int return_error(int);

    string cmd_login(string cmd, unsigned int);

    string cmd_pass(string cmd, unsigned int);

    string cmd_ping(string cmd, unsigned int);

    string cmd_ls(string, unsigned int);

    string cmd_cd(string cmd, unsigned int);

    string cmd_mkdir(string, unsigned int);

    string cmd_rm(string, unsigned int);

    string cmd_get(string, unsigned int);

    string cmd_put(string, unsigned int);

    string cmd_grep(string, unsigned int);

    string cmd_date(string, unsigned int);

    string cmd_whoami(string, unsigned int);

    string cmd_w(string, unsigned int);

    string cmd_logout(string, unsigned int);

    string cmd_exit(string, unsigned int);

};

#endif
