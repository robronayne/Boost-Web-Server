#ifndef STATIC_HANDLER_H
#define STATIC_HANDLER_H

#include <string>
#include <iostream>
#include <filesystem>

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
    static_handler(std::string location, std::string root_file_path, std::string request_url);
    http::status serve(const http::request<http::dynamic_body> req, http::response<http::dynamic_body>& res);

  private:
    void log_message_info(std::string res_code);

    std::string location_;
    std::string request_url_;
    std::string method;
    std::string extension;
    std::string root_;
    util utility;
};

#endif