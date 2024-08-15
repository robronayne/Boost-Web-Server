#ifndef _404_HANDLER_H
#define _404_HANDLER_H

#include "request_handler_interface.h"
#include <cstring>

/**
 *  404 handler calss
 *
 *  Inherits from request_handler_interface
 *  
 *  The 404 handler constructs a 404 handler given
 *  a location and request url
 *
 *  The function serve constructs the 404 response with
 *  the appropriate headers.
 */
class _404_handler : public request_handler_interface
{
  public:
    _404_handler(std::string location, std::string request_url);
    http::status serve(const http::request<http::dynamic_body> req, http::response<http::dynamic_body>& res);

  private:
    std::string location_;
    std::string request_url_;
    http::status err_code;
    util utility;
};




#endif