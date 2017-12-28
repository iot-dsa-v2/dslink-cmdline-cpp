#include "command_unknown.h"

void CommandUnknown::_print_usage_info() {
  std::cout<<"The typed command ";
  if(command.str.size() > 0)
    std::cout<<command.str[0]<<" ";
  std::cout<<"could't understand"<<std::endl;

  std::cout<<"Available commands: {cd, list, invoke, set, subscribe, quit}"<<std::endl;
}

COMMAND_RETURN_TYPE CommandUnknown::_execute() {
  print_usage_info();
  return COMMAND_RETURN_CONTINUE;
}

void CommandUnknown::_clear() {
}

void CommandUnknown::_print() {
}


