#include "../../../include/commands.h"
#include "../../../include/AuthenticationService.h"
#include "../../../include/AuthorizationService.h"
#include "../../../include/StringHelper.h"
#include <regex>
#include <unistd.h>

using namespace std;

#define CMD_NB 15
#define PATH_MAX_LEN 128
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

Commands::Commands(const Configuration config): conf(config), auth(config) {
  this->path = config.getBase();
}


/**
 * Sanitize and try to execute a given command
 * @method exec_command
 * @param  cmd          The given command
 * @param  p            The permission level of the user who try to execute the command
 * @return              0 for success, 1 for exit, <0 for failure
 */
string Commands::exec(string cmd, unsigned int socket){
    // Volatile otherwise it will be (wrongly) optimized
    volatile bool cleared = true;

  try{
    if (cmd==str_nodata) return cmd;

    // TODO : waw
    if (strncmp(cmd.c_str(), str_login.c_str(), str_login.size()) != 0 && strncmp(cmd.c_str(), str_pass.c_str(), str_pass.size()) != 0) {
        chdir((this->path + to_string(socket) + "/").c_str());
        cleared = false;
    }
    string response = sanitize(cmd, socket);

    if (strncmp(cmd.c_str(), str_login.c_str(), str_login.size()) != 0 && strncmp(cmd.c_str(), str_pass.c_str(), str_pass.size()) != 0) {
        chdir("../");
        cleared = true;
    }

    if (response=="") {
      return str_nodata;
    }
    if (response[response.size()-1]!='\n') response += '\n';
    return response;
  }
  catch(Exception& e){
      if (!cleared) {
          chdir("../");
      }
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
            Command(str_grep,   &Commands::cmd_grep),
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

void Commands::require_parameters(string cmd){
    if (cmd=="") throw Exception(ERR_INVALID_ARGS);
}
void Commands::require_no_parameters(string cmd){
    if (cmd!="") throw Exception(ERR_INVALID_ARGS);
}

string Commands::get_relative_path(){
    string current_folder = call_cmd(str_pwd);
    current_folder = current_folder.substr(0,current_folder.size()-1)+"/";
    string files_path = conf.getFilesPath();
    if (current_folder.compare(0,files_path.size(),files_path,0,files_path.size())){
        throw Exception(ERR_ACCESS_DENIED);
    }
    return current_folder.substr(files_path.size());
}

/**
 * Verify the given hostname, throw an invalid argument exception if it contains
 * characters others that "A-Z", "a-z", "0-9", '-' and '.'.
 *
 * @method check_hostname
 * @param  str            the given hostname
 */
void Commands::check_hostname(string str){
  if (str.size()>=128) throw Exception(ERR_PATH_TOO_LONG);
  for(size_t i=0;i < str.size();++i){
    char c=str[i];
    if (!(c == '.' || c == '-' || (c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))){
      throw Exception(ERR_INVALID_ARGS);
    }
  }
}

/**
 * Verify the given filename, throw an invalid argument exception if it contains
 * characters others that "A-Z", "a-z", "0-9", '-', '_' and '.'.
 *
 * @method check_filename
 * @param  str            the given filename
 */
void Commands::check_filename(string str){
    if (str.size()>=128) throw Exception(ERR_PATH_TOO_LONG);
    for(size_t i=0;i < str.size();++i){
        char c=str[i];
        if (!(c == '.' || c == '-' || c == '_' || (c >= '0' && c <= '9') ||
                (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))){
            throw Exception(ERR_INVALID_ARGS);
        }
    }
}

void Commands::check_path(string str){
    if (str.size()>=128) throw Exception(ERR_PATH_TOO_LONG);
    for(size_t i=0;i < str.size();++i){
      char c=str[i];
      if (!(c == '.' || c == '-' || c == '_' || c == '/' || (c >= '0' && c <= '9') ||
              (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))){
        throw Exception(ERR_INVALID_ARGS);
      }
    }
}

string Commands::call_cmd(string str1){
    //require_parameters(str1);
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
    require_parameters(cmd);
    auth.registerUser(socket, cmd);
    return "OK. Go on...";
}

string Commands::cmd_pass(string cmd, unsigned int socket){
    require_parameters(cmd);
    return auth.login(socket, auth.getUser(socket).getName(), cmd) ?
            "Login successful. Welcome!" : "Incorrect credentials";
}

string Commands::cmd_ping(string cmd, unsigned int){
    require_parameters(cmd);
    check_hostname(cmd);
    string str = str_ping + " -c1 " + cmd;
    return call_cmd(str);
}

string Commands::cmd_ls(string cmd, unsigned int){
    require_no_parameters(cmd);
    string str = str_ls + " -l";
    return call_cmd(str);
}

string Commands::cmd_cd(string cmd, unsigned int){
    //TODO: redirect sh error to the client (or hide it from the server)
    cmd = remove_spaces(cmd);
    require_parameters(cmd);
    check_path(cmd);
    string files_path = conf.getFilesPath();
    files_path = files_path.substr(0,files_path.size());
    bool dot = (cmd==".");
    if (cmd[0]=='/'){
        cmd=files_path + cmd;
    }
    string old_path = call_cmd(str_pwd);
    string new_path = call_cmd(str_cd+" "+cmd+";"+str_pwd);
    if (old_path==new_path && !dot) throw Exception(ERR_CD);

    if (!new_path.compare(0,files_path.size(),files_path,0,files_path.size())){
        chdir((cmd).c_str());
        return "";
    }
    throw Exception(ERR_ACCESS_DENIED);
}

string Commands::cmd_mkdir(string cmd, unsigned int){
    cmd = remove_front_spaces(cmd);
    require_parameters(cmd);
    check_filename(cmd);
    string current_folder = get_relative_path();
    if (current_folder.size() + cmd.size() > PATH_MAX_LEN){
        throw Exception(ERR_PATH_TOO_LONG);
    }
    return call_cmd(str_mkdir+" "+cmd);
}

string Commands::cmd_rm(string cmd, unsigned int){
    cmd = remove_spaces(cmd);
    require_parameters(cmd);
    check_filename(cmd);
    return call_cmd(str_rm+" -r "+cmd);
}

// TODO : check correctness of parameters for get and put
string Commands::cmd_get(string cmd, unsigned int){
    cmd = remove_front_spaces(cmd);
    require_parameters(cmd);
    check_filename(cmd);

    // Get the filename
    string separator = cmd.substr(cmd.find(" ") + 1);

    string filename = separator.substr(0, separator.find(" "));

    // Remove the last \n otherwise the filename is invalid
    return filename;
}

string Commands::cmd_put(string cmd, unsigned int){
  // TODO : check if space is present for substr

  cmd = remove_front_spaces(cmd);
  require_parameters(cmd);
  // Get the filename and checks if its correct
  string filename = cmd.substr(0, cmd.find(" "));
  check_filename(filename);

  // TODO : marche pas
  /*string current_folder = get_relative_path();
  if (current_folder.size() + cmd.size() > PATH_MAX_LEN){
      throw Exception(ERR_PATH_TOO_LONG);
  }*/

  // Get the size
  int size = std::stoi(cmd.substr(cmd.find(" ") + 1));

  return filename + ":" + to_string(size);
}

string Commands::cmd_grep(string pattern, unsigned int socket){
    // Check and parse regex
    require_parameters(pattern);
    regex re;
    try { re = regex(pattern); }
    catch(...){ throw Exception(ERR_INVALID_ARGS); }

    // List all possible files
    stringstream matches;
    vector<string> files = StringHelper::split(call_cmd("find " + conf.getBase() + "/" + auth.getUser(socket).getName() + " -type f"), '\n');

    // Filter files for which regex matches
    for(const auto& file: files) {
        FileReader fr(file);
        vector<string> fileLines;
        fr.readFileVector(fileLines);
        string content = StringHelper::stringify(fileLines);
        if(regex_match(content, re))
            matches << file << '\n';
    }
    return matches.str().substr(0, matches.str().size()-1);
}

string Commands::cmd_date(string, unsigned int){
  return call_cmd(str_date);
}

string Commands::cmd_whoami(string, unsigned int socket){
    return auth.getUser(socket).getName();
}

string Commands::cmd_w(string, unsigned int){
    stringstream users;
    for(const User &u: auth.getAuthenticatedUsers())
        users << u.getName() << endl;
    return users.str().substr(0, users.str().size()-1);
}

string Commands::cmd_logout(string, unsigned int socket){
    auth.logout(socket);
    return "Logout successful!";
}

string Commands::cmd_exit(string, unsigned int){
    return str_bye;
}
