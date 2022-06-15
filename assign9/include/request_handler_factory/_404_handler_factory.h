#ifndef _404_HANDLER_FACTORY_H
#define _404_HANDLER_FACTORY_H

#include "request_handler_factory.h"
#include "http/path.h"

/**
 *  404 handler factory class
 *  
 *  Inherits from request_handler_factory
 *  
 *  The 404 handler factory constructs a 404 handler factory
 *  by giving the location and path object
 *
 *  The function create will return a request_handler_interface point.
 *  In this class, the return point point to a 404 handler.
 *  
 */
class _404_handler_factory : public request_handler_factory
{
  public:
    _404_handler_factory(std::string location, path root_file_path);

    request_handler_interface* create(std::string location, std::string request_url, user_profile profile);

  private:
    std::string location_;
    path root_file_path_;
};








#endif