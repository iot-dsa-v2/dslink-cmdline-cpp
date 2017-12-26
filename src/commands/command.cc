
#include "command.h"

Command::Command(const command_str command,
                 const std::string current_path,
                 const ref_<DsLink> link) {
  this->command = command;
  this->current_path = current_path;
  this->link = link;
}


void Command::print_usage_info() {
  std::cout<<"Printing Usage Info..."<<std::endl;
  _print_usage_info();
  std::cout<<"...Printed Usage Info"<<std::endl;
}

void Command::print() {
  print_mutex.lock();
  // TODO: clear screen
  _print();
  print_mutex.unlock();
}


COMMAND_RETURN_TYPE Command::execute() {
  // First check command
  if(command.token_error){
    std::cout<<"\nThere is an error on tokenize.\n"<<std::endl;
    print_usage_info();
    std::cout<<"\n"<<std::endl;
    return COMMAND_RETURN_CONTINUE;
  }

  try{
    return _execute();
  }catch(std::exception &e){
    std::cout<<e.what()<<std::endl;
    print_usage_info();
    return COMMAND_RETURN_CONTINUE;
  }
}

void Command::clear() {
  _clear();
}

