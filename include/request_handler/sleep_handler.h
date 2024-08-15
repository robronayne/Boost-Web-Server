#ifndef SLEEP_HANDLER_H
#define SLEEP_HANDLER_H

#include <string>
#include <boost/log/trivial.hpp>
#include "request_handler_interface.h"

/**
 * Sleep handler class
 *
 * Inherits from request_handler_interface
 *
 * The sleep handler delays the response before returning a generic response.
 *
 * The function serve constructs the sleep response
 * with the appropriate headers.
 */

class sleep_handler : public request_handler_interface
{
  public:
    sleep_handler(std::string location, std::string request_url);
    http::status serve(const http::request<http::dynamic_body> req, http::response<http::dynamic_body>& res);

  private:
    void log_message_info(std::string res_code);

    std::string location_;
    std::string request_url_;
};

#endif