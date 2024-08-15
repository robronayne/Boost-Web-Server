#ifndef REQUEST_HANDLER_FACTORY_H
#define REQUEST_HANDLER_FACTORY_H

#include <string>
#include "request_handler/request_handler_interface.h"
#include "user_profile.h"

/*
 *  Parent class for request handler factory object. Request handler factories
 *  are designed to create a request handler by giving the location and 
 *  request_url.
 */
class request_handler_factory
{
  public:
    virtual request_handler_interface* create(std::string location, std::string request_url, user_profile profile) = 0;
    virtual ~request_handler_factory() {}
};


#endif