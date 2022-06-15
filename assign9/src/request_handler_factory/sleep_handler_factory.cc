#include "request_handler_factory/sleep_handler_factory.h"
#include "request_handler/sleep_handler.h"

sleep_handler_factory::sleep_handler_factory(std::string location, path root_file_path) :
      location_(location), root_file_path_(root_file_path)
{

}

request_handler_interface* sleep_handler_factory::create(std::string location, std::string request_url, user_profile profile)
{
  return new sleep_handler(location, request_url);
}