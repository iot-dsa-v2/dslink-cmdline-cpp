
#ifndef CMDLINE_DSLINK_COMMAND_FACTORY_H
#define CMDLINE_DSLINK_COMMAND_FACTORY_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <map>

#include "command.h"

using namespace dsa;

 class CommandFactory {
   std::map<std::string, COMMAND_TYPE> command_str_map;

  public:
   CommandFactory();

   std::shared_ptr<Command> get_command(std::string _line);

  private:
   command_str tokenize(std::string _line);

 };

#endif //CMDLINE_DSLINK_COMMAND_FACTORY_H
