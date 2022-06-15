#include "request_handler_factory/authentication_handler_factory.h"
#include "request_handler/authentication_handler.h"

authentication_handler_factory::authentication_handler_factory(std::string location, path root_file_path) :
      location_(location), root_file_path_(root_file_path)
{

}

request_handler_interface* authentication_handler_factory::create(std::string location, std::string request_url, user_profile profile)
{
  return new authentication_handler(location, request_url, root_file_path_.info_map["data_path"], root_file_path_.info_map["root"],
                                    root_file_path_.info_map["login"], root_file_path_.info_map["logout"], root_file_path_.info_map["signup"], profile);
}