
#include "command_invoke.h"
#include "message/request/invoke_request_message.h"
#include "stream/requester/incoming_invoke_stream.h"

std::vector<int> CommandInvoke::_available_args_num_options() {
  return {1, 2};
}

void CommandInvoke::_print_usage_info() {
  std::cout<<"Writing invoke usage info..."<<std::endl;
}

COMMAND_RETURN_TYPE CommandInvoke::_execute() {
  target_path = Command::merge_paths(current_path, cmd_data.get_path_str());

  ref_<InvokeRequestMessage> invoke_req = make_ref_<InvokeRequestMessage>();

  invoke_req->set_target_path(target_path);

  if(cmd_data.get_value_str().size() != 0){
    auto v = get_Var_from_str(cmd_data.get_value_str());
    //std::cout<<cmdlog::var<<v.to_json()<<cmdlog::endl;
    invoke_req->set_value(v);
  }

  set_invokable();
  stream = link->invoke(
      [&](IncomingInvokeStream &stream, ref_<const InvokeResponseMessage> &&message) {
        print_mutex.lock();
        this->message = message;
        this->status = message->get_status();
        print_mutex.unlock();
        print();
      },
      std::move(invoke_req));

  return COMMAND_RETURN_CONTINUE;
}

void CommandInvoke::_clear() {
  if(stream != nullptr){
    stream->destroy();
  }
}

void CommandInvoke::_print() {
  print_message(message);
}
