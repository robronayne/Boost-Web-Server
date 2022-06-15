#ifndef STATIC_HANDLER_H
#define STATIC_HANDLER_H

#include "request_handler_interface.h"
#include <iostream>
#include "boost/filesystem.hpp"
#include <filesystem>
#include "request.h"
#include <string>

class static_handler : public request_handler_interface
{
  public:
    static_handler();
    static_handler(request request_, std::string root_);

    void set_request(request request_, std::string root_);


    reply get_reply();

  private:
    reply::status_type ec;
    reply rep;
    std::string uri;
    std::string method;
    std::string extension;
    std::string root_;
    // std::filesystem::path path_;
};




#endif