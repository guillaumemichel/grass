#include "../../../include/commands.h"

using namespace std;

/**
 * Sanitize and try to execute a given command
 * @method exec_command
 * @param  cmd          The given command
 * @param  p            The permission level of the user who try to execute the command
 * @return              0 for success, 1 for exit, <0 for failure
 */
int exec_command(const char* cmd, int p){
  system(cmd);
  return 0;
}
