#include "cmdline.h"

#include <boost/algorithm/string.hpp>
#include "utils/cmdlog.h"
#include "utils/kbhit.h"

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


bool CmdLine::get_input() {
  is_waiting_user_input = true;

  char line[CMD_MAX_LENGTH];

  // Print current path and wait
  print_wanting_user_input();

  // 1. Input taking
  if (!fgets(line, CMD_MAX_LENGTH, stdin)) return false;
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

  // We don't require to do something
  if( value == is_connected) return;

  is_connected = value;

  if(is_waiting_user_input){
    print_wanting_user_input();
    return;
  }

  // It can be streaming...
  // maybe do something here
}

void CmdLine::print_wanting_user_input(){
  std::cout<<std::endl;
  if(is_connected){
    std::cout<<termcolor::green;
  }
  else{
    std::cout<<termcolor::red;
  }
  std::cout << CMDLINE_CIRCLE << " ";

  std::cout<<cmdlog::path<<"> "<<Command::current_path<<cmdlog::reset<<" ";
  std::cout<<std::flush;
}


