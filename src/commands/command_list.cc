#include "dsa_common.h"

#include "command_list.h"

std::vector<int> CommandList::_available_args_num_options() { return {0, 1}; }

const char *CommandList::_get_usage_info() {
  return "\t(list|ls|dir) Lists to given directory and close the stream after "
         "listing.\n\n"
         "```> ls```\n"
         "\tlists current directory\n\n"
         "```> ls .```\n"
         "\tlists current directory\n\n"
         "```> ls TARGET_PATH```\n"
         "\tlist to target path\n\n"
         "If you put \"stream\" in front it "
         "like ```> stream list PATH``` "
         "or ```> stream list``` it doesn't close "
         "the stream so you can take updates in real time from server.\n\n";
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

  if (cmd_data.is_stream) return COMMAND_RETURN_WAIT;

  return COMMAND_RETURN_CONTINUE;
}

void CommandList::_clear() {
  if (incoming_list_cache) {
    auto l = incoming_list_cache;
    incoming_list_cache.reset();
    link->strand->post([l]() { l->destroy(); });

    wait_for_bool([l]() -> bool { return l->is_destroyed(); });
  }
}

void CommandList::_print() {
  auto map = cache.get_map();

  if (status == MessageStatus::OK)
    std::cout << cmdlog::var << Var(new VarMap(map)).to_json(1) << cmdlog::endl;
  else
    std::cout << cmdlog::var << "Message Status : " << to_string(status)
              << cmdlog::endl;
}
