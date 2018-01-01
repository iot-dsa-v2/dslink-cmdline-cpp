
#include "command_list.h"

std::vector<int> CommandList::_available_args_num_options() {
  return {0, 1};
}

void CommandList::_print_usage_info() {
  std::cout<<"Printing list usage info"<<std::endl;
}

COMMAND_RETURN_TYPE CommandList::_execute() {
  target_path = Command::merge_paths(current_path, cmd_data.get_path_str());

  set_invokable();
  incoming_list_cache = link->list(
      target_path.c_str(),
      [&](IncomingListCache &cache, const std::vector<string_> &str) {
        print_mutex.lock();
        this->cache = cache;
        this->changes = str;
        this->status = cache.get_status();
        print_mutex.unlock();
        print();
      });

  if(cmd_data.is_stream) return COMMAND_RETURN_WAIT;

  return COMMAND_RETURN_CONTINUE;
}

void CommandList::_clear() {
  if(incoming_list_cache){
    incoming_list_cache->destroy();
    incoming_list_cache.reset();
  }
}
void CommandList::_print() {
  auto map = cache.get_map();

  if(status == MessageStatus::OK)
    std::cout<<cmdlog::var<<Var(new VarMap(map)).to_json(1)<<cmdlog::endl;
  else
    std::cout<<cmdlog::var<<"Message Status : "<<get_status_str(status)<<cmdlog::endl;
}
