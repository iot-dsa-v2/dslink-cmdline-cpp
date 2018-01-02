
#ifndef CMDLINE_DSLINK_COMMAND_FILELIST_H
#define CMDLINE_DSLINK_COMMAND_FILELIST_H

#include <commands/core/command.h>

class CommandFileList : public Command {
 private:

 public:
  using Command::Command;

 protected:
  std::vector<int> _available_args_num_options() override;
  const char* _get_usage_info() override;
  COMMAND_RETURN_TYPE _execute() override;
  void _print() override;

};


#endif //CMDLINE_DSLINK_COMMAND_FILELIST_H
