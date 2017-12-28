#ifndef CMDLINE_DSLINK_COMMAND_STR_H
#define CMDLINE_DSLINK_COMMAND_STR_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>

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

struct command_data{
  bool token_error;

  std::string command_str;
  COMMAND_TYPE type;

  std::vector<std::string> args;

  bool is_debug;
  bool is_stream;

  command_data(){
    token_error = false;

    type = COMMAND_UNKNOWN;

    is_debug = false;
    is_stream = false;
  }

  // PATH arg:0
  std::string get_path_str(){
    if(args.size() > 0) return args[0];
    return std::string();
  }

  // VALUE arg:1
  std::string get_value_str(){
    if(args.size() > 1) return args[1];
    return std::string();
  }

  // ATTRIBUTE arg:2
  std::string get_attribute_str(){
    if(args.size() > 2) return args[2];
    return std::string();
  }

} typedef command_data;


command_data tokenize(std::string _line);

#endif //CMDLINE_DSLINK_COMMAND_STR_H
