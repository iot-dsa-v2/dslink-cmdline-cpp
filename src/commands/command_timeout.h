#ifndef CMDLINE_DSLINK_COMMAND_TIMEOUT_H
#define CMDLINE_DSLINK_COMMAND_TIMEOUT_H

#include "core/command.h"

class CommandTimeout : public Command {
 private:

 public:
  using Command::Command;

 protected:
  std::vector<int> _available_args_num_options() override;
  const char* _get_usage_info() override;
  COMMAND_RETURN_TYPE _execute() override;
};


#endif //CMDLINE_DSLINK_COMMAND_TIMEOUT_H
