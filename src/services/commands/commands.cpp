#include <sstream>
#include "../../../include/commands.h"
#include "../../../include/AuthorizationService.h"
#include "../../../include/AuthenticationService.h"

using namespace std;

#define CMD_NB 15
#define RESPONSE_MAX_SIZE 2048
#define RESPONSE_LINE_SIZE 256
/*
 * The 15 different commands recognized by the server
 */
std::string cmd_login(string, unsigned int);
std::string cmd_pass(string, unsigned int);
std::string cmd_ping(string, unsigned int);
std::string cmd_ls(string, unsigned int);
std::string cmd_cd(string, unsigned int);
std::string cmd_mkdir(string, unsigned int);
std::string cmd_rm(string, unsigned int);
std::string cmd_get(string, unsigned int);
std::string cmd_put(string, unsigned int);
std::string cmd_grep(string, unsigned int);
std::string cmd_date(string, unsigned int);
std::string cmd_whoami(string, unsigned int);
std::string cmd_w(string, unsigned int);
std::string cmd_logout(string, unsigned int);
std::string cmd_exit(string, unsigned int);

std::string sanitize(string, unsigned int);
string break_characters = " \n"; //space and newline

class Command{
public:
  string str;
  std::string (*fct)(string, unsigned int);

  Command(string str0, std::string (*fct0)(string, unsigned int)){
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

Configuration conf = Configuration(FileReader("grass.conf"));
AuthenticationService auth = AuthenticationService(conf);

/**
 * Sanitize and try to execute a given command
 * @method exec_command
 * @param  cmd          The given command
 * @param  p            The permission level of the user who try to execute the command
 * @return              0 for success, 1 for exit, <0 for failure
 */
std::string exec_command(string cmd, unsigned int socket){
  try{
    return sanitize(cmd, socket);
  }
  catch(Exception& e){
    e.print_error();
  }
  return string();
}

std::string sanitize(string full_cmd, unsigned int socket){
  //TODO: it is suppose to work with spaces in front of the command
  //TODO: sanitize more
  int pos = full_cmd.find_first_of((break_characters).c_str(),0);
  string cmd = full_cmd.substr(0,pos);

  for (int i=0; i < CMD_NB; ++i){
    if (!cmd.compare(0, commands[i].str.length(), commands[i].str)){
      return commands[i].fct(full_cmd, socket);
    }
  }

  throw Exception(ERR_INVALID_CMD);
}

void check_last_n(const char* tmp0, const char* tmp1, int n){
  if (tmp0==tmp1 || n < 0|| n>=0x100) throw Exception(ERR_INVALID_ARGS);
}

void check_n(const char* tmp0, const char* tmp1, int n){
  check_last_n(tmp0,tmp1,n);
  if (tmp1[0] != '.') throw Exception(ERR_INVALID_ARGS);
}

string tokenize_ip(string str0){
  //TODO: check for spaces at the begining of the string
  const char* str1 = (str0).c_str();
  char *tmp0, *tmp1;
  int n;
  n = strtol(str1, &tmp0, 10);
  check_n(str1, tmp0, n);
  n = strtol(++tmp0, &tmp1, 10);
  check_n(tmp0, tmp1, n);
  n = strtol(++tmp1, &tmp0, 10);
  check_n(tmp1, tmp0, n);
  n = strtol(++tmp0, &tmp1, 10);
  check_last_n(tmp0, tmp1, n);

  return str0.substr(0,str1-tmp1);
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

std::string cmd_login(string cmd, unsigned int socket){
    return auth.registerUser(socket, cmd) ? "Login successful" : "Unable to login";
}

std::string cmd_pass(string cmd, unsigned int socket){
    return auth.login(socket, auth.getUser(socket).getName(), cmd) ? "Password entered successfully" : "Incorrect password";
}

std::string cmd_ping(string cmd, unsigned int socket){
    // Check access with AuthorizationService(auth.getUser(socket)).hasAccessTo(str_ping);
  if (cmd.size() == str_ping.size()){
    throw Exception(ERR_INVALID_ARGS);
  }
  string str = str_ping + " " + tokenize_ip(cmd.substr(str_ping.size() + 1)) + " -c1";
  return call_cmd((str).c_str());
}

std::string cmd_ls(string cmd, unsigned int socket){
    // Check access with AuthorizationService(auth.getUser(socket)).hasAccessTo(str_ls);
  string str = str_ls + " -l";
  return call_cmd(str);
}

std::string cmd_cd(string cmd, unsigned int socket){
    // Check access with AuthorizationService(auth.getUser(socket)).hasAccessTo(str_cd);
  return 0;
}

std::string cmd_mkdir(string cmd, unsigned int socket){
    // Check access with AuthorizationService(auth.getUser(socket)).hasAccessTo(str_mkdir);
  return 0;
}

std::string cmd_rm(string cmd, unsigned int socket){
    // Check access with AuthorizationService(auth.getUser(socket)).hasAccessTo(str_rm);
  return 0;
}

std::string cmd_get(string cmd, unsigned int socket){
    // Check access with AuthorizationService(auth.getUser(socket)).hasAccessTo(str_get);
  return 0;
}

std::string cmd_put(string cmd, unsigned int socket){
    // Check access with AuthorizationService(auth.getUser(socket)).hasAccessTo(str_put);
  return 0;
}

std::string cmd_grep(string cmd, unsigned int socket){
    // Check access with AuthorizationService(auth.getUser(socket)).hasAccessTo(str_grep);
  return 0;
}

std::string cmd_date(string cmd, unsigned int socket){
    // Check access with AuthorizationService(auth.getUser(socket)).hasAccessTo(str_date);
  //TODO: update according to the login
  return call_cmd((str_date).c_str());
}

std::string cmd_whoami(string cmd, unsigned int socket){
    if(!AuthorizationService(auth.getUser(socket)).hasAccessTo(str_whoami)) { throw Exception(ERR_LOGIN_REQUIRED); }
    return auth.getUser(socket).getName();
}

std::string cmd_w(string cmd, unsigned int socket){
    if(!AuthorizationService(auth.getUser(socket)).hasAccessTo(str_w)) { throw Exception(ERR_LOGIN_REQUIRED); }
    std::stringstream users;
    for(const User &u: auth.getAuthenticatedUsers())
        users << u.getName() << endl;
    return users.str().substr(0, users.str().size()-1);
}

std::string cmd_logout(string cmd, unsigned int socket){
    // Check access with AuthorizationService(auth.getUser(socket)).hasAccessTo(str_logout);
  auth.logout(socket);
  return "Logout successful";
}

std::string cmd_exit(string cmd, unsigned int socket){
    // Check access with AuthorizationService(auth.getUser(socket)).hasAccessTo(str_exit);
  return str_bye;
}
