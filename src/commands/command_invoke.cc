
#include "command_invoke.h"
#include "message/request/invoke_request_message.h"
#include "stream/requester/incoming_invoke_stream.h"

void CommandInvoke::_print_usage_info() {
  std::cout<<"Writing invoke usage info..."<<std::endl;
}

COMMAND_RETURN_TYPE CommandInvoke::_execute() {
  if(command.num_args() != 2) {
    throw std::runtime_error("there must be 3 variable for invoke");
  }

  target_path = Command::merge_paths(current_path, command.get_path());

  ref_<InvokeRequestMessage> invoke_req = make_ref_<InvokeRequestMessage>();
  invoke_req->set_target_path(target_path);

  invoke_req->set_value(command.get_value());

  bool is_triggered = false;

  auto inv = link->invoke(
      [&](IncomingInvokeStream &stream, ref_<const InvokeResponseMessage> &&message) {
        print_mutex.lock();
        is_triggered = true;
        this->message = message;
        this->status = message->get_status();
        print_mutex.unlock();
        print();
      },
      std::move(invoke_req));

  Command::wait_for_bool(2000, [&]()->bool{return is_triggered;});
  inv->destroy();

  if(is_triggered){
    if(message->get_body() != nullptr)
      message->print_message(std::cout, 0);
    else
      std::cout<<"Body return null, probably you wanted to invoke on invalid path";
  }else{
    std::cout<<"not triggered, server does not gives any sound.";
  }

  return COMMAND_RETURN_CONTINUE;
}

void CommandInvoke::_clear() {

}

void CommandInvoke::_print() {

}