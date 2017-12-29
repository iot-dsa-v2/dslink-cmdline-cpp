#include "cmdline.h"

#include <boost/algorithm/string.hpp>
#include "utils/cmdlog.h"
#include "utils/kbhit.h"

CmdLine::CmdLine(std::shared_ptr<App> app, ref_<DsLink> dslink) {
  this->app = app;
  this->strand.reset(app->new_strand());
  this->link = dslink;

  Command::current_path = "";
  Command::link = dslink;
}


void CmdLine::run() {
  strand->post([&](){
    while(this->get_input()){}
    if(link != nullptr) link->strand->post([this](){link->destroy();});
  });

  app->close();
  app->wait();
}


bool CmdLine::get_input() {
  char line[CMD_MAX_LENGTH];

  // Print current path and wait
  std::cout<<cmdlog::path<<"> "<<Command::current_path<<cmdlog::reset<<" ";

  // 1. Input taking
  if (!fgets(line, CMD_MAX_LENGTH, stdin)) return false;
  std::string line_str(line);

  // If user press enter without anything
  if(line_str[0] == '\n') return true;

  // 2. Get Command
  auto cmd = command_factory.get_command(line_str);

  // 3. Execute Command
  cmd->execute();

  auto return_ = cmd->get_return_type();

  // 4. Command says we need wait he was executing
  if(return_ == COMMAND_RETURN_TERMINATE){
    return false;
  }
  else if(return_ == COMMAND_RETURN_WAIT) {
    try{
      while (std::cin.get() != '\n') {}
    }
    catch(boost::thread_interrupted&){}
  }
  else{
    Command::wait_for_bool([&]()->bool{return cmd->is_invoked();});
    if(!cmd->is_invoked()){
      std::cout<<cmdlog::stream<<"It seems that server busy, not responding or message can have large data"<<cmdlog::endl;
      std::cout<<cmdlog::stream<<"You can wait for it or cancel by pressing enter."<<cmdlog::endl;
      fflush(stdout);
      while (!cmd->is_invoked() && !kbhit()) {
        boost::this_thread::sleep(boost::posix_time::milliseconds(10));
      }
    }

  }

  // 5. Clear
  cmd->clear();

  return true;
}

