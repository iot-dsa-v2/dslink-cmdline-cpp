#include "dsa_common.h"

#include "command.h"
#include "message/base_message.h"
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

#include "dsa_common.h"
#include <module/default/console_logger.h>

#include <boost/algorithm/string.hpp>
#include <fstream>
#include <streambuf>
#include <string>

#define BINARY_FILE_TEXT_BEGIN "binary_file("
#define TEXT_FILE_TEXT_BEGIN "text_file("

std::string Command::current_path = "";
ref_<DsLink> Command::link = nullptr;
int Command::timeout_in_ms = 1000;

Command::Command(const command_data cmd_data) {
  this->cmd_data = cmd_data;
  // default is continue
  return_type = COMMAND_RETURN_CONTINUE;
  // default is false
  invoked = true;
  informed = false;
}

void Command::print_usage_info() {
  std::cout << termcolor::yellow << "\n\n\n# ";
  if (cmd_data.type != COMMAND_UNKNOWN)
    std::cout << cmd_data.command_str;
  std::cout << " Usage Info\n" << termcolor::reset << std::endl;
  auto usage_info = _get_usage_info();
  print_usage_static(usage_info);
}

void Command::print() {
  set_invoked();

  if (!informed) {
    // inform user how he can close stream
    if (cmd_data.is_stream) {
      std::cout << cmdlog::stream << "Press enter to cancel stream..."
                << std::endl;
    }
    informed = true;
  }

  //  // clear screen is stream
  //  if(cmd_data.is_stream){
  //    for(int i = 0; i < 100; i++) std::cout<<std::endl;
  //  }

  print_mutex.lock();
  _print();
  print_mutex.unlock();
}

void Command::execute() {
  // check command
  if (cmd_data.token_error) {
    std::cout << cmdlog::error << "There is an error on tokenizer."
              << cmdlog::endl;
    print_usage_info();
    return;
  }

  // Check argument number is valid
  auto avaib_args = _available_args_num_options();
  if (avaib_args.size() != 0) {
    auto provid_arg = cmd_data.args.size();

    if (std::find(avaib_args.begin(), avaib_args.end(), provid_arg) ==
        avaib_args.end()) {
      std::cout << cmdlog::error << "There is an error on argument numbers."
                << cmdlog::endl;
      print_usage_info();
      return;
    }
  }

  // Debug?
  if (cmd_data.is_debug) {
    static_cast<ConsoleLogger &>(Logger::_()).level = Logger::ALL___;
  }

  try {
    return_type = _execute();
    if (return_type == COMMAND_RETURN_WAIT) {
      std::cout << cmdlog::stream << "We are waiting server to respond..."
                << cmdlog::endl;
    }
    return;
  } catch (std::exception &e) {
    std::cout << cmdlog::error << e.what() << cmdlog::endl;
    print_usage_info();
    return;
  }
}

void Command::clear() {
  print_mutex.lock();
  _clear();
  print_mutex.unlock();
  // Debug?
  if (cmd_data.is_debug) {
    static_cast<ConsoleLogger &>(Logger::_()).level = Logger::NONE__;
  }
}

void Command::print_message(const ref_<const ResponseMessage> message) {
  std::cout << cmdlog::var;
  auto status = message->get_status();
  std::cout << "Message Status : " << to_string(status) << std::endl;

  if (message->get_body() != nullptr)
    message->print_message(std::cout, 0);
  else
    std::cout << "Body: null";
  std::cout << cmdlog::endl;
}

std::string Command::merge_paths(const std::string &first__,
                                 const std::string &second_) {
  std::vector<std::string> total_path_div;

  // Check whether first is overloaded?
  auto first_ = first__;
  if (second_.size() > 0 && second_[0] == '/') {
    first_ = "";
  }

  // Divide first and add to total path
  std::vector<std::string> first_div;
  boost::split(first_div, first_, boost::is_any_of("/"));
  while (!first_div.empty()) {
    auto current = first_div[0];
    first_div.erase(first_div.begin());

    if (current.compare("") != 0)
      total_path_div.push_back(current);
  }

  // Divide second add to total path
  std::vector<std::string> second_div;
  boost::split(second_div, second_, boost::is_any_of("/"));
  while (!second_div.empty()) {
    auto current = second_div[0];
    second_div.erase(second_div.begin());
    current.erase(remove(current.begin(), current.end(), '\"'), current.end());
    if (current.compare("") != 0)
      total_path_div.push_back(current);
  }

  std::vector<std::string> total_path_filtered;
  // apply .. to the path
  while (!total_path_div.empty()) {
    auto current = total_path_div[0];
    total_path_div.erase(total_path_div.begin());

    if (current.compare(".") == 0) {
    } else if (current.compare("..") == 0) {
      if (!total_path_filtered.empty())
        total_path_filtered.pop_back();
    } else {
      total_path_filtered.push_back(current);
    }
  }

  std::string total_path;

  while (!total_path_filtered.empty()) {
    total_path = total_path + total_path_filtered[0];
    total_path_filtered.erase(total_path_filtered.begin());
    if (!total_path_filtered.empty())
      total_path = total_path + "/";
  }

  return total_path;
}

