
#ifndef CMDLINE_DSLINK_COMMAND_SUBSCRIBE_H
#define CMDLINE_DSLINK_COMMAND_SUBSCRIBE_H

#include "core/command.h"

namespace subscribe_command{
enum SubscribeOptStr{
  LEVEL,
  SIZE,
  DURATION,
  PRIORITY,
  UNKNOWN
};
}

class CommandSubscribe : public Command {
 private:
  ref_<IncomingSubscribeCache>  incoming_subscribe_cache;
  ref_<const SubscribeResponseMessage> message;
  std::string target_path;

  static subscribe_command::SubscribeOptStr get_enum_from_str(string_ str);
  static bool update_subscribe_options(std::vector<string_> strs, SubscribeOptions& options);


 public:
  using Command::Command;

 protected:
  std::vector<int> _available_args_num_options() override;
  const char* _get_usage_info() override;
  COMMAND_RETURN_TYPE _execute() override;
  void _clear() override;
  void _print() override;
};

#endif //CMDLINE_DSLINK_COMMAND_SUBSCRIBE_H
