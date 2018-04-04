#include "dsa_common.h"
#include "cmdline.h"

#include <boost/algorithm/string.hpp>
#include "utils/cmdlog.h"
#include "utils/kbhit.h"
#include "utils/linenoise.h"
#include "utils/ConvertUTF.h"

#include <wctype.h>

CmdLine::CmdLine(std::shared_ptr<App> app, ref_<DsLink> dslink) {
  this->app = app;
  this->strand.reset(app->new_strand());
  this->link = dslink;

  is_connected = false;
  is_waiting_user_input = false;

  Command::current_path = "";
  Command::link = dslink;

  std::cout << cmdlog::info << "CMDLINE-DSLINK" << cmdlog::endl;
  std::cout << cmdlog::info << "If you are new user type 'help' to see available commands" << cmdlog::endl;

  dslink->connect(
      [&](const shared_ptr_<Connection> connection, ref_<DsLinkRequester> link_req) {
        if(!connection){
          set_connected(false);
          return;
        }

        auto session = connection->session();
        if(!session) {
          set_connected(false);
          return;
        }

        set_connected(session->is_connected());
      },
      Client::FIRST_CONNECTION | Client::BROKER_INFO_CHANGE | Client::EVERY_CONNECTION | Client::DISCONNECTION);

  std::cout << "------------" << std::endl;
  std::cout << cmdlog::info << "We requested to connect." << cmdlog::endl;
  std::cout << cmdlog::info << "Connected : "<< termcolor::green<< CMDLINE_CIRCLE <<termcolor::reset
            << " | "<<"Disconnected : " <<termcolor::red<< CMDLINE_CIRCLE << termcolor::reset << cmdlog::endl;
  std::cout << cmdlog::info << "If it is not connected, check broker address that you gave." << cmdlog::endl;

  // Just wait little bit for connect
  for (int i = 0; i < 20; i++) {
    boost::this_thread::sleep(boost::posix_time::milliseconds(25));
    if (is_connected) {
      break;
    }
  }
}


void CmdLine::run() {
  strand->post([&](){
    while(this->get_input()){}
    if(link != nullptr) link->strand->post([this](){link->destroy();});
  });

  app->close();
  app->wait();
}
const int SLEEP_INTERVAL = 10;
int wait_for_bool(int wait_time, const std::function<bool()>& callback) {
  int waited = 0;
  while (waited < wait_time) {
    if (callback()) {
      return waited;
    }
    boost::this_thread::sleep(
        boost::posix_time::milliseconds(SLEEP_INTERVAL));
    waited += SLEEP_INTERVAL;
  }
  return -1;
}


void completion(const char *buf, linenoiseCompletions *lc) {
  std::string buf_str(buf);
  std::string path_to_list;
  std::string filter;
  path_to_list = Command::current_path;

  std::size_t found_slash = buf_str.rfind("/");
  if (found_slash != std::string::npos) {
    if(!path_to_list.empty())
      path_to_list += "/";
    path_to_list += buf_str.substr(0, found_slash);
    filter = buf_str.substr(found_slash+1);
  } else {
    filter = buf_str;
  }

  VarMap list_map;
  bool listed = false;
  Command::link->strand->post([&, path_to_list]() {

    auto incoming_list_cache =
        dynamic_cast<DsLinkRequester *>(Command::link.get())
            ->list(path_to_list, [&](IncomingListCache &cache,
                                            const std::vector<string_> &str) {
              VarMap map = cache.get_map();
              list_map.insert(map.begin(), map.end());
              listed = true;
              //close cache
              cache.close();
            });
  });
  int list_done = wait_for_bool(500,[&]() -> bool { return listed; });
  if(list_done != -1) {
    if(Command::current_path.size() != 0)
      path_to_list = path_to_list.erase(0, Command::current_path.size()+1);
    else
      path_to_list = path_to_list.erase(0, Command::current_path.size());
    for (auto &list_item : list_map) {
      std::size_t found = list_item.first.find(filter);
      if (found != std::string::npos && found == 0) {
        if(!path_to_list.empty())
          linenoiseAddCompletion(lc, (path_to_list + "/" + list_item.first).c_str());
        else
          linenoiseAddCompletion(lc, list_item.first.c_str());
      }
    }
  }

}