const int SLEEP_INTERVAL = 10;
int Command::wait_for_bool(const std::function<bool()> &callback) {
  int wait_time = timeout_in_ms;
  int waited = 0;
  while (waited < wait_time) {
    if (callback()) {
      return waited;
    }
    boost::this_thread::sleep(boost::posix_time::milliseconds(SLEEP_INTERVAL));
    waited += SLEEP_INTERVAL;
  }
  return -1;
}

std::vector<std::string> split_file_strings(const std::string _source) {

  auto source = _source;
  std::vector<std::string> separated_list;

  while (true) {
    auto position_bin = source.find(BINARY_FILE_TEXT_BEGIN);
    auto position_txt = source.find(TEXT_FILE_TEXT_BEGIN);
    auto position = std::min(position_bin, position_txt);

    if (position > source.size())
      break;

    // add first
    separated_list.push_back(source.substr(0, position));

    // find close char ')'
    int close_position = -1;
    for (int i = position; i < source.size(); i++) {
      if (source[i] == ')') {
        close_position = i + 1;
        break;
      }
    }

    if (close_position == -1)
      throw std::runtime_error("token error");

    // add file str
    separated_list.push_back(
        source.substr(position, close_position - position));

    // assign remaining
    source = source.substr(close_position, source.size() - close_position);
  }

  // add remaining
  separated_list.push_back(source);

  return std::move(separated_list);
}

Var Command::get_Var_from_str(const std::string str_) {
  if (str_.size() == 0)
    throw std::runtime_error("str is null for turning into VAR ");

  auto split_list = split_file_strings(str_);

  // We are iterating list to replace file contents
  for (int i = 0; i < split_list.size(); i++) {
    auto str = split_list[i];
    if (str.rfind(BINARY_FILE_TEXT_BEGIN, 0) == 0) {
      // Extracting path
      str.erase(0, sizeof(BINARY_FILE_TEXT_BEGIN) - 1);
      str.pop_back();
      auto file_path = str;

      // BIN READ
      std::ifstream f(file_path, std::ios::binary);
      if (!f.is_open()) {
        throw std::runtime_error("file does not exists");
      }
      // copies all data into buffer
      std::vector<char> buffer((std::istreambuf_iterator<char>(f)),
                               (std::istreambuf_iterator<char>()));
      auto v = Var((uint8_t *)buffer.data(), buffer.size());
      auto str_from_file = v.to_json(0);

      // replace str
      split_list[i] = str_from_file;

    } else if (str.rfind(TEXT_FILE_TEXT_BEGIN, 0) == 0) {
      // Extracting path
      str.erase(0, sizeof(TEXT_FILE_TEXT_BEGIN) - 1);
      str.pop_back();

      // replace str
      auto file_path = str;
      // TEXT READ
      std::ifstream f(file_path);
      if (!f.is_open()) {
        throw std::runtime_error("file does not exists");
      }
      std::string str_from_file((std::istreambuf_iterator<char>(f)),
                                std::istreambuf_iterator<char>());

      // replace str
      split_list[i] = str_from_file;
    }
  }

  // Merge them
  string_ str;
  for (auto sp : split_list) {
    str = str + sp;
  }

  // It is a hack for reading value in json
  try {
    Var v = Var::from_json("{\"var\":" + str + "}");
    return v["var"];
  } catch (std::exception e) {
    throw std::runtime_error(
        "error in json turning your value into dsa::Var : ");
  }
}
void Command::print_usage_static(std::string usage_str) {
  typedef std::vector<std::string> split_vector_type;

  split_vector_type SplitVec;
  boost::split(SplitVec, usage_str, boost::is_any_of("```"),
               boost::token_compress_on);

  int i = 0;
  for (auto s : SplitVec) {
    if (i % 2 == 1) {
      std::cout << cmdlog::stream;
    } else {
      std::cout << cmdlog::reset;
    }
    std::cout << s;
    i++;
  }
  std::cout << cmdlog::reset;
}