#include "cmdline.h"

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


bool CmdLine::get_input() {
  char line[CMD_MAX_LENGTH];

  // TODO: Print Current Path
  std::cout<<"> "<<current_path<<" ";

  // 1. Input taking
  if (!fgets(line, CMD_MAX_LENGTH, stdin)) return false;
  std::string line_str(line);
  if(line_str[0] == '\n') return true;

  // 2. Get Command
  auto cmd = command_factory.get_command(line_str, current_path, link);

  // 3. Execute Command
  auto return_ = cmd->execute();

  // 3.1 Get if current path changed
  current_path = cmd->get_current_path();

  // 4. If command says quit, we quit if continue we will
  if(return_ == COMMAND_RETURN_TERMINATE) return false;
  if(return_ == COMMAND_RETURN_CONTINUE) return true;

  // 5. Command says we need wait he was executing
  while (std::cin.get() != '\n'){}

  // 6. Clear
  cmd->clear();

  return true;
}

