#include "../../../include/commands.h"

using namespace std;

#define CMD_NB 15
#define RESPONSE_MAX_SIZE 2048
#define RESPONSE_LINE_SIZE 256
/*
 * The 15 different commands recognized by the server
 */
std::string cmd_login(string);
std::string cmd_pass(string);
std::string cmd_ping(string);
std::string cmd_ls(string);
std::string cmd_cd(string);
std::string cmd_mkdir(string);
std::string cmd_rm(string);
std::string cmd_get(string);
std::string cmd_put(string);
std::string cmd_grep(string);
std::string cmd_date(string);
std::string cmd_whoami(string);
std::string cmd_w(string);
std::string cmd_logout(string);
std::string cmd_exit(string);

std::string sanitize(string);
string break_characters = " \n"; //space and newline

class Command{
public:
  string str;
  std::string (*fct)(string);

  Command(string str0, std::string (*fct0)(string)){
    str = str0;
    fct = fct0;
  }
};

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
std::string exec_command(string cmd, int socket){
  try{
    return sanitize(cmd);
  }
  catch(Exception& e){
    e.print_error();
  }
  return 0;
}

std::string sanitize(string full_cmd){
  //TODO: it is suppose to work with spaces in front of the command
  //TODO: sanitize more
  int pos = full_cmd.find_first_of((break_characters).c_str(),0);
  string cmd = full_cmd.substr(0,pos);

  for (int i=0; i < CMD_NB; ++i){
    if (!cmd.compare(0, commands[i].str.length(), commands[i].str)){
      return commands[i].fct(full_cmd);
    }
  }
  throw Exception(ERR_INVALID_CMD);
}

std::string tokenize_ip(string){
  return "0.0.0.0";
}

std::string call_cmd(string str1){
  char *str2 = new char [str1.length()+1];
  // TODO : strncpy
  std::strcpy(str2, str1.c_str());

  /* Open the command for reading. */
  FILE *fp;
  fp = popen(str2, "r");
  if (fp == NULL) {
    throw Exception(ERR_FAIL_CMD);
  }

  char response[RESPONSE_MAX_SIZE];
  response[0] = '\0';
  char tmp[RESPONSE_LINE_SIZE];
  size_t len = 0;

   /* Read the output a line at a time - output it. */
  while (fgets(tmp, RESPONSE_LINE_SIZE, fp)) {
    len += strlen(tmp);
    if (len >= RESPONSE_MAX_SIZE){
      throw Exception(ERR_RESPONSE_TOO_LONG);
    }
    //TODO: strncat
    strcat(response, tmp);
  }
  pclose(fp);
  //TODO: verify response != NULL
  str1.assign(response, len);
  return str1;
}

std::string cmd_login(string cmd){
  return 0;
}

std::string cmd_pass(string cmd){
  return 0;
}

std::string cmd_ping(string cmd){
  if (cmd.size() == str_ping.size()){
    throw Exception(ERR_INVALID_ARGS);
  }
  string str = str_ping + " " + tokenize_ip(cmd.substr(str_ping.size() + 1)) + " -c1";
  return call_cmd((str).c_str());
}

std::string cmd_ls(string cmd){
  string str = str_ls + " -l";
  return call_cmd(str);
}

std::string cmd_cd(string cmd){
  return 0;
}

std::string cmd_mkdir(string cmd){
  return 0;
}

std::string cmd_rm(string cmd){
  return 0;
}

std::string cmd_get(string cmd){
  return 0;
}

std::string cmd_put(string cmd){
  return 0;
}

std::string cmd_grep(string cmd){
  return 0;
}

std::string cmd_date(string cmd){
  //TODO: update according to the login
  return call_cmd((str_date).c_str());
}

std::string cmd_whoami(string cmd){
  //TODO: update according to the login
  return call_cmd((str_whoami).c_str());
}

std::string cmd_w(string cmd){
  return 0;
}

std::string cmd_logout(string cmd){
  return 0;
}

std::string cmd_exit(string cmd){
  return str_bye;
}
