
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include "command.h"
#include "message/base_message.h"

std::string Command::current_path = "";
ref_<DsLink> Command::link = nullptr;
int Command::timeout_in_ms = 1000;

Command::Command(const command_data cmd_data) {
  this->cmd_data = cmd_data;
  // default is continue
  return_type = COMMAND_RETURN_CONTINUE;
  // default is false
  invoked = true;
}

void Command::print_usage_info() {
  std::cout<<"Printing Usage Info..."<<std::endl;
  _print_usage_info();
  std::cout<<"...Printed Usage Info"<<std::endl;
}

void Command::print() {
  set_invoked();

  // clear screen is stream
  if(cmd_data.is_stream){
    for(int i = 0; i < 100; i++) std::cout<<std::endl;
  }

  print_mutex.lock();
  _print();
  print_mutex.unlock();

  // inform user how he can close stream
  if(cmd_data.is_stream){
    std::cout<<cmdlog::stream<< "Press enter to cancel stream..."<<std::endl;
  }
}


void Command::execute() {
  // check command
  if(cmd_data.token_error){
    std::cout<<cmdlog::error<<"There is an error on tokenizer."<<cmdlog::endl;
    print_usage_info();
    return;
  }

  // Check argument number is valid
  auto avaib_args = _available_args_num_options();
  if(avaib_args.size() != 0) {
    auto provid_arg = cmd_data.args.size();

    if (std::find(avaib_args.begin(), avaib_args.end(), provid_arg) == avaib_args.end()) {
      std::cout<<cmdlog::error<<"There is an error on argument numbers."<<cmdlog::endl;
      print_usage_info();
      return;
    }
  }

  try{
    return_type = _execute();
    if(return_type == COMMAND_RETURN_WAIT){
      std::cout<<cmdlog::stream<<"We are waiting server to respond..."<<cmdlog::endl;
    }
    return;
  }catch(std::exception &e){
    std::cout<<e.what()<<std::endl;
    print_usage_info();
    return;
  }
}

void Command::clear() {
  _clear();
}

void Command::print_message(const ref_<const ResponseMessage> message){
  if(message->get_body() != nullptr)
    message->print_message(std::cout, 0);
  else
    std::cout<<"Body return null, probably you wanted to subscribe invalid path";
  std::cout<<std::endl;
}

std::string Command::merge_paths(const std::string &first_, const std::string &second_) {
  std::vector<std::string> total_path_div;

  // Divide first and add to total path
  std::vector<std::string> first_div;
  boost::split(first_div, first_, boost::is_any_of("/"));
  while(!first_div.empty()){
    auto current = first_div[0];
    first_div.erase(first_div.begin());

    if(current.compare("") != 0)
      total_path_div.push_back(current);
  }

  // Divide second add to total path
  std::vector<std::string> second_div;
  boost::split(second_div, second_, boost::is_any_of("/"));
  while(!second_div.empty()){
    auto current = second_div[0];
    second_div.erase(second_div.begin());

    if(current.compare("") != 0)
      total_path_div.push_back(current);
  }

  std::vector<std::string> total_path_filtered;
  // apply .. to the path
  while(!total_path_div.empty()){
    auto current = total_path_div[0];
    total_path_div.erase(total_path_div.begin());

    if(current.compare(".") == 0){}
    else if(current.compare("..") == 0){
      if(!total_path_filtered.empty())
        total_path_filtered.pop_back();
    }
    else{
      total_path_filtered.push_back(current);
    }
  }

  std::string total_path;

  while(!total_path_filtered.empty()){
    total_path = total_path + total_path_filtered[0];
    total_path_filtered.erase(total_path_filtered.begin());
    if(!total_path_filtered.empty())
      total_path = total_path + "/";
  }

  return total_path;
}

const int SLEEP_INTERVAL = 10;
int Command::wait_for_bool(const std::function<bool()> &callback) {
  int wait_time = timeout_in_ms;
  int waited = 0;
  while (waited < wait_time) {
    if (callback()) {
      return waited;
    }
    boost::this_thread::sleep(boost::posix_time::milliseconds(SLEEP_INTERVAL));
    waited += SLEEP_INTERVAL;
  }
  return -1;
}

Var Command::get_Var_from_str(const std::string str) {
  if (str.size() == 0)
    throw std::runtime_error("str is null for turning into VAR ");

  // It is a hack for reading value in json
  try {
    Var v = Var::from_json("{\"var\":" + str + "}");
    return v["var"];
  } catch (
      std::exception e
  ) {
    throw std::runtime_error("error in json turning your value into dsa::Var : ");
  }
}