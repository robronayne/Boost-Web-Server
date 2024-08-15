#ifndef API_CONSOLE_HANDLER_H
#define API_CONSOLE_HANDLER_H

#include <string>

#include "request_handler_interface.h"

/**
 * API console handler class
 *
 * Inherits from request_handler_interface
 *
 * The api console handler allows logged in users to access the API handler.
 *
 * The function serve constructs the API console response
 * with the appropriate headers.
 */

class api_console_handler : public request_handler_interface
{
  public:
    api_console_handler(std::string location, std::string request_url, std::string root, user_profile profile);
    http::status serve(const http::request<http::dynamic_body> req, http::response<http::dynamic_body>& res);

  private:
    std::string location_;
    std::string request_url_;
    std::string root_;
    user_profile profile_;
    util utility;
};

#endif