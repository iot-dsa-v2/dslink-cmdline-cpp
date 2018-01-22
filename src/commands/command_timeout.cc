#include "dsa_common.h"

#include "command_timeout.h"
std::vector<int> CommandTimeout::_available_args_num_options() {
  return {1};
}

const char* CommandTimeout::_get_usage_info() {
  return
          "\tSometimes server don't respond so cmd waits TIMEOUT milisecond and than "
          "inform the user about it and gives a chance to cancel it. You can set "
          "the time between begining command and asking user to cancel it.\n\n"
          "```> timeout 2000```\n"
          "\tsets 2 seconds\n\n";
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
