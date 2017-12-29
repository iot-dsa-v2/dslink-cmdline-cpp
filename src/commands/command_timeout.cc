//
// Created by mk on 12/29/17.
//

#include "command_timeout.h"
std::vector<int> CommandTimeout::_available_args_num_options() {
  return {1};
}

void CommandTimeout::_print_usage_info() {
  std::cout<<"time out usage"<<std::endl;
}

COMMAND_RETURN_TYPE CommandTimeout::_execute() {
  auto s = cmd_data.get_path_str();

  bool has_only_digits = (s.find_first_not_of( "0123456789" ) == std::string::npos);
  if(!has_only_digits)
    throw std::runtime_error("argument is not int");

  int time = std::stoi( s );

  timeout_in_ms = time;

  return COMMAND_RETURN_CONTINUE;
}
