
#ifndef CMDLINE_DSLINK_COMMAND_SUBSCRIBE_H
#define CMDLINE_DSLINK_COMMAND_SUBSCRIBE_H

#include "command.h"

class CommandSubscribe : public Command {
 private:
  ref_<IncomingSubscribeCache>  incoming_subscribe_cache;
  IncomingSubscribeCache cache;
  ref_<const SubscribeResponseMessage> message;

  std::string target_path;

 public:
  CommandSubscribe(const command_str &command,
                   const std::string &current_path,
                   const ref_<DsLink> &link);

 protected:

  void _print() override;

  void _print_usage_info() override;

  COMMAND_RETURN_TYPE _execute() override;

  void _clear() override;
};

#endif //CMDLINE_DSLINK_COMMAND_SUBSCRIBE_H
