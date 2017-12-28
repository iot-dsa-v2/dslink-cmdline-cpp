
#include "command_factory.h"

#include "command_unknown.h"
#include "command_quit.h"
#include "command_list.h"
#include "command_cd.h"
#include "command_subscribe.h"
#include "command_set.h"
#include "command_invoke.h"

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
}

command_str CommandFactory::tokenize(std::string _line) {
  auto line = std::string(_line);

  command_str cmd;

  while(line.size()!=0) {
    auto char_ = line[0];
    line.erase(0, 1);

    if(char_ == '\n') break;
    if(char_ == ' ')  continue;

    std::size_t pos;

    switch(char_){
      case '{':
        pos = line.find('}');
        if(pos < line.size()) pos++;
        break;

      case '[':
        pos = line.find(']');
        if(pos < line.size()) pos++;
        break;

      case '"':
        pos = line.find('"');
        if(pos < line.size()) pos++;
        break;

      default:
        pos = line.find(' ');
        if(pos > line.size()) pos = line.find('\n');
        break;
    }

    if(pos > line.size()){
      cmd.token_error = true;
      break;
    }

    cmd.str.push_back(char_ + line.substr(0, pos));

    line.erase(0, pos);
  }

  if(cmd.str.size() > 0){
    if(cmd.str[0] == "debug"){
      cmd.is_debug = true;
      cmd.str.erase(cmd.str.begin());
    }
  }

  if(cmd.str.size() > 0){
    if(cmd.str[0] == "stream"){
      cmd.is_stream = true;
      cmd.str.erase(cmd.str.begin());
    }
  }

  if(cmd.str.size() > 0)
    cmd.type = command_str_map[cmd.str[0]];

  return std::move(cmd);
}

std::shared_ptr<Command> CommandFactory::get_command(
    std::string _line, std::string current_path, ref_<DsLink> link) {

  auto command_str = tokenize(_line);

  switch (command_str.type){
    case COMMAND_QUIT:
      return make_shared_<CommandQuit>(command_str, current_path, link);
    case COMMAND_CD:
      return make_shared_<CommandCD>(command_str, current_path, link);
    case COMMAND_LIST:
      return make_shared_<CommandList>(command_str, current_path, link);
    case COMMAND_SUBSCRIBE:
      return make_shared_<CommandSubscribe>(command_str, current_path, link);
    case COMMAND_INVOKE:
      return make_shared_<CommandInvoke>(command_str, current_path, link);
    case COMMAND_SET:
      return make_shared_<CommandSet>(command_str, current_path, link);
  }

  return make_shared_<CommandUnknown>(command_str, current_path, link);
}

