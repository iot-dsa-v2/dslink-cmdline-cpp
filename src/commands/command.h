
#ifndef CMDLINE_DSLINK_COMMAND_H
#define CMDLINE_DSLINK_COMMAND_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>

#include "dslink.h"

enum COMMAND_TYPE{
  COMMAND_UNKNOWN = 0,
  COMMAND_QUIT,
  COMMAND_CD,
  COMMAND_LIST,
  COMMAND_INVOKE,
  COMMAND_SET,
  COMMAND_SUBSCRIBE
};

enum COMMAND_RETURN_TYPE{
  COMMAND_RETURN_TERMINATE,
  COMMAND_RETURN_CONTINUE,
  COMMAND_RETURN_WAIT
};

struct command_str{
  std::vector<std::string> str;
  bool token_error;
  COMMAND_TYPE type;

  std::string get_path(){
    if(str.size() > 1) return str[1];
    return std::string();
  }
} typedef command_str;

using namespace dsa;

class Command {
 protected:
  command_str command;
  std::string current_path;

  ref_<DsLink> link;

  boost::mutex print_mutex;

  MessageStatus status;

  virtual void _clear() {};
  virtual void _print() {};
  virtual void _print_usage_info() {};
  virtual COMMAND_RETURN_TYPE _execute() {return COMMAND_RETURN_CONTINUE;};

  void print();
  void print_usage_info();

  Command(const command_str command,
          const std::string current_path,
          const ref_<DsLink> link);

  virtual ~Command(){};

 public:
  COMMAND_RETURN_TYPE execute();
  void clear();

  std::string get_current_path();

  static std::string merge_paths(const std::string &first, const std::string &second);
  static int wait_for_bool(int wait_time, const std::function<bool()>& callback);
};

#endif //CMDLINE_DSLINK_COMMAND_H
