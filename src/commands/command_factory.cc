
#include "command_factory.h"

#include "command_unknown.h"
#include "command_quit.h"

CommandFactory::CommandFactory() {
  command_str_map.insert(std::pair<std::string, COMMAND_TYPE>("quit", COMMAND_QUIT));
  command_str_map.insert(std::pair<std::string, COMMAND_TYPE>("cd", COMMAND_CD));
  command_str_map.insert(std::pair<std::string, COMMAND_TYPE>("list", COMMAND_LIST));
  command_str_map.insert(std::pair<std::string, COMMAND_TYPE>("invoke", COMMAND_INVOKE));
  command_str_map.insert(std::pair<std::string, COMMAND_TYPE>("set", COMMAND_SET));
  command_str_map.insert(std::pair<std::string, COMMAND_TYPE>("subscribe", COMMAND_SUBSCRIBE));
}

command_str CommandFactory::tokenize(std::string _line) {
  auto line = std::string(_line);

  command_str cmd;
  cmd.token_error = false;
  cmd.type = COMMAND_UNKNOWN;

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

  if(cmd.str.size() > 0)
    cmd.type = command_str_map[cmd.str[0]];

  return std::move(cmd);
}

std::shared_ptr<Command> CommandFactory::get_command(std::string _line,
                                                     std::string current_path,
                                                     ref_<DsLink> link) {
  auto command_str = tokenize(_line);



  switch (command_str.type){
    case COMMAND_UNKNOWN:
      return make_shared_<CommandUnknown>(command_str, current_path, link);
    case COMMAND_QUIT:
      return make_shared_<CommandQuit>(command_str, current_path, link);
    case COMMAND_CD:
      break;
    case COMMAND_LIST:
      break;
    case COMMAND_INVOKE:
      break;
    case COMMAND_SET:
      break;
    case COMMAND_SUBSCRIBE:
      break;
  }

  return make_shared_<CommandUnknown>(command_str, current_path, link);
}

