#include "../../../include/commands.h"
#include "../../../include/AuthorizationService.h"
#include "../../../include/AuthenticationService.h"

using namespace std;

#define CMD_NB 15
#define RESPONSE_LINE_SIZE 256
#define RESPONSE_MAX_SIZE RESPONSE_LINE_SIZE*64

/*
 * The 15 different commands recognized by the server
 */
string cmd_login(string, unsigned int);
string cmd_pass(string, unsigned int);
string cmd_ping(string, unsigned int);
string cmd_ls(string, unsigned int);
string cmd_cd(string, unsigned int);
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

string sanitize(string, unsigned int);
string break_characters = " \n\0"; //space and newline

class Command{
public:
  string str;
  string (*fct)(string, unsigned int);

  Command(string str0, string (*fct0)(string, unsigned int)){
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
string exec_command(string cmd, unsigned int socket){
  try{
    return sanitize(cmd, socket);
  }
  catch(Exception& e){
    e.print_error();
  }
  return string();
}

string remove_spaces(string input){
  string output;
  size_t i;
  for(i = 0;i < input.size();++i){
    if(input[i] != ' ') output += input[i];
  }
  return output;
}

string sanitize(string full_cmd, unsigned int socket){
  //TODO: sanitize more
  int pos = full_cmd.find_first_of((break_characters).c_str(),0);
  string cmd = full_cmd.substr(0,pos);

  for (int i=0; i < CMD_NB; ++i){
    if (!cmd.compare(0, commands[i].str.size(), commands[i].str)){
      //TODO: try this
      //if(!AuthorizationService(auth.getUser(socket)).hasAccessTo(commands[i].str))
      //  throw Exception(ERR_LOGIN_REQUIRED);
      return commands[i].fct(full_cmd.substr(commands[i].str.size()+1), socket);
    }
  }
  throw Exception(ERR_INVALID_CMD);
}

/**
 * Verify the given hostname, throw an invalid argument exception if it contains
 * characters others that "A-Z", "a-z", "0-9", '-' and '.'.
 *
 * @method check_hostname
 * @param  str            the given hostname
 */
void check_hostname(string str){
  size_t len = strlen((str).c_str());
  for(size_t i=0;i < len;++i){
    char c=str[i];
    if (!(c == '.' || c == '-' || (c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))){
      cout << i << c << endl;
      throw Exception(ERR_INVALID_ARGS);
    }
  }
}

string call_cmd(string str1){
  const char *str2 = (str1).c_str();//new char [str1.length()+1];

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
    strncat(response, tmp, len);
  }
  pclose(fp);
  if (response==NULL) throw Exception(ERR_NULL_POINTER);
  str1.assign(response, len);
  return str1;
}

string cmd_login(string cmd, unsigned int socket){
  return auth.registerUser(socket, cmd) ? "Login successful" : "Unable to login";
}

string cmd_pass(string cmd, unsigned int socket){
  return auth.login(socket, auth.getUser(socket).getName(), cmd) ?
          "Password entered successfully" : "Incorrect password";
}

string cmd_ping(string cmd, unsigned int){
    // Check access with AuthorizationService(auth.getUser(socket)).hasAccessTo(str_ping);
  if (cmd.size() == str_ping.size()){
    throw Exception(ERR_INVALID_ARGS);
  }
  check_hostname(cmd);
  string str = str_ping + " -c1 " + cmd;
  return call_cmd((str).c_str());
}

string cmd_ls(string, unsigned int){
    // Check access with AuthorizationService(auth.getUser(socket)).hasAccessTo(str_ls);
  string str = str_ls + " -l";
  return call_cmd(str);
}

string cmd_cd(string, unsigned int){
    // Check access with AuthorizationService(auth.getUser(socket)).hasAccessTo(str_cd);
  return "";
}

string cmd_mkdir(string, unsigned int){
    // Check access with AuthorizationService(auth.getUser(socket)).hasAccessTo(str_mkdir);
  return "";
}

string cmd_rm(string, unsigned int){
    // Check access with AuthorizationService(auth.getUser(socket)).hasAccessTo(str_rm);
  return "";
}

string cmd_get(string, unsigned int){
    // Check access with AuthorizationService(auth.getUser(socket)).hasAccessTo(str_get);
  return "";
}

string cmd_put(string, unsigned int){
    // Check access with AuthorizationService(auth.getUser(socket)).hasAccessTo(str_put);
  return "";
}

string cmd_grep(string, unsigned int){
    // Check access with AuthorizationService(auth.getUser(socket)).hasAccessTo(str_grep);
  return "";
}

string cmd_date(string, unsigned int){
    // Check access with AuthorizationService(auth.getUser(socket)).hasAccessTo(str_date);
  //TODO: update according to the login
  return call_cmd((str_date).c_str());
}

string cmd_whoami(string, unsigned int socket){
    if(!AuthorizationService(auth.getUser(socket)).hasAccessTo(str_whoami)) { throw Exception(ERR_LOGIN_REQUIRED); }
    return auth.getUser(socket).getName();
}

string cmd_w(string, unsigned int socket){
    if(!AuthorizationService(auth.getUser(socket)).hasAccessTo(str_w)) { throw Exception(ERR_LOGIN_REQUIRED); }
    std::stringstream users;
    for(const User &u: auth.getAuthenticatedUsers())
        users << u.getName() << endl;
    return users.str().substr(0, users.str().size()-1);
}

string cmd_logout(string, unsigned int socket){
    // Check access with AuthorizationService(auth.getUser(socket)).hasAccessTo(str_logout);
  auth.logout(socket);
  return "Logout successful";
}

string cmd_exit(string, unsigned int){
    // Check access with AuthorizationService(auth.getUser(socket)).hasAccessTo(str_exit);
  return str_bye;
}
