
#ifndef CMDLINE_DSLINK_COMMAND_UNKNOWN_H
#define CMDLINE_DSLINK_COMMAND_UNKNOWN_H

#include "core/command.h"


class CommandHelp: public Command {
 private:
 public:
  using Command::Command;

 protected:
  const char* _get_usage_info() override;
  COMMAND_RETURN_TYPE _execute() override;
  void _clear() override;
  void _print() override;
};

#endif //CMDLINE_DSLINK_COMMAND_UNKNOWN_H
