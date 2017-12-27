
#ifndef CMDLINE_DSLINK_COMMAND_LIST_H
#define CMDLINE_DSLINK_COMMAND_LIST_H

#include "command.h"

class CommandList: public Command {
 private:
  ref_<IncomingListCache> incoming_list_cache;
  IncomingListCache cache;
  std::vector<std::string> changes;
  std::string target_path;
 public:
  CommandList(const command_str &command,
              const std::string &current_path,
              const ref_<DsLink> &link);

 protected:

  void _print() override;

  void _print_usage_info() override;

  COMMAND_RETURN_TYPE _execute() override;

  void _clear() override;
};

#endif //CMDLINE_DSLINK_COMMAND_LIST_H
