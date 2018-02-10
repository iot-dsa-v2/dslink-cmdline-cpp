#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "dslink.h"
#include "util/app.h"

#include <commands/command_help.h>
#include <module/default/console_logger.h>
#include <module/storage.h>
#include <module/client_manager.h>
#include <module/authorizer.h>

#include "cmdline.h"

#include "utils/cmdlog.h"

using namespace dsa;


ref_<DsLink> create_dslink(int argc, const char* argv[], std::shared_ptr<App> app) {
  auto link = make_ref_<DsLink>(argc, argv, "cmd-dslink", "1.0.0", app);
  link->init_responder();

  static_cast<ConsoleLogger&>(link->strand->logger()).level = Logger::NONE__;

  return link;
}

int main(int argc, const char* argv[]) {
  auto app = std::make_shared<App>();

  auto cmd_link = create_dslink(argc, argv, app);
  auto cmdline = CmdLine(app, cmd_link);
  cmdline.run();

  return 0;
}
