
#ifndef CMDLINE_DSLINK_COMMAND_SET_H
#define CMDLINE_DSLINK_COMMAND_SET_H

#include <dsa/stream.h>
#include "command.h"

class CommandSet : public Command {
 private:
  ref_<const SetResponseMessage> message;

  std::string target_path;
 public:
  CommandSet(const command_str &command,
             const std::string &current_path,
             const ref_<DsLink> &link);

 protected:

  void _print() override;

  void _print_usage_info() override;

  COMMAND_RETURN_TYPE _execute() override;

  void _clear() override;
};

#endif //CMDLINE_DSLINK_COMMAND_SET_H
