#include "dsa_common.h"

#include "command_factory.h"

#include <commands/command_timeout.h>
#include "commands/command_quit.h"
#include "commands/command_list.h"
#include "commands/command_cd.h"
#include "commands/command_subscribe.h"
#include "commands/command_set.h"
#include "commands/command_invoke.h"
#include "commands/command_help.h"

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
  command_str_map.insert(std::pair<std::string, COMMAND_TYPE>("help", COMMAND_HELP));
  command_str_map.insert(std::pair<std::string, COMMAND_TYPE>("?", COMMAND_HELP));
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
    case COMMAND_HELP:
      return make_shared_<CommandHelp>(cmd);
  }

  return make_shared_<CommandHelp>(cmd);
}

void CommandFactory::print_info(command_data cmd_data){
  auto which = cmd_data.command_str;
  if(which == "stream") {
    auto p =
        "\t# stream prefix\n"
            "\tAdd stream in front of subscribe or list, so the stream will be open unless user closes.\n\n"
            "```> stream list downstream```\n"
            "\tlistens downstream and print if any changes\n\n"
            "```> stream subs downstream/my_node/value```\n"
            "\tlistens downstream and print if any changes\n\n";
    Command::print_usage_static(p);
    return;
  } else if(which == "debug") {
    auto p =
        "\t# debug prefix\n"
            "\tOpen main dslink trace to see whats going on the inside of dslink.\n\n"
            "```> debug list downstream```\n"
            "\twhile listing downstream see what is going on in dslink\n\n";
    Command::print_usage_static(p);
    return;
  }

  cmd_data.type = command_str_map[cmd_data.command_str];

  switch (cmd_data.type){
    case COMMAND_QUIT:
      CommandQuit(cmd_data).print_usage_info();
      break;
    case COMMAND_CD:
      CommandCD(cmd_data).print_usage_info();
      break;
    case COMMAND_LIST:
      CommandList(cmd_data).print_usage_info();
      break;
    case COMMAND_SUBSCRIBE:
      CommandSubscribe(cmd_data).print_usage_info();
      break;
    case COMMAND_INVOKE:
      CommandInvoke(cmd_data).print_usage_info();
      break;
    case COMMAND_SET:
      CommandSet(cmd_data).print_usage_info();
      break;
    case COMMAND_TIMEOUT:
      CommandTimeout(cmd_data).print_usage_info();
      break;
  }
}
