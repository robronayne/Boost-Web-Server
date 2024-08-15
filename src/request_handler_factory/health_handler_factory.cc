#include "request_handler_factory/health_handler_factory.h"
#include "request_handler/health_handler.h"

health_handler_factory::health_handler_factory(std::string location, path root_file_path) :
      location_(location), root_file_path_(root_file_path)
{

}

request_handler_interface* health_handler_factory::create(std::string location, std::string request_url, user_profile profile)
{
  return new health_handler(location, request_url);
}