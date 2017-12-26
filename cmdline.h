#ifndef CMDLINE_DSLINK_CMDLINE_H
#define CMDLINE_DSLINK_CMDLINE_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <boost/asio.hpp>
#include <boost/asio/strand.hpp>

#include "dslink.h"
#include "util/app.h"

#define CMD_MAX_LENGTH 4096

using namespace dsa;

enum COMMAND_TYPE{
  COMMAND_QUIT,
  COMMAND_CD,
  COMMAND_LIST,
  COMMAND_INVOKE,
  COMMAND_SET,
  COMMAND_SUBSCRIBE,
  COMMAND_UNKNOWN
};

class CmdLine {
 private:
  std::shared_ptr<App> app;
  ref_<DsLink> link;
  std::string current_path;

  std::unique_ptr<boost::asio::io_service::strand> strand;

  bool get_input();
  COMMAND_TYPE get_command_type(std::string str);

 public:
  CmdLine(std::shared_ptr<App> app, ref_<DsLink> dslink);

  void run();
};

#endif //CMDLINE_DSLINK_CMDLINE_H
