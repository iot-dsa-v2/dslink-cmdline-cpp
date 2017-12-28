
#include "command_quit.h"

CommandQuit::CommandQuit(const command_str &command,
                         const std::string &current_path,
                         const ref_<DsLink> &link)
    : Command(command, current_path, link) {}

void CommandQuit::_print() {

}

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
