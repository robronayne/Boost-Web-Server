#ifndef REQUEST_HANDLER_INTERFACE_H
#define REQUEST_HANDLER_INTERFACE_H

#include <vector>

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include "util.h"

namespace beast = boost::beast;
namespace http = beast::http;

/** 
 * Parent class for request interface objects. Request handlers 
 * are designed to return a HTTP reply given different requests 
 * that are received.
 */
class request_handler_interface
{
  public:
    virtual http::status serve(const http::request<http::dynamic_body> req, http::response<http::dynamic_body>& res) = 0;
    virtual ~request_handler_interface() {}
};

#endif