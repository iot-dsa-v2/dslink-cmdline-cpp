
#include "command_subscribe.h"


CommandSubscribe::CommandSubscribe(const command_str &command,
                 const std::string &current_path,
                 const ref_<DsLink> &link):
    Command(command, current_path, link) {}


void CommandSubscribe::_print_usage_info() {
  std::cout<<"Printing subscribe usage info"<<std::endl;
}

void CommandSubscribe::_clear() {
  if(incoming_subscribe_cache != nullptr){
    incoming_subscribe_cache->destroy();
    incoming_subscribe_cache.reset();
  }
}

COMMAND_RETURN_TYPE CommandSubscribe::_execute() {
  if(command.num_args() > 1){
    throw std::runtime_error("There are more than 1 argument.");
  }

  target_path = Command::merge_paths(current_path, command.get_path());

  SubscribeOptions update_options;
  update_options.queue_duration = 0x9876;
  update_options.queue_size = 0x5432;

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

  return COMMAND_RETURN_WAIT;
}

void CommandSubscribe::_print() {
  std::cout<<"Subscribe message from path : "<<target_path<<std::endl;
  if(message->get_body() != nullptr)
    message->print_message(std::cout, 0);
  else
    std::cout<<"Body return null, probably you wanted to subscribe invalid path";
  std::cout<<std::endl;
}

