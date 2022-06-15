#ifndef STATIC_HANDLER_H
#define STATIC_HANDLER_H

#include <string>
#include <iostream>
#include <filesystem>

#include "http/request.h"
#include "boost/filesystem.hpp"
#include "request_handler_interface.h"

/** 
 * Static handler class
 *
 * Inherits from request_handler_interface
 *
 * The static handler constructs the appropriate static
 * response given a request object and a root file path.
 *
 * The function set_request() is used to pass a request
 * object and root file path. The function get_reply() 
 * searches for the file (if it exists) and constructs 
 * the static response with the appropriate headers.
 */
class static_handler : public request_handler_interface
{
  public:
    static_handler();
    static_handler(request request_, std::string root_);

    void set_request(request request_, std::string root_);
    reply get_reply();

  private:
    reply::status_type ec;
    reply reply_;
    std::string uri;
    std::string method;
    std::string extension;
    std::string root_;
};

#endif