#ifndef CMDLINE_DSLINK_COMMAND_CD_H
#define CMDLINE_DSLINK_COMMAND_CD_H

#include "command.h"

class CommandCD: public Command {
 private:
  std::string target_path;

 public:
  using Command::Command;

 protected:
  void _print_usage_info() override;
  COMMAND_RETURN_TYPE _execute() override;
  void _clear() override;
  void _print() override;
};

#endif //CMDLINE_DSLINK_COMMAND_CD_H
