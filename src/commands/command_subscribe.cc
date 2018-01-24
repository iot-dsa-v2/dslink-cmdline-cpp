#include "dsa_common.h"
#include "command_subscribe.h"

#include <boost/algorithm/string.hpp>

std::vector<int> CommandSubscribe::_available_args_num_options() {
  return {1,2,3,4,5};
}

const char* CommandSubscribe::_get_usage_info() {
  return
          "\t(subscribe|subs) Subscribes to the path. You can give options for (level|size|duration|priority)\n\n"
          "```>subscribe PATH```\n"
          "\tsubscribe to the path\n\n"
          "```>subscribe PATH level=0```\n"
          "\tsubscribe to the path with level 0\n\n"
          "```>subscribe PATH priority=false```\n"
          "\tsubscribe to the path with priority false\n\n"
          "```>subscribe PATH size=256 priority=false```\n"
          "\tsubscribe to the path with size 256 and priority false\n\n"
          "```>subscribe PATH level=0 size=244 duration=255 priority=true```\n"
          "\tall options usage example\n\n"

          "If you put \"stream\" in front it like stream ```> subs PATH``` "
          "it doesn't close the stream so you can take updates in real time from server.\n\n";
}


COMMAND_RETURN_TYPE CommandSubscribe::_execute() {
  target_path = Command::merge_paths(current_path, cmd_data.get_path_str());

  // Default values
  SubscribeOptions update_options;

  // User gave options process them
  if(cmd_data.args.size() > 1){
    // Copy
    auto opts = std::vector<string_>(cmd_data.args);
    // remove first which means path
    opts.erase(opts.begin());

    bool result = update_subscribe_options(opts, update_options);
    if(!result){
      print_usage_info();
      return COMMAND_RETURN_CONTINUE;
    }
  }

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

SubscribeOptStr CommandSubscribe::get_enum_from_str(string_ str){

  if(str == "priority")
    return SubscribeOptStr::PRIORITY;

  if(str == "level")
    return SubscribeOptStr::LEVEL;

  if(str == "duration")
    return SubscribeOptStr::DURATION;

  if(str == "size")
    return SubscribeOptStr::SIZE;

  return SubscribeOptStr::UNKNOWN;
}


bool CommandSubscribe::update_subscribe_options(std::vector<string_> strs, SubscribeOptions& options){
  for(auto str:strs) {
    std::vector<string_> divided;
    boost::split(divided, str, boost::is_any_of("="));

    if (divided.size() != 2) {
      std::cout << cmdlog::error << "Error while tokanize on " << cmdlog::path << str << cmdlog::endl;
      return false;
    }

    // Check availability of options
    auto opt = get_enum_from_str(divided[0]);

    if(opt == SubscribeOptStr::UNKNOWN){
      std::cout << cmdlog::error << "Option couldn't be understanded. "
          "It can be (priority|level|size|duration) but it is " << cmdlog::path << divided[0] << cmdlog::endl;
      return false;
    }

    // First check priority because it has bool so different from others(integer)
    if(opt == SubscribeOptStr::PRIORITY){
      auto must_bool = divided[1];
      if(must_bool == "true"){
        options.priority = true;
      }
      else if(must_bool == "false"){
        options.priority = false;
      }
      else{
        std::cout << cmdlog::error << "Priority must be either true or false. "
            "It is " << cmdlog::path << must_bool << cmdlog::endl;
        return false;
      }
      continue;
    }

    // Value should be integer in this case so we are turning into integer
    int val;
    try{
      val = std::stoi(divided[1]);
    }
    catch(std::invalid_argument a){
      std::cout <<cmdlog::error << "You didn't give integer. You gave "
                <<cmdlog::path << divided[1] << cmdlog::endl;
      return false;
    }

    switch(opt){
      case SubscribeOptStr::LEVEL:
        if(val < 0 || val > 3){
          std::cout <<cmdlog::error << "Level must be between [0,3], you gave "
                    <<cmdlog::path << val << cmdlog::endl;
          return false;
        }
        options.qos = (QosLevel)val;
        break;

      case SubscribeOptStr::DURATION:
        options.queue_duration = val;
        break;

      case SubscribeOptStr::SIZE:
        options.queue_size = val;
        break;
    }

  }

  return true;
}
