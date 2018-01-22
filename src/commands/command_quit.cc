#include "dsa_common.h"
#include "command_quit.h"


std::vector<int> CommandQuit::_available_args_num_options() {
  return {0};
}

const char* CommandQuit::_get_usage_info() {
  return
      "\t(quit|exit|q)\n"
          "\tStops its link and exits..\n\n"
          "```> quit```\n"
          "\tquits\n\n";
}

COMMAND_RETURN_TYPE CommandQuit::_execute() {
  std::cout<<cmdlog::info<<"Quiting..."<<cmdlog::endl;
  return COMMAND_RETURN_TERMINATE;
}

void CommandQuit::_clear() {

}
void CommandQuit::_print() {
}
