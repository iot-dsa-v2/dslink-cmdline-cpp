
#include "command_quit.h"


void CommandQuit::_print_usage_info() {
  std::cout<<"Just type 'quit'..."<<std::endl;
}

COMMAND_RETURN_TYPE CommandQuit::_execute() {

  if(command.num_args() != 0) {
    throw std::runtime_error("quit command does not have any arguments");
  }

  std::cout<<"Quiting..."<<std::endl;

  return COMMAND_RETURN_TERMINATE;
}

void CommandQuit::_clear() {

}

void CommandQuit::_print() {

}
