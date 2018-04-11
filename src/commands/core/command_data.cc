#include "dsa_common.h"

#include "command_data.h"

command_data tokenize(std::string _line) {
  auto line = std::string(_line);

  command_data cmd;
  std::vector<std::string> tokens;

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

//    if(pos > line.size()){
//      cmd.token_error = true;
//      break;
//    }

    tokens.push_back(char_ + line.substr(0, pos));

    line.erase(0, pos);
  }

  if(tokens.size() > 0){
    if(tokens[0] == "debug"){
      cmd.is_debug = true;
      tokens.erase(tokens.begin());
    }
  }

  if(tokens.size() > 0){
    if(tokens[0] == "stream"){
      cmd.is_stream = true;
      tokens.erase(tokens.begin());
    }
  }

  if(tokens.size() > 0){
    cmd.command_str = tokens[0];
    tokens.erase(tokens.begin());
  }

  cmd.args = std::move(tokens);

  return std::move(cmd);
}
