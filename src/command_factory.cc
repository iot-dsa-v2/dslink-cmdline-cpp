
#include <commands/command_timeout.h>
#include "command_factory.h"

#include "commands/command_unknown.h"
#include "commands/command_quit.h"
#include "commands/command_list.h"
#include "commands/command_cd.h"
#include "commands/command_subscribe.h"
#include "commands/command_set.h"
#include "commands/command_invoke.h"

CommandFactory::CommandFactory() {
  command_str_map.insert(std::pair<std::string, COMMAND_TYPE>("quit", COMMAND_QUIT));
  command_str_map.insert(std::pair<std::string, COMMAND_TYPE>("q", COMMAND_QUIT));
  command_str_map.insert(std::pair<std::string, COMMAND_TYPE>("exit", COMMAND_QUIT));
  command_str_map.insert(std::pair<std::string, COMMAND_TYPE>("cd", COMMAND_CD));
  command_str_map.insert(std::pair<std::string, COMMAND_TYPE>("list", COMMAND_LIST));
  command_str_map.insert(std::pair<std::string, COMMAND_TYPE>("ls", COMMAND_LIST));
  command_str_map.insert(std::pair<std::string, COMMAND_TYPE>("dir", COMMAND_LIST));
  command_str_map.insert(std::pair<std::string, COMMAND_TYPE>("invoke", COMMAND_INVOKE));
  command_str_map.insert(std::pair<std::string, COMMAND_TYPE>("inv", COMMAND_INVOKE));
  command_str_map.insert(std::pair<std::string, COMMAND_TYPE>("set", COMMAND_SET));
  command_str_map.insert(std::pair<std::string, COMMAND_TYPE>("subscribe", COMMAND_SUBSCRIBE));
  command_str_map.insert(std::pair<std::string, COMMAND_TYPE>("subs", COMMAND_SUBSCRIBE));
  command_str_map.insert(std::pair<std::string, COMMAND_TYPE>("timeout", COMMAND_TIMEOUT));
}

std::shared_ptr<Command> CommandFactory::get_command(std::string _line) {

  auto cmd = tokenize(_line);
  cmd.type = command_str_map[cmd.command_str];

  switch (cmd.type){
    case COMMAND_QUIT:
      return make_shared_<CommandQuit>(cmd);
    case COMMAND_CD:
      return make_shared_<CommandCD>(cmd);
    case COMMAND_LIST:
      return make_shared_<CommandList>(cmd);
    case COMMAND_SUBSCRIBE:
      return make_shared_<CommandSubscribe>(cmd);
    case COMMAND_INVOKE:
      return make_shared_<CommandInvoke>(cmd);
    case COMMAND_SET:
      return make_shared_<CommandSet>(cmd);
    case COMMAND_TIMEOUT:
      return make_shared_<CommandTimeout>(cmd);
  }

  return make_shared_<CommandUnknown>(cmd);
}

