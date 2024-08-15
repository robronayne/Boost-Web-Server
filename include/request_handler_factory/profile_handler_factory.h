#ifndef PROFILE_HANDLER_FACTORY_H
#define PROFILE_HANDLER_FACTORY_H

#include "request_handler_factory.h"
#include "http/path.h"

/**
 *  Profile handler factory class.
 *  
 *  Inherits from request_handler_factory.
 *  
 *  The profile handler factory constructs a profile handler factory
 *  by giving the location and path.
 *
 *  The function create will return a request_handler_interface point.
 *  In this class, the return point points to a profile handler.
 *
 */
class profile_handler_factory : public request_handler_factory
{
  public:
    profile_handler_factory(std::string location, path root_file_path);

    request_handler_interface* create(std::string location, std::string request_url, user_profile profile);

  private:
    std::string location_;
    path root_file_path_;
};

#endif