#include "dsa_common.h"
#include "command_subscribe.h"


std::vector<int> CommandSubscribe::_available_args_num_options() {
  return {1};
}

const char* CommandSubscribe::_get_usage_info() {
  return
          "\t(subscribe|subs) Subcribes to the path\n\n"
          "```>subscribe PATH```\n"
          "\tsubscribe to the path\n\n"
          "If you put \"stream\" in front it like stream ```> subs PATH``` "
          "it doesn't close the stream so you can take updates in real time from server.\n\n";
}

COMMAND_RETURN_TYPE CommandSubscribe::_execute() {
  target_path = Command::merge_paths(current_path, cmd_data.get_path_str());

  SubscribeOptions update_options;
  update_options.queue_duration = 0x9876;
  update_options.queue_size = 0x5432;

  set_invokable();
  incoming_subscribe_cache = link->subscribe(
      target_path.c_str(),
      [&](IncomingSubscribeCache &cache, ref_<const SubscribeResponseMessage> message) {
        print_mutex.lock();
        this->cache = cache;
        this->message = message;
        this->status = message->get_status();
        print_mutex.unlock();
        print();
      },
      update_options);

  if(cmd_data.is_stream) return COMMAND_RETURN_WAIT;

  return COMMAND_RETURN_CONTINUE;
}

void CommandSubscribe::_clear() {
  if (incoming_subscribe_cache) {
    auto l = incoming_subscribe_cache;
    incoming_subscribe_cache.reset();
    link->strand->post([l]() { l->destroy(); });

    wait_for_bool([l]() -> bool { return l->is_destroyed(); });
  }
}

void CommandSubscribe::_print() {
  std::cout<<cmdlog::info<<"Printing subscribe message from path : "
           <<cmdlog::path<<target_path<<std::endl;
  print_message(message);
}

