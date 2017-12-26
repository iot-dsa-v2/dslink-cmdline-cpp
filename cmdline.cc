#include "cmdline.h"

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string.hpp>

CmdLine::CmdLine(std::shared_ptr<App> app, ref_<DsLink> dslink) {
  this->app = app;
  this->link = dslink;

  current_path = "";
  this->strand.reset(app->new_strand());
}


void CmdLine::run() {
  strand->post([&](){
    while(this->get_input()){}
    if(link != nullptr) link->strand->post([this](){this->link->destroy();});
  });

  app->close();
  app->wait();
}


COMMAND_TYPE CmdLine::get_command_type(std::string str) {
  if(boost::starts_with(str, "quit"))
    return COMMAND_QUIT;

  if(boost::starts_with(str, "cd"))
    return COMMAND_CD;

  if(boost::starts_with(str, "list"))
    return COMMAND_LIST;

  if(boost::starts_with(str, "invoke"))
    return COMMAND_INVOKE;

  if(boost::starts_with(str, "set"))
    return COMMAND_SET;

  if(boost::starts_with(str, "subscribe"))
    return COMMAND_SUBSCRIBE;

  return COMMAND_UNKNOWN;
}


bool CmdLine::get_input() {
  char line[CMD_MAX_LENGTH];

  // TODO: Print Current Path
  printf("$ ");

  // 1. Input taking
  if (!fgets(line, CMD_MAX_LENGTH, stdin)) return false;
  std::string line_str(line);
  if(line_str[0] == '\n') return true;

  // 2. CHECK COMMAND TYPE
  COMMAND_TYPE command = get_command_type(line_str);

  if(command == COMMAND_UNKNOWN){
    std::cout<<"COMMAND_TYPE unknown"<<std::endl;
    std::cout<<"Available commands: {cd, list, invoke, set, subscribe, quit}"<<std::endl;
    return true;
  }

  if(command == COMMAND_QUIT){
    return false;
  }

  // 3. Create Operation

  // TODO: Start Operation
  std::cout<<"echo : "<<line<<std::endl;

  // Wait to end operation
  do {
    std::cout << '\n' << "Press enter to continue...";
  } while (std::cin.get() != '\n');

  // TODO: Clear operation

  return true;

}

