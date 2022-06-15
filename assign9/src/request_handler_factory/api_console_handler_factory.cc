#include "request_handler_factory/api_console_handler_factory.h"
#include "request_handler/api_console_handler.h"

api_console_handler_factory::api_console_handler_factory(std::string location, path root_file_path) :
      location_(location), root_file_path_(root_file_path)
{

}

request_handler_interface* api_console_handler_factory::create(std::string location, std::string request_url, 
                                                               user_profile profile)
{
  return new api_console_handler(location, request_url, root_file_path_.info_map["root"], profile);
}