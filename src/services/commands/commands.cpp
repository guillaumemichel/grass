#include "../../../include/commands.h"

using namespace std;

#define CMD_NB 15
/*
 * The 15 different commands recognized by the server
 */
int cmd_login(string);
int cmd_pass(string);
int cmd_ping(string);
int cmd_ls(string);
int cmd_cd(string);
int cmd_mkdir(string);
int cmd_rm(string);
int cmd_get(string);
int cmd_put(string);
int cmd_grep(string);
int cmd_date(string);
int cmd_whoami(string);
int cmd_w(string);
int cmd_logout(string);
int cmd_exit(string);

int sanitize(string);
string break_characters = " \n"; //space and newline

class Command{
public:
  string str;
  int (*fct)(string);

  Command(string str0, int (*fct0)(string)){
    str = str0;
    fct = fct0;
  }
};

string str_login  = "login";
string str_pass   = "pass";
string str_ping   = "ping";
string str_ls     = "ls";
string str_cd     = "cd";
string str_mkdir  = "mkdir";
string str_rm     = "rm";
string str_get    = "get";
string str_put    = "put";
string str_grep   = "grep";
string str_date   = "date";
string str_whoami = "whoami";
string str_w      = "w";
string str_logout = "logout";
string str_exit   = "exit";

Command commands[CMD_NB] = {
  Command(str_login,  cmd_login),
  Command(str_pass,   cmd_pass),
  Command(str_ping,   cmd_ping),
  Command(str_ls,     cmd_ls),
  Command(str_cd,     cmd_cd),
  Command(str_mkdir,  cmd_mkdir),
  Command(str_rm,     cmd_rm),
  Command(str_get,    cmd_get),
  Command(str_put,    cmd_put),
  Command(str_get,    cmd_get),
  Command(str_date,   cmd_date),
  Command(str_whoami, cmd_whoami),
  Command(str_w,      cmd_w),
  Command(str_logout, cmd_logout),
  Command(str_exit,   cmd_exit)
};

/**
 * Sanitize and try to execute a given command
 * @method exec_command
 * @param  cmd          The given command
 * @param  p            The permission level of the user who try to execute the command
 * @return              0 for success, 1 for exit, <0 for failure
 */
int exec_command(string cmd, int p){
  try{
    return sanitize(cmd);
  }
  catch(Exception& e){
    e.print_error();
  }
  return 0;
}

int sanitize(string full_cmd){
  //TODO: it is suppose to work with spaces in front of the command
  //TODO: sanitize more
  int pos = full_cmd.find_first_of((break_characters).c_str(),0);
  string cmd = full_cmd.substr(0,pos);

  for (int i=0; i < CMD_NB; ++i){
    // Fix otherwise they were not correctly compared
    if (0 == cmd.compare(0, commands[i].str.length(), commands[i].str)){
        return commands[i].fct(full_cmd);
    }
  }
  throw Exception(ERR_INVALID_CMD);
}

string tokenize_ip(string){
  return "0.0.0.0";
}

int cmd_login(string cmd){
  return 0;
}

int cmd_pass(string cmd){
  return 0;
}

int cmd_ping(string cmd){
  if (cmd.size() == str_ping.size()){
    throw Exception(ERR_INVALID_ARGS);
  }
  string str = str_ping + " " + tokenize_ip(cmd.substr(str_ping.size() + 1)) + " -c1";
  system((str).c_str());
  return 0;
}

int cmd_ls(string cmd){
  string str = str_ls + " -l";

  // Convert string to char *
  char * tab2 = new char [str.length()+1];
  // TODO : strncpy
  std::strcpy (tab2, str.c_str());

  //system((str).c_str());

  // TODO : recode this (taken from StackOverflow)
  FILE *fp;
  char path[1035];

  /* Open the command for reading. */
  fp = popen(tab2, "r");
  if (fp == NULL) {
    throw invalid_argument("Failed to run command 'ls'");
  }

  /* Read the output a line at a time - output it. */
  while (fgets(path, sizeof(path)-1, fp) != NULL) {
   printf("%s", path);
  }

  /* close */
  pclose(fp);

  // Would be cool to return the command output
  return 0;
}

int cmd_cd(string cmd){
  return 0;
}

int cmd_mkdir(string cmd){
  return 0;
}

int cmd_rm(string cmd){
  return 0;
}

int cmd_get(string cmd){
  return 0;
}

int cmd_put(string cmd){
  return 0;
}

int cmd_grep(string cmd){
  return 0;
}

int cmd_date(string cmd){
  //TODO: update according to the login
  system((str_date).c_str());
  return 0;
}

int cmd_whoami(string cmd){
  //TODO: update according to the login
  system((str_whoami).c_str());
  return 0;
}

int cmd_w(string cmd){
  return 0;
}

int cmd_logout(string cmd){
  return 0;
}

int cmd_exit(string cmd){
  return 1;
}
