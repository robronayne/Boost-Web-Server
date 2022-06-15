#ifndef HEALTH_HANDLER_H
#define HEALTH_HANDLER_H

#include <string>
#include <boost/log/trivial.hpp>
#include "request_handler_interface.h"

/**
 * Health handler class
 *
 * Inherits from request_handler_interface
 *
 * The health handler always constructs the 
 * appropriate 200 OK response.
 *
 * The function serve constructs the health response
 * with the appropriate headers.
 */
class health_handler : public request_handler_interface
{
  public:
    health_handler(std::string location, std::string request_url);
    http::status serve(const http::request<http::dynamic_body> req, http::response<http::dynamic_body>& res);

  private:
    void log_message_info(std::string res_code);

    std::string location_;
    std::string request_url_;
    util utility;
};

#endif