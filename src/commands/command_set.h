
#ifndef CMDLINE_DSLINK_COMMAND_SET_H
#define CMDLINE_DSLINK_COMMAND_SET_H

#include "core/command.h"
#include <dsa/stream.h>

class CommandSet : public Command {
 private:
  ref_<IncomingSetStream> stream;

  ref_<const SetResponseMessage> message;
  std::string target_path;

 public:
  using Command::Command;

 protected:
  std::vector<int> _available_args_num_options() override;
  void _print_usage_info() override;
  COMMAND_RETURN_TYPE _execute() override;
  void _clear() override;
  void _print() override;
};

#endif //CMDLINE_DSLINK_COMMAND_SET_H