bool CmdLine::get_input() {
  is_waiting_user_input = true;

  char *line_ret;

  char line[CMD_MAX_LENGTH];

  // Print current path and wait
  print_wanting_user_input();

  // 1. Input taking
  //if (!fgets(line, CMD_MAX_LENGTH, stdin)) return false;

  linenoiseSetCompletionCallback(completion);

  /* Load history from file. The history file is just a plain text file
   * where entries are separated by newlines. */
  linenoiseHistoryLoad("history.txt"); /* Load the history at startup */

  line_ret = linenoise(buffer.str().c_str());

  buffer.clear();
  buffer.str(std::string());

  if (line_ret == NULL)
    exit(0);

  linenoiseHistoryAdd(line_ret);
  linenoiseHistorySave("history.txt");
  strcpy(line, line_ret);
  free(line_ret);

  is_waiting_user_input = false;

  std::string line_str(line);

  // If user press enter without anything
  if(line_str[0] == '\n') return true;

  // 2. Get Command
  auto cmd = command_factory.get_command(line_str);

  // 3. Execute Command
  cmd->execute();

  auto return_ = cmd->get_return_type();

  // 4. Command says we need wait he was executing
  if(return_ == COMMAND_RETURN_TERMINATE){
    return false;
  }
  else if(return_ == COMMAND_RETURN_WAIT) {
    try{
      while (std::cin.get() != '\n') {}
    }
    catch(boost::thread_interrupted&){}
  }
  else{
    Command::wait_for_bool([&]()->bool{return cmd->is_invoked();});
    if(!cmd->is_invoked()){
      std::cout<<cmdlog::stream<<"It seems that server busy, not responding or message can have large data"<<cmdlog::endl;
      std::cout<<cmdlog::stream<<"You can wait for it or cancel by pressing enter."<<cmdlog::endl;
      fflush(stdout);
      while (!cmd->is_invoked() && !kbhit()) {
        boost::this_thread::sleep(boost::posix_time::milliseconds(10));
      }
      if(cmd->is_invoked()){
        std::cout<<cmdlog::stream<<"Oh! Server responded, sorry about that"<<cmdlog::endl;
        std::cout<<cmdlog::stream<<"If you are seeing this message to much.."<<cmdlog::endl;
        std::cout<<cmdlog::stream<<"...you can increase the timeout by typing 'timeout TimeInMilisec'"<<cmdlog::endl;
      }
      else{
        std::cout<<cmdlog::stream<<"Oh! You are impatient, OK!"<<cmdlog::endl;
      }
    }

  }

  // 5. Clear
  cmd->clear();

  return true;
}

void CmdLine::set_connected(bool value){

  std::string str;
  // We don't require to do something
  if( value == is_connected) return;

  is_connected = value;

  if(is_waiting_user_input){
    std::cout << std::endl;
    if(is_connected){
      std::cout << termcolor::green;
    }
    else{
      std::cout << termcolor::red;
    }
    std::cout << CMDLINE_CIRCLE << " " << termcolor::reset << "> " << Command::current_path << " ";
    std::cout<<std::flush;
    return;
  }



  // It can be streaming...
  // maybe do something here
}
std::ostringstream toString( std::ostream& str )
{
  std::ostringstream ss;
  ss << str.rdbuf();
  return ss;
}

void CmdLine::print_wanting_user_input(){
  //buffer<<std::endl;
  if(is_connected){
    //buffer << toString(termcolor::green(buffer)).str().c_str();
    buffer<<"\033[32m"; //green
  }
  else{
//    buffer<<termcolor::red;
    buffer<<"\033[31m"; //red
  }
  buffer << CMDLINE_CIRCLE << " ";

  buffer << "\033[00m";

  buffer<<cmdlog::path<<"> "<<Command::current_path<<cmdlog::reset<<" ";
  //std::cout<<std::flush;

}


