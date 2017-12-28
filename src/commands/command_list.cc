
#include "command_list.h"

void CommandList::_print_usage_info() {
  std::cout<<"Printing list usage info"<<std::endl;
}

COMMAND_RETURN_TYPE CommandList::_execute() {
  if(command.num_args() > 1){
    throw std::runtime_error("There are more than 1 argument.");
  }

  target_path = Command::merge_paths(current_path, command.get_path());

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

  return COMMAND_RETURN_WAIT;
}

void CommandList::_clear() {
  if(incoming_list_cache){
    incoming_list_cache->destroy();
    incoming_list_cache.reset();
  }
}

void CommandList::_print() {
  auto map = cache.get_map();

  std::cout<<"Listing path : "<<target_path<<std::endl;

  std::cout<<Var(new VarMap(map)).to_json(1)<<std::endl;
}
