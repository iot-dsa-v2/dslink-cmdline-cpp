#include "dsa_common.h"

#include "command_cd.h"

std::vector<int> CommandCD::_available_args_num_options() { return {1}; }

const char *CommandCD::_get_usage_info() {
  return "\tChanges current directory to given directory. If there is '/' "
         "given in beginning it overrides your current directory.\n\n"
         "```> cd /```\n"
         "\tnavigates to the root\n\n"
         "```> cd TARGET_PATH```\n"
         "\tnavigates to target path\n\n"
         "```> cd ..```\n"
         "\tnavigates to one level back\n\n"
         "```> cd .```\n"
         "\tdoes nothing, you are still same directory\n\n";
}

COMMAND_RETURN_TYPE CommandCD::_execute() {
  // There is special case if only cd is typed. We are returning back to the
  // root
  if (cmd_data.get_path_str().size() != 0)
    target_path = Command::merge_paths(current_path, cmd_data.get_path_str());
  else
    target_path = std::string("");

  set_invokable();

  link->strand->post([ this, keepptr = shared_from_this() ]() {

    cache =
        dynamic_cast<DsLinkRequester *>(link.get())->list(target_path.c_str(), [
          this, keepptr = std::move(keepptr)
        ](IncomingListCache & cache, const std::vector<string_> &str) {
          print_mutex.lock();
          this->status = cache.get_status();
          print_mutex.unlock();
          print();
        });
  });

  return COMMAND_RETURN_CONTINUE;
}

void CommandCD::_clear() {
  if (cache != nullptr) {
    auto l = this->cache;
    this->cache.reset();

    link->strand->post([l]() { l->destroy(); });
    wait_for_bool([l]() -> bool { return l->is_destroyed(); });
  }
}

void CommandCD::_print() {
  if (status == Status::OK) {
    current_path = target_path;
  } else {
    std::cout << cmdlog::error << "Failed to change path to " << cmdlog::path
              << target_path << cmdlog::reset << " maybe it is invalid\n"
              << "Message status : " << to_string(status) << std::endl;
  }
}
