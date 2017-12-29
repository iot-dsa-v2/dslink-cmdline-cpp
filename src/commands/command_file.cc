
#include "command_file.h"

#include <string>
#include <fstream>
#include <streambuf>

std::vector<int> CommandFile::_available_args_num_options() {
  return {3};
}

void CommandFile::_print_usage_info() {
  std::cout<<"loadtxt usage info"<<std::endl;
}

COMMAND_RETURN_TYPE CommandFile::_execute() {

  std::ifstream t(cmd_data.get_path_str());
  if(!t.is_open()){
    throw std::runtime_error("file does not exists");
  }

//  std::string str((std::istreambuf_iterator<char>(t)),
//                  std::istreambuf_iterator<char>());

  auto value = cmd_data.get_value_str();

  auto attr = cmd_data.get_attribute_str();

  if(value == "txt"){
    file_txts.emplace(attr, cmd_data.get_path_str());
  }
  else if(value == "bin"){
    file_bins.emplace(attr, cmd_data.get_path_str());
  }
  else{
    throw std::runtime_error("type is not txt or bin");
  }

  return COMMAND_RETURN_CONTINUE;
}



