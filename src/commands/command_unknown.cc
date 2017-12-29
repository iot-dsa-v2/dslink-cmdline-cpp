#include "command_unknown.h"

void CommandUnknown::_print_usage_info() {
  std::cout<<"Available commands: {cd, list, invoke, set, subscribe, quit}"<<std::endl;
}

COMMAND_RETURN_TYPE CommandUnknown::_execute() {
  std::cout<<cmdlog::error<<"Couldn't be recognized a command called "
           <<cmdlog::path<<cmd_data.command_str<<cmdlog::endl;

  print_usage_info();
  return COMMAND_RETURN_CONTINUE;
}

void CommandUnknown::_clear() {
}

void CommandUnknown::_print() {
}


