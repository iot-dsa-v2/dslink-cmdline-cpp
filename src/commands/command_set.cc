
#include "command_set.h"
#include "message/request/set_request_message.h"
#include "stream/requester/incoming_set_stream.h"

CommandSet::CommandSet(const command_str &command,
                       const std::string &current_path,
                       const ref_<DsLink> &link):
    Command(command, current_path, link) {}



void CommandSet::_print_usage_info() {
  std::cout<<"Printing set usage info"<<std::endl;
}
void CommandSet::_clear() {

}
void CommandSet::_print() {

}

COMMAND_RETURN_TYPE CommandSet::_execute() {

  if(command.num_args() != 2 && command.num_args() != 3){
    throw std::runtime_error("There must be 2-3 argument; path and value and optional attr.");
  }

  auto request = make_ref_<SetRequestMessage>();

  // Target path set
  target_path = Command::merge_paths(current_path, command.get_path());
  request->set_target_path(target_path);

//  std::cout<<command.get_value().to_json(1)<<std::endl;
//  std::cout<<command.get_value().get_type()<<std::endl;
  request->set_value(command.get_value());

  // Attribute set
  auto attr = command.get_attribute();
  if(!attr.empty()) request->set_attribute_field(attr);

  bool is_triggered = false;

  auto set_str = link->set(
      [&](IncomingSetStream& stream, ref_<const SetResponseMessage> message) {
        print_mutex.lock();
        is_triggered = true;
        this->message = message;
        this->status = message->get_status();
        print_mutex.unlock();
        print();
      }, copy_ref_(request));

  Command::wait_for_bool(2000, [&]()->bool{return is_triggered;});
  set_str->destroy();

  if(is_triggered){
    if(message->get_body() != nullptr)
      message->print_message(std::cout, 0);
    else
      std::cout<<"Body return null, probably you wanted to subscribe invalid path";
  }else{
    std::cout<<"not triggered, server does not gives any sound.";
  }

  return COMMAND_RETURN_CONTINUE;
}
