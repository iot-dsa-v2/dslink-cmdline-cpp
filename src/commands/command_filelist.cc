#include "dsa_common.h"
#include "command_filelist.h"


std::vector<int> CommandFileList::_available_args_num_options() {
  return {0};
}

const char* CommandFileList::_get_usage_info() {
  return
      "\tLists all defined placeholders with their filepaths.\n\n"
      "```> filelist``` \n "
      "\tlist all opened files\n\n";
}

COMMAND_RETURN_TYPE CommandFileList::_execute() {
  std::cout<<cmdlog::var<<"TXT files"<<cmdlog::endl;

  for(auto a:file_txts){
    std::cout<<cmdlog::var<<"\t>> "
             <<cmdlog::path<<a.first
             <<cmdlog::var<<" : "<<a.second<<cmdlog::endl;
  }

  std::cout<<cmdlog::var<<""<<cmdlog::endl;
  std::cout<<cmdlog::var<<"BIN files"<<cmdlog::endl;

  for(auto a:file_bins){
    std::cout<<cmdlog::var<<"\t>> "
             <<cmdlog::path<<a.first
             <<cmdlog::var<<" : "<<a.second<<cmdlog::endl;
  }

  return COMMAND_RETURN_CONTINUE;
}
void CommandFileList::_print() {

}



