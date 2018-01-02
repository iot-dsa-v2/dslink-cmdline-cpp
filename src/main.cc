#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "dslink.h"
#include "util/app.h"

#include <module/default/console_logger.h>
#include <commands/command_help.h>

#include "cmdline.h"

#include "utils/cmdlog.h"


using namespace dsa;
//
ref_<DsLink> cmd_link;

bool connect_dslink(int argc, const char* argv[], std::shared_ptr<App> app) {
  cmd_link = make_ref_<DsLink>(argc, argv, "cmd-dslink", "1.0.0", app);
  cmd_link->init_responder();

  static_cast<ConsoleLogger &>(cmd_link->strand->logger()).level =
      Logger::NONE__;

//  static_cast<ConsoleLogger &>(cmd_link->strand->logger()).filter =
//      Logger::WARN__ | Logger::ERROR_ | Logger::FATAL_;

  bool is_connected = false;
  cmd_link->connect([&](const shared_ptr_<Connection> connection) {
    is_connected = true;
  });

  boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
  if(!is_connected){
    std::cout<<cmdlog::error<<"cannot connect to the broker! Be sure if you give broker address in parameters."<<std::endl;
    return false;
  }

  std::cout<<cmdlog::success<<"cmd-dslink is connected, you can go for it!"<<cmdlog::endl;
  CommandHelp(command_data()).print_usage_info();

  return true;
}


int main(int argc, const char* argv[]) {

  auto app = std::make_shared<App>();

  // Connect to the broker
  if(!connect_dslink(argc, argv, app)) {
    cmd_link->destroy();
    return 1;
  }

  auto cmdline = CmdLine(app, cmd_link);
  cmdline.run();

  return 0;
}

//#include "utils/cmdlog.h"
//
//int main(int argc, const char* argv[]) {
//  std::cout<<cmdlog::success<<"This is success message"<<cmdlog::endl;
//  std::cout<<cmdlog::error<<"This is error message"<<cmdlog::endl;
//  std::cout<<cmdlog::info<<"Listing path : "<<cmdlog::reset<<cmdlog::path<<"downstream"<<cmdlog::endl;
//  std::cout<<cmdlog::var<<"{\n 'mert' : 5 \n}"<<cmdlog::endl;
//  std::cout<<cmdlog::stream<<"Press enter to cancel the stream..."<<cmdlog::endl;
//  std::cout<<cmdlog::path<<"> downstream/mylink"<<cmdlog::endl;
//
//}