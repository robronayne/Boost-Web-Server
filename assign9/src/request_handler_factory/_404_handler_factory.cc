#include "request_handler_factory/_404_handler_factory.h"
#include "request_handler/_404_handler.h"

_404_handler_factory::_404_handler_factory(std::string location, path root_file_path) :
      location_(location), root_file_path_(root_file_path) {}

request_handler_interface* _404_handler_factory::create(std::string location, std::string request_url, user_profile profile)
{
  return new _404_handler(location, request_url);
}