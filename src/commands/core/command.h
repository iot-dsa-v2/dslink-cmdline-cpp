
#ifndef CMDLINE_DSLINK_COMMAND_H
#define CMDLINE_DSLINK_COMMAND_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include <algorithm>
#include <vector>

#include "command_data.h"
#include "dslink.h"

#include "utils/cmdlog.h"


using namespace dsa;

class Command {
 private:
  COMMAND_RETURN_TYPE return_type;

  bool invoked;
  bool informed;

 protected:
  command_data cmd_data;
  MessageStatus status;

  boost::mutex print_mutex;

  virtual std::vector<int> _available_args_num_options() {return {};};
  virtual const char* _get_usage_info() {return "";};
  virtual COMMAND_RETURN_TYPE _execute() {return COMMAND_RETURN_CONTINUE;};
  virtual void _clear() {};
  virtual void _print() {};

  void print();
  void set_invoked() {invoked = true;}
  void set_invokable() {invoked = false;}

 public:
  Command(const command_data cmd_data);

  virtual ~Command(){};

 public:

  void print_usage_info();
  void execute();
  void clear();

  bool is_invoked() {return invoked;}

  COMMAND_RETURN_TYPE get_return_type(){return return_type;}

  static std::string current_path;
  static ref_<DsLink> link;
  static int timeout_in_ms;

  static void print_message(const ref_<const ResponseMessage> message);
  static Var get_Var_from_str(const std::string str);
  static std::string merge_paths(const std::string &first, const std::string &second);
  static int wait_for_bool(const std::function<bool()>& callback);

  static const char* get_status_str(MessageStatus status);
  static void print_usage_static(std::string usage_str);

};

#endif //CMDLINE_DSLINK_COMMAND_H
