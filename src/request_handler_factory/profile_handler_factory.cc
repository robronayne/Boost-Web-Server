#include "request_handler_factory/profile_handler_factory.h"
#include "request_handler/profile_handler.h"


profile_handler_factory::profile_handler_factory(std::string location, path root_file_path) :
      location_(location), root_file_path_(root_file_path)
{

}

request_handler_interface* profile_handler_factory::create(std::string location, std::string request_url, user_profile profile)
{
  return new profile_handler(location, request_url, profile);
}