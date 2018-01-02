
#include "command_file.h"

#include <string>
#include <fstream>
#include <streambuf>

std::vector<int> CommandFile::_available_args_num_options() {
  return {3};
}

const char* CommandFile::_get_usage_info() {
  return
      "\tSave file path from local machine for later use in Var.\n\n"
      "```> file PATH bin PLACEHOLDERNAME``` \n"
      "\tstores PATH as named PLACEHOLDER with noted as bin. "
      "It should be called if you want to send data as binary\n\n"
      "```> file PATH txt PLACEHOLDERNAME``` \n"
      "\tstores PATH as named PLACEHOLDER with noted as txt. "
      "It should be called if you want to send data as string.\n\n"
      "You can use these while you are giving a Var to a command. "
      "For example if we have json tex file we can load it "
      "like ```> file my.json txt myjson``` so you can invoke "
      "like ```> invoke sys/stop 'myjson'``` It reads file "
      "content in txt and puts there. Placeholders in commands "
      "should be encapsulated with '' You can write nested Var "
      "like ```> invoke sys/stop {\"key\":'myjson'}```. Binary is "
      "the same : ```> file lena.jpg bin my_image```\n\n";
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



