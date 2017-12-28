
#ifndef CMDLINE_DSLINK_COMMAND_H
#define CMDLINE_DSLINK_COMMAND_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <vector>
#include <algorithm>

#include "dslink.h"
#include "command_data.h"


using namespace dsa;

class Command {
 protected:
  command_data cmd_data;
  MessageStatus status;

  boost::mutex print_mutex;

  virtual void _clear() {};
  virtual std::vector<int> _available_args_num_options() {return {};};
  virtual void _print() {};
  virtual void _print_usage_info() {};
  virtual COMMAND_RETURN_TYPE _execute() {return COMMAND_RETURN_CONTINUE;};

  void print();
  void print_usage_info();

 public:
  Command(const command_data cmd_data);

  virtual ~Command(){};

 public:
  COMMAND_RETURN_TYPE execute();
  void clear();

  static std::string current_path;
  static ref_<DsLink> link;
  static int timeout_in_ms;

  static Var get_Var_from_str(const std::string str);
  static std::string merge_paths(const std::string &first, const std::string &second);
  static int wait_for_bool(int wait_time, const std::function<bool()>& callback);
};

#endif //CMDLINE_DSLINK_COMMAND_H
