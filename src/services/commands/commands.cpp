#include "../../../include/commands.h"
#include "../../../include/AuthenticationService.h"
#include "../../../include/AuthorizationService.h"

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

void Commands::require_parameters(string cmd){
    if (cmd=="") throw Exception(ERR_INVALID_ARGS);
}
void Commands::require_no_parameters(string cmd){
    if (cmd!="") throw Exception(ERR_INVALID_ARGS);
}

string Commands::get_relative_path(){
    char command[] = "/bin/pwd";
    char * const argv[] = {command, NULL};
    char * const envp[] = {NULL};
    string current_folder = call_cmd2(command,argv,envp);

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

bool Commands::dir_exists(string dir, string name){
    char command[] = "/bin/ls";
    char arg0[] = "-al";
    char * arg1 = &dir[0u];
    char * const argv[] = {command, arg0, arg1, NULL};
    char * const envp[] = {NULL};

    string ls = call_cmd2(command,argv,envp);

    int hit = ls.find(" "+name+"\n");
    cout << hit << endl;
    if (hit<0){
        return false;
    }
    int index = ls.substr(0,hit).find_last_of("\n")+1;
    if (ls[index]=='d'){
        return true;
    }
    return false;
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

string Commands::call_cmd2(const char* cmd, char * const argv[], char * const envp[]){

    char buffer[RESPONSE_MAX_SIZE] = {0};
    int pipe0[2];
    int saved_stdout;

    saved_stdout = dup(STDOUT_FILENO);  // save stdout for display later

    if( pipe(pipe0) != 0 ) {          // make a pipe
      throw Exception(ERR_FAIL_CMD);
    }

    if(!fork()){
        dup2(pipe0[1], STDOUT_FILENO); // redirect stdout to the pipe
        dup2(pipe0[1], STDERR_FILENO); // redirect stderr to the pipe
        close(pipe0[0]);
        fcntl(pipe0[1], F_SETFD, FD_CLOEXEC);
        execve(cmd, argv, envp);
        write(pipe0[1], "", 1);
        _exit(1);
    } else {
        fflush(stdout);
        close(pipe0[1]);
        read(pipe0[0], buffer, RESPONSE_MAX_SIZE); // read from pipe into buffer
    }

    dup2(saved_stdout, STDOUT_FILENO);  // reconnect stdout for testing
    return string(buffer, strlen(buffer));
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

    char command[] = "/bin/ping";
    char arg0[] = "-c1";
    char *arg1 = &cmd[0u];
    char * const argv[] = {command, arg0, arg1, NULL};
    char * const envp[] = {NULL};

    string ret = call_cmd2(command,argv,envp);
    ret = ret.substr(0,ret.size()-1); // remove the 2nd '\n'
    return ret;
}

string Commands::cmd_ls(string cmd, unsigned int){
    require_no_parameters(cmd);
    char command[] = "/bin/ls";
    char arg[] = "-l";
    char * const argv[] = {command, arg, NULL};
    char * const envp[] = {NULL};

    return call_cmd2(command,argv,envp);
}

string Commands::cmd_cd(string cmd, unsigned int){
    cmd = remove_spaces(cmd);
    if (cmd=="") cmd = "/";
    check_path(cmd);
    string files_path = conf.getFilesPath();
    string full_path;
    if (cmd==".") return ""; // no need to do anything if 'cd .'
    else if (cmd == "/") {
        full_path = files_path;
    } else if (cmd[0]=='/'){
        full_path=files_path + cmd;
    } else {
        char command0[] = "/bin/pwd";
        char * const argv0[] = {command0, NULL};
        char * const envp0[] = {NULL};

        string pwd = call_cmd2(command0,argv0,envp0);
        full_path=pwd.substr(0,pwd.size()-1)+"/"+cmd;
    }
    while(full_path[full_path.size()-1]=='/'){
        full_path = full_path.substr(0, full_path.size()-1);
    }
    size_t divider = full_path.find_last_of("/");
    string curr_dir = full_path.substr(0,divider);
    string name = full_path.substr(divider+1);
    string tmp_dir=conf.getFilesPath();
    if (full_path.size()<tmp_dir.size() || full_path.compare(0,files_path.size(),files_path)){
        throw Exception(ERR_ACCESS_DENIED);
    }
    string tmp_end;
    if (full_path==tmp_dir) tmp_end = "/";
    else tmp_end=full_path.substr(tmp_dir.size()+1)+"/";
    while(tmp_end[0]=='/'){
        tmp_end = tmp_end.substr(1);
    }
    string tmp_name;

    int index;
    while ((index=tmp_end.find_first_of("/"))>0){
        while(tmp_end[0]=='/'){
            tmp_end = tmp_end.substr(1);
        }
        tmp_name = "/"+tmp_end.substr(0,index);
        tmp_end = tmp_end.substr(index+1);
        while(tmp_end[0]=='/'){
            tmp_end = tmp_end.substr(1);
        }
        if (tmp_name != "/" && !dir_exists(tmp_dir,tmp_name.substr(1))){
            return "Error: No directory doesn't exist "+cmd;
        }
        if (tmp_name == "/.."){
            tmp_dir = tmp_dir.substr(0,tmp_dir.find_last_of("/")); //remove previous folder
        } else {
            tmp_dir = tmp_dir + tmp_name;
        }
        if (tmp_dir.compare(0,files_path.size(),files_path)){
            throw Exception(ERR_ACCESS_DENIED);
        }
    }

    if (!full_path.compare(0,files_path.size(),files_path)){
        chdir((tmp_dir).c_str());
        return "";
    }
    throw Exception(ERR_ACCESS_DENIED);
}

string Commands::cmd_cd_old(string cmd, unsigned int){
    cmd = remove_spaces(cmd);
    if (cmd=="") cmd = "/";
    check_path(cmd);
    string files_path = conf.getFilesPath();
    files_path = files_path.substr(0,files_path.size());
    if (cmd==".") return ""; // no need to do anything if 'cd .'
    if (cmd[0]=='/'){
        cmd=files_path + cmd;
    }
    cout << cmd << endl;
    string arg = "`cd "+cmd+";pwd`";
    cout << arg << endl;
    char command0[] = "/bin/sh";
    char arg00[] = "-c";
    char *arg01 = &arg[0u];
    char * const argv0[] = {command0, arg00, arg01, NULL};
    char * const envp0[] = {NULL};

    string res = call_cmd2(command0,argv0,envp0);
    cout << res << endl;
    const char* cd_error = "/bin/sh: 1: cd:";

    if (!res.compare(0,strlen(cd_error),cd_error)){
        string ret = "Error :"+res.substr(strlen(cd_error),res.find_first_of("\n"));
        return ret;
    }
    const char * prep = "/bin/sh: 1: ";
    if (!res.compare(0,strlen(prep),prep)){

        char command1[] = "/bin/pwd";
        char * const argv1[] = {command1, NULL};
        char * const envp1[] = {NULL};

        string old_path = call_cmd2(command1,argv1,envp1);
        cout << "old path " << old_path << endl;
        chdir((cmd).c_str());
        string new_path = call_cmd2(command1,argv1,envp1);

        //if (old_path==new_path) throw Exception(ERR_CD);

        if (!new_path.compare(0,files_path.size(),files_path,0,files_path.size())){
            return "";
        }
        chdir((old_path).c_str());
        throw Exception(ERR_ACCESS_DENIED);
    } throw Exception(ERR_CD);
}

string Commands::cmd_mkdir(string cmd, unsigned int){
    //TODO: correct error not accurate
    cmd = remove_front_spaces(cmd);
    require_parameters(cmd);
    check_filename(cmd);
    string current_folder = get_relative_path();
    if (current_folder.size() + cmd.size() > PATH_MAX_LEN){
        throw Exception(ERR_PATH_TOO_LONG);
    }

    char command[] = "/bin/mkdir";
    char *arg = &cmd[0u];
    char * const argv[] = {command, arg, NULL};
    char * const envp[] = {NULL};

    string ret = call_cmd2(command,argv,envp);
    if (ret!="") ret = "Error :"+ret.substr(strlen(command)+1);
    return ret;
}

string Commands::cmd_rm(string cmd, unsigned int){
    //TODO: correct error not accurate
    cmd = remove_spaces(cmd);
    require_parameters(cmd);
    check_filename(cmd);

    char command[] = "/bin/rm";
    char arg0[] = "-r";
    char *arg1 = &cmd[0u];
    char * const argv[] = {command, arg0, arg1, NULL};
    char * const envp[] = {NULL};

    string ret = call_cmd2(command,argv,envp);
    if (ret!="") ret = "Error :"+ret.substr(strlen(command)+1);
    return ret;
}

string Commands::cmd_get(string, unsigned int){
  return "";
}

string Commands::cmd_put(string, unsigned int){
    //TODO: path too long check
  return "";
}

string Commands::cmd_grep(string, unsigned int){
    return "";
}

string Commands::cmd_date(string cmd, unsigned int){
    require_no_parameters(cmd);
    char command[] = "/bin/date";
    char * const argv[] = {command, NULL};
    char * const envp[] = {NULL};

    return call_cmd2(command,argv,envp);
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
    return "Logout successful!";
}

string Commands::cmd_exit(string, unsigned int){
    return str_bye;
}
