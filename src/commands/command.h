
#ifndef CMDLINE_DSLINK_COMMAND_H
#define CMDLINE_DSLINK_COMMAND_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>

#include "dslink.h"
#include "variant/variant.h"

using namespace dsa;

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
  bool token_error;

  std::vector<std::string> str;
  COMMAND_TYPE type;

  bool is_debug;
  bool is_stream;

  command_str(){
    token_error = false;

    type = COMMAND_UNKNOWN;

    is_debug = false;
    is_stream = false;
  }

  size_t num_args(){ return str.size() - 1; }

  std::string get_path(){
    if(num_args() > 0) return str[1];
    return std::string();
  }

  bool is_value_available(){
    return num_args() > 1;
  }

  Var get_value(){
    if(!is_value_available())
      return Var();

    // It is a hack for reading value in json
    try{
      Var v = Var::from_json("{\"var\":" + str[2]+ "}");
      return v["var"];
    }catch(std::exception e){
      throw std::runtime_error("error in json turning your value into dsa::Var : ");
    }
  }

  std::string get_attribute(){
    if(num_args() > 2) return str[3];
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

 public:
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
