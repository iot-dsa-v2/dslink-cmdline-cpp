#include "command_help.h"

#include <command_factory.h>


const char* CommandHelp::_get_usage_info() {
  return "\tAvailable commands {list, cd, file, filelist, invoke, subscribe, quit, set, subscribe, timeout}.\n"
         "\tAvailable prefixes {stream, debug}.\n"
         "\tType ```> help COMMAND``` for help specific command\n";
}

COMMAND_RETURN_TYPE CommandHelp::_execute() {
  if(cmd_data.command_str != "help"){
    throw std::runtime_error("Couldn't be recognized a command called " + cmd_data.command_str);
  }

  auto which = cmd_data.get_path_str();

  cmd_data.command_str = which;

  if(which == "") {
    print_usage_info();
  } else {
    CommandFactory f;
    f.print_info(cmd_data);
  }

  return COMMAND_RETURN_CONTINUE;
}

void CommandHelp::_clear() {
}

void CommandHelp::_print() {
}


