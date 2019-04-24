#include "../../../include/commands.h"
#include "../../../include/AuthenticationService.h"
#include "../../../include/AuthorizationService.h"

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
string break_characters = " \n\0\t"; //space and newline

class Command {
public:
  string str;
  string (Commands::*fct)(string, unsigned int);

  Command(string str0, string (Commands::*fct0)(string, unsigned int)){
  //Command(string str0, string (*fct0)(string, unsigned int)){
    str = str0;
    fct = fct0;
  }
};

Commands::Commands(const Configuration config): conf(config), auth(config) {}


/**
 * Sanitize and try to execute a given command
 * @method exec_command
 * @param  cmd          The given command
 * @param  p            The permission level of the user who try to execute the command
 * @return              0 for success, 1 for exit, <0 for failure
 */
string Commands::exec(string cmd, unsigned int socket){
  try{
    if (cmd==str_nodata) return cmd;
    string response = sanitize(cmd, socket);
    if (response=="") {
      return str_nodata;
    }
    if (response[response.size()-1]!='\n') response += '\n';
    return response;
  }
  catch(Exception& e){
    return e.print_error();
  }
}

string Commands::remove_spaces(string input){
  string output;
  size_t i;
  for(i = 0;i < input.size();++i){
    if(!isspace(input[i])) output += input[i];
  }
  return output;
}

string Commands::remove_front_spaces(string input){
  size_t len = strlen((input).c_str());
  size_t i=0;
  for (; i < len && isspace(input[i]); ++i){}
  return input.substr(i);
}

string Commands::sanitize(string full_cmd, unsigned int socket){

    Command commands[CMD_NB] = {
            Command(str_login,  &Commands::cmd_login),
            Command(str_pass,   &Commands::cmd_pass),
            Command(str_ping,   &Commands::cmd_ping),
            Command(str_ls,     &Commands::cmd_ls),
            Command(str_cd,     &Commands::cmd_cd),
            Command(str_mkdir,  &Commands::cmd_mkdir),
            Command(str_rm,     &Commands::cmd_rm),
            Command(str_get,    &Commands::cmd_get),
            Command(str_put,    &Commands::cmd_put),
            Command(str_get,    &Commands::cmd_get),
            Command(str_date,   &Commands::cmd_date),
            Command(str_whoami, &Commands::cmd_whoami),
            Command(str_w,      &Commands::cmd_w),
            Command(str_logout, &Commands::cmd_logout),
            Command(str_exit,   &Commands::cmd_exit)
    };

  //full_cmd = full_cmd.substr(0, full_cmd.find_first_of('\0'));
  int pos = full_cmd.find_first_of((break_characters).c_str(),0);
  string cmd = full_cmd.substr(0,pos);

  for (int i=0; i < CMD_NB; ++i){
    if (cmd.size()==commands[i].str.size() && !cmd.compare(0, commands[i].str.size(), commands[i].str)){
        if(!AuthorizationService(auth.getUser(socket)).hasAccessTo(commands[i].str)) { throw Exception(ERR_LOGIN_REQUIRED); }
        if (full_cmd.size() <= commands[i].str.size()){
            return (this->*commands[i].Command::fct)("", socket);
        } else {
            return (this->*commands[i].Command::fct)(remove_front_spaces(full_cmd.substr(commands[i].str.size()+1)), socket);
        }
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
void Commands::check_hostname(string str){
  size_t len = strlen((str).c_str());
  for(size_t i=0;i < len;++i){
    char c=str[i];
    if (!(c == '.' || c == '-' || (c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))){
      cout << i << c << endl;
      throw Exception(ERR_INVALID_ARGS);
    }
  }
}

/**
 * Verify the given filename, throw an invalid argument exception if it contains
 * characters others that "A-Z", "a-z", "0-9", '-', '_' and '.'.
 *
 * @method check_hostname
 * @param  str            the given hostname
 */
void check_filename(string str){
  size_t len = strlen((str).c_str());
  for(size_t i=0;i < len;++i){
    char c=str[i];
    if (!(c == '.' || c == '-' || c == '_' || (c >= '0' && c <= '9') ||
            (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))){
      cout << i << c << endl;
      throw Exception(ERR_INVALID_ARGS);
    }
  }
}

string Commands::call_cmd(string str1){
  str1 += " 2>&1"; // to redirect stderr to stdout
  const char *str2 = (str1).c_str();

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

string Commands::cmd_login(string cmd, unsigned int socket){
  return auth.registerUser(socket, cmd) ? "OK. Go on..." : "Unable to login on this socket. Please restart the client.";
}

string Commands::cmd_pass(string cmd, unsigned int socket){
  return auth.login(socket, auth.getUser(socket).getName(), cmd) ?
          "Login successful. Welcome!" : "Incorrect credentials";
}

string Commands::cmd_ping(string cmd, unsigned int){
  if (cmd.size() == str_ping.size()){
    throw Exception(ERR_INVALID_ARGS);
  }
  check_hostname(cmd);
  string str = str_ping + " -c1 " + cmd;
  return call_cmd((str).c_str());
}

string Commands::cmd_ls(string, unsigned int){
  string str = str_ls + " -l";
  return call_cmd(str);
}

string Commands::cmd_cd(string cmd, unsigned int){
  //TODO: sanitize access to non-existing files and filename
  //move "/" to the files folder
  string server_path = conf.getServerPath();
  string new_path = call_cmd(str_cd+" "+cmd+";"+str_pwd);

  if (!new_path.compare(0,server_path.size()-1,server_path,0,server_path.size()-1)){
      chdir((cmd).c_str());
      return "";
  }
  throw Exception(ERR_ACCESS_DENIED);
}

string Commands::cmd_mkdir(string, unsigned int){
  cout << system("pwd") << endl;
  return "";
}

string Commands::cmd_rm(string, unsigned int){
  return "";
}

string Commands::cmd_get(string, unsigned int){
  return "";
}

string Commands::cmd_put(string, unsigned int){
  return "";
}

string Commands::cmd_grep(string, unsigned int){return "";
}

string Commands::cmd_date(string, unsigned int){
  //TODO: update according to the login
  return call_cmd((str_date).c_str());
}

string Commands::cmd_whoami(string, unsigned int socket){
    return auth.getUser(socket).getName();
}

string Commands::cmd_w(string, unsigned int){
    std::stringstream users;
    for(const User &u: auth.getAuthenticatedUsers())
        users << u.getName() << endl;
    return users.str().substr(0, users.str().size()-1);
}

string Commands::cmd_logout(string, unsigned int socket){
    auth.logout(socket);
    return "Bye!";
}

string Commands::cmd_exit(string, unsigned int){
    return str_bye;
}
