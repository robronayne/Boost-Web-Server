#ifndef ECHO_HANDLER_H
#define ECHO_HANDLER_H

#include <string>
#include <vector>

#include "request_handler_interface.h"

/**
 * Echo handler class
 *
 * Inherits from request_handler_interface
 *
 * The echo handler constructs the appropriate echo
 * response given a location and request_url.
 *
 * The function serve constructs the echo response
 * with the appropriate headers.
 */
class echo_handler : public request_handler_interface
{
  public:
    echo_handler(std::string location, std::string request_url);
    http::status serve(const http::request<http::dynamic_body> req, http::response<http::dynamic_body>& res);

  private:
    std::string location_;
    std::string request_url_;
    util utility;
};

#endif