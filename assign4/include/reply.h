/**
  Inspiration from https://www.boost.org/doc/libs/1_71_0/doc/html/boost_asio/example/cpp11/http/server/reply.hpp
**/
#ifndef REPLY_H
#define REPLY_H


#include <string>
#include <vector>
#include <boost/asio.hpp>

#include "header.h"

struct reply
{
  enum status_type
  {
    ok = 200,
    bad_request = 400,
    not_found = 404,
    unknown = 999
  } status;

  /// The headers to be included in the reply.
  std::vector<header> headers;

  /// The content to be sent in the reply.
  std::string content;

  std::vector<boost::asio::const_buffer> to_buffers();

  /// Get a stock reply.
  static reply stock_reply(status_type status);
};

namespace stock_replies {
  // Declare standard reply messages
  const char ok[] = "";
  const char bad_request[] =
    "<html>"
    "<head><title>Bad Request</title></head>"
    "<body><h1>400 Bad Request</h1></body>"
    "</html>";
  const char not_found[] =
    "<html>"
    "<head><title>Not Found</title></head>"
    "<body><h1>404 Not Found</h1></body>"
    "</html>";
}

namespace status_strings 
{
  // Declare status strings for error codes
  const std::string ok =
    "HTTP/1.1 200 OK\r\n";
  const std::string bad_request =
    "HTTP/1.1 400 Bad Request\r\n";
  const std::string not_found =
    "HTTP/1.1 404 Not Found\r\n";
}

#endif