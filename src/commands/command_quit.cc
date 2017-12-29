
#include "command_quit.h"


std::vector<int> CommandQuit::_available_args_num_options() {
  return {0};
}

void CommandQuit::_print_usage_info() {
  std::cout<<"Just type 'quit'..."<<std::endl;
}

COMMAND_RETURN_TYPE CommandQuit::_execute() {
  print();
  return COMMAND_RETURN_TERMINATE;
}

void CommandQuit::_clear() {

}
void CommandQuit::_print() {
  std::cout<<cmdlog::info<<"Quiting..."<<cmdlog::endl;
}
