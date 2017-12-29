
#include "command_cd.h"

std::vector<int> CommandCD::_available_args_num_options() {
  return {0, 1};
}

void CommandCD::_print_usage_info() {
  std::cout<<"Printing cd usage info"<<std::endl;
}

COMMAND_RETURN_TYPE CommandCD::_execute() {
  // There is special case if only cd is typed. We are returning back to the root
  if(cmd_data.get_path_str().size() != 0)
    target_path = Command::merge_paths(current_path, cmd_data.get_path_str());
  else
    target_path = std::string("");

  set_invokable();
  cache = link->list(
      target_path.c_str(),
      [&](IncomingListCache &cache, const std::vector<string_> &str) {
        print_mutex.lock();
        this->status = cache.get_status();
        print_mutex.unlock();
        print();
      });

  return COMMAND_RETURN_CONTINUE;
}

void CommandCD::_clear() {
  if(cache != nullptr){
    cache->destroy();
  }
}

void CommandCD::_print() {
  if(status == MessageStatus::OK) {
    current_path = target_path;
  }
  else{
    std::cout<< cmdlog::error<<"Failed to change path to "
             << cmdlog::path<<target_path<<cmdlog::reset
             << " maybe it is invalid"<<std::endl;
  }
}
