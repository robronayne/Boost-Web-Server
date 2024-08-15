#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

#include <boost/log/trivial.hpp>
#include "request_handler_interface.h"

/**
 * Error handler class
 *
 * Inherits from request_handler_interface
 *
 * The error handler constructs the appropriate error
 * response given a status_type error code.
 *
 * The function set_error_code() is used to pass an 
 * error code and get_reply() constructs the standard 
 * error response with the appropriate headers depending
 * on the code.
 */
class error_handler : public request_handler_interface
{
  public:
    error_handler(http::status ec, std::string request_url);
    http::status serve(const http::request<http::dynamic_body> req, http::response<http::dynamic_body>& res);

  private:
    void log_message_info(http::status code);

    std::string request_url_;
    http::status err_code;
    util utility;
};

#endif