
#include "command_set.h"
#include "message/request/set_request_message.h"
#include "stream/requester/incoming_set_stream.h"

std::vector<int> CommandSet::_available_args_num_options() {
  return {2,3};
}

void CommandSet::_print_usage_info() {
  std::cout<<"Printing set usage info"<<std::endl;
}

COMMAND_RETURN_TYPE CommandSet::_execute() {

  auto request = make_ref_<SetRequestMessage>();

  // Target path set
  target_path = Command::merge_paths(current_path, cmd_data.get_path_str());
  request->set_target_path(target_path);

//  std::cout<<command.get_value().to_json(1)<<std::endl;
//  std::cout<<command.get_value().get_type()<<std::endl;
  request->set_value(get_Var_from_str(cmd_data.get_value_str()));

  // Attribute set
  auto attr = cmd_data.get_attribute_str();
  if(!attr.empty()) request->set_attribute_field(attr);

  bool is_triggered = false;

  auto set = link->set(
      [&](IncomingSetStream& stream, ref_<const SetResponseMessage> message) {
        print_mutex.lock();
        is_triggered = true;
        this->message = message;
        this->status = message->get_status();
        print_mutex.unlock();
        print();
      }, copy_ref_(request));

  Command::wait_for_bool([&]()->bool{return is_triggered;});
  set->destroy();

  if(is_triggered){
    if(message->get_body() != nullptr)
      message->print_message(std::cout, 0);
    else
      std::cout<<"Body return null, probably you wanted to set on invalid path";
  }else{
    std::cout<<"not triggered, server does not gives any sound.";
  }

  return COMMAND_RETURN_CONTINUE;
}

void CommandSet::_clear() {
}
void CommandSet::_print() {
}
