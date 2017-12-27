#ifndef CMDLINE_DSLINK_COMMAND_CD_H
#define CMDLINE_DSLINK_COMMAND_CD_H

#include "command.h"

class CommandCD: public Command {
 private:
  std::string target_path;

 public:
  CommandCD(const command_str &command,
            const std::string &current_path,
            const ref_<DsLink> &link);

 protected:

  void _print() override;

  void _print_usage_info() override;

  COMMAND_RETURN_TYPE _execute() override;

  void _clear() override;

};

#endif //CMDLINE_DSLINK_COMMAND_CD_H
