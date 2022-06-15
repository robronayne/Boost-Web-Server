#include "request_handler_factory/echo_handler_factory.h"
#include "request_handler/echo_handler.h"

echo_handler_factory::echo_handler_factory(std::string location, path root_file_path) :
      location_(location), root_file_path_(root_file_path)
{

}

request_handler_interface* echo_handler_factory::create(std::string location, std::string request_url)
{
  return new echo_handler(location, request_url);
}