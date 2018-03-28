#ifndef CMDLINE_DSLINK_CMDLINE_H
#define CMDLINE_DSLINK_CMDLINE_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <boost/asio.hpp>
#include <boost/asio/strand.hpp>

#include "dslink.h"
#include "util/app.h"

#include "command_factory.h"

#define CMD_MAX_LENGTH 4096

using namespace dsa;

#ifdef _WIN32
#define CMDLINE_CIRCLE "O"
#else
#define CMDLINE_CIRCLE "\u25CF"
#endif

class CmdLine {
 private:
  std::shared_ptr<App> app;
  ref_<DsLink> link;
  CommandFactory command_factory;

  bool is_connected;
  void set_connected(bool value);

  bool is_waiting_user_input;

  std::unique_ptr<boost::asio::io_service::strand> strand;

  bool get_input();


 public:
  CmdLine(std::shared_ptr<App> app, ref_<DsLink> dslink);
  void print_wanting_user_input();

  void run();
};

#endif //CMDLINE_DSLINK_CMDLINE_H
