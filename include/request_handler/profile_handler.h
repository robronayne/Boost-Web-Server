#ifndef PROFILE_HANDLER_H
#define PROFILE_HANDLER_H

#include <string>
#include <boost/log/trivial.hpp>
#include "request_handler_interface.h"
#include "user_profile.h"

/**
 * Profile handler class.
 *
 * Inherits from request_handler_interface.
 *
 * The profile handler constructs the appropriate profile response
 * based on the user's status of authorized or not authorized.
 *
 * The function serve constructs the profile response
 * with the appropriate headers, including user information.
 *
 */
class profile_handler : public request_handler_interface
{
  public:
    profile_handler(std::string location, std::string request_url, user_profile profile);
    http::status serve(const http::request<http::dynamic_body> req, http::response<http::dynamic_body>& res);

  private:
    void log_message_info(std::string res_code);

    std::string location_;
    std::string request_url_;
    user_profile user;
    util utility;
};

#endif