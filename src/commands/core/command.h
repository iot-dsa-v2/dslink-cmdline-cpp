
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
 private:
  COMMAND_RETURN_TYPE return_type;

 protected:
  command_data cmd_data;
  MessageStatus status;

  boost::mutex print_mutex;

  virtual std::vector<int> _available_args_num_options() {return {};};
  virtual void _print_usage_info() {};
  virtual COMMAND_RETURN_TYPE _execute() {return COMMAND_RETURN_CONTINUE;};
  virtual void _clear() {};
  virtual void _print() {};

  void print();
  void print_usage_info();

 public:
  Command(const command_data cmd_data);

  virtual ~Command(){};

 public:
  void execute();
  void clear();

  COMMAND_RETURN_TYPE get_return_type(){return return_type;}

  static std::string current_path;
  static ref_<DsLink> link;
  static int timeout_in_ms;

  static Var get_Var_from_str(const std::string str);
  static std::string merge_paths(const std::string &first, const std::string &second);
  static int wait_for_bool(const std::function<bool()>& callback);
};

#endif //CMDLINE_DSLINK_COMMAND_H
