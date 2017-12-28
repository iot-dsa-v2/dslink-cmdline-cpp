
#include "command_cd.h"

CommandCD::CommandCD(const command_str &command,
                     const std::string &current_path,
                     const ref_<DsLink> &link):
    Command(command, current_path, link) {}

void CommandCD::_print() {
}

void CommandCD::_print_usage_info() {
  std::cout<<"Printing cd usage info"<<std::endl;
}

COMMAND_RETURN_TYPE CommandCD::_execute() {
  if(command.num_args() > 1){
    throw std::runtime_error("There are more than 1 argument.");
  }

  // There is special case if only cd is typed. We are returning back to the root
  if(command.get_path().size() != 0)
    target_path = Command::merge_paths(current_path, command.get_path());
  else
    target_path = std::string("");

  bool is_triggered = false;
  auto incoming_cache = link->list(
      target_path.c_str(),
      [&](IncomingListCache &cache, const std::vector<string_> &str) {
        this->status = cache.get_status();
        is_triggered = true;
        });

  Command::wait_for_bool(2000, [&]()->bool{return is_triggered;});
  if(is_triggered && status == MessageStatus::OK){
    std::cout<<"Current directory will changing to : "<<target_path;
    if(target_path.size() == 0)
      std::cout<<"root";
    std::cout<<std::endl;
    current_path = target_path;
  }
  else{
    std::cout<<"Current directory Can NOT changing to : "<<target_path<<", it is invalid"<<std::endl;
  }

  incoming_cache->destroy();

  return COMMAND_RETURN_CONTINUE;
}

void CommandCD::_clear() {
}
