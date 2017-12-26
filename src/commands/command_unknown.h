
#ifndef CMDLINE_DSLINK_COMMAND_UNKNOWN_H
#define CMDLINE_DSLINK_COMMAND_UNKNOWN_H

#include "command.h"

class CommandUnknown: public Command {

 public:
  CommandUnknown(const command_str &command,
                 const std::string &current_path,
                 const ref_<DsLink> &link);

 protected:

  void _print() override;

  void _print_usage_info() override;

  COMMAND_RETURN_TYPE _execute() override;

  void _clear() override;


};

#endif //CMDLINE_DSLINK_COMMAND_UNKNOWN_H
