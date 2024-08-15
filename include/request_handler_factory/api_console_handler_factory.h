#ifndef API_CONSOLE_HANDLER_FACTORY_H
#define API_CONSOLE_HANDLER_FACTORY_H

#include "request_handler_factory.h"
#include "http/path.h"

/**
 *  API Console handler factory class
 *  
 *  Inherits from request_handler_factory
 *  
 *  The API console handler factory constructs an API console handler factory
 *  by giving the location and path
 *
 *  The function create will return a request_handler_interface point.
 *  In this class, the return pointer points to an API console handler.
 *  
 */
 
class api_console_handler_factory : public request_handler_factory
{
  public:
    api_console_handler_factory(std::string location, path root_file_path);
    request_handler_interface* create(std::string location, std::string request_url, user_profile profile);

  private:
    std::string location_;
    path root_file_path_;
};



#endif