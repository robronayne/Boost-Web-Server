#ifndef AUTHENTICATION_HANDLER_H
#define AUTHENTICATION_HANDLER_H

#include <string>
#include <boost/log/trivial.hpp>
#include "request_handler_interface.h"

/**
 * Authentication handler class.
 *
 * Inherits from request_handler_interface.
 *
 * The authentication handler directs workflow for login and
 * logout depending upon the endpoint provided. 
 *
 * The function serve constructs the authentication response
 * with the appropriate headers, determined by the success or
 * failure of the selected action.
 *
 */
class authentication_handler : public request_handler_interface
{
  public:
    authentication_handler(std::string location, std::string request_url, std::string data_path, std::string root,
                           std::string login, std::string logout, std::string signup, user_profile profile);
    http::status serve(const http::request<http::dynamic_body> req, http::response<http::dynamic_body>& res);
    user_profile get_profile();

  private:
    void log_message_info(std::string res_code);

    std::string location_;
    std::string request_url_;
    std::string data_path_;
    std::string root_;
    std::string login_;
    std::string logout_;
    std::string signup_;
    util utility;
    user_profile profile_;
};

#endif