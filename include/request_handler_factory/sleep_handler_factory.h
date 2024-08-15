#ifndef SLEEP_HANDLER_FACTORY_H
#define SLEEP_HANDLER_FACTORY_H

#include "request_handler_factory.h"
#include "http/path.h"

/**
 *  Sleep handler factory class
 *  
 *  Inherits from request_handler_factory
 *  
 *  The sleep handler factory constructs a sleep handler factory
 *  by giving the location and path
 *
 *  The function create will return a request_handler_interface point.
 *  In this class, the return pointer points to a sleep handler.
 *  
 */
 
class sleep_handler_factory : public request_handler_factory
{
  public:
    sleep_handler_factory(std::string location, path root_file_path);

    request_handler_interface* create(std::string location, std::string request_url);

  private:
    std::string location_;
    path root_file_path_;
};



#endif