#include "dsa_common.h"
#include "command_invoke.h"
#include "message/request/invoke_request_message.h"
#include "stream/requester/incoming_invoke_stream.h"

std::vector<int> CommandInvoke::_available_args_num_options() {
  return {1, 2};
}

const char* CommandInvoke::_get_usage_info() {
  return
          "(invoke|inv) a path from server.\n\n"
          "```> invoke PATH```\n"
          "\tinvoke PATH\n\n"
          "```> invoke PATH Var```\n"
          "\tinvoke PATH with given Var\n\n";
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

  link->strand->post([this, invoke_req = std::move(invoke_req)]() {

      stream = dynamic_cast<DsLinkRequester*>(link.get())->invoke(
              [&](IncomingInvokeStream &stream, ref_<const InvokeResponseMessage> &&message) {
                  print_mutex.lock();
                  this->message = message;
                  this->status = message->get_status();
                  print_mutex.unlock();
                  print();
              },
              std::move(invoke_req));
  });

  if(cmd_data.is_stream) return COMMAND_RETURN_WAIT;

  return COMMAND_RETURN_CONTINUE;
}

void CommandInvoke::_clear() {
  if (stream) {
    auto l = stream;
    stream.reset();
    link->strand->post([l]() {
      l->close();
    });

    wait_for_bool([l]() -> bool { return l->is_closed(); });
  }
}

void CommandInvoke::_print() {
  print_message(message);

  auto v = message->get_value();
  if(v.get_type() != Var::NUL)
    std::cout<<cmdlog::var<<v.to_json(0)<<cmdlog::endl;
}
