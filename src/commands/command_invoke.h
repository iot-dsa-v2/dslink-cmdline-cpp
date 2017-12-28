
#ifndef CMDLINE_DSLINK_COMMAND_INVOKE_H
#define CMDLINE_DSLINK_COMMAND_INVOKE_H

#include "command.h"

class CommandInvoke: public Command {
 private:

 public:
  using Command::Command;

 protected:
  void _print_usage_info() override;
  COMMAND_RETURN_TYPE _execute() override;
  void _clear() override;
  void _print() override;
};

#endif //CMDLINE_DSLINK_COMMAND_INVOKE_H
