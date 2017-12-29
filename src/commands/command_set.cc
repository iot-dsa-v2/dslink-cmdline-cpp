
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

  set_invokable();
  stream = link->set(
      [&](IncomingSetStream& stream, ref_<const SetResponseMessage> message) {
        print_mutex.lock();
        this->message = message;
        this->status = message->get_status();
        print_mutex.unlock();
        print();
      }, copy_ref_(request));

  return COMMAND_RETURN_CONTINUE;
}

void CommandSet::_clear() {
  if(stream != nullptr){
    stream->destroy();
  }
}

void CommandSet::_print() {
  print_message(message);
}
