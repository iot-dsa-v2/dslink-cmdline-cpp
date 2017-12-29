
#ifndef CMDLINE_DSLINK_COMMAND_LOADTXT_H
#define CMDLINE_DSLINK_COMMAND_LOADTXT_H

#include <commands/core/command.h>

class CommandFile : public Command {
 private:

 public:
  using Command::Command;

 protected:
  std::vector<int> _available_args_num_options() override;
  void _print_usage_info() override;
  COMMAND_RETURN_TYPE _execute() override;
};


#endif //CMDLINE_DSLINK_COMMAND_LOADTXT_H
