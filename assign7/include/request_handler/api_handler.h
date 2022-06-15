#ifndef API_HANDLER_H
#define API_HANDLER_H

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <unordered_map>
#include "boost/filesystem.hpp"
#include "boost/beast/core/buffers_to_string.hpp"
#include "request_handler_interface.h"
#include "entity_manager.h"

class api_handler : public request_handler_interface {

  public:
    api_handler(std::string location, std::string root_file_path, std::string request_url, entity_manager* entity_manager);
    http::status serve(const http::request<http::dynamic_body> req, http::response<http::dynamic_body>& res);
    void fail(http::response<http::dynamic_body>& res, http::status fail_type);

  private:
    std::string location_;
    std::string request_url_;
    std::string method;
    std::string root_;
    util utility;
    entity_manager* e_manager;
};

#endif