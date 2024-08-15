#include <string>
#include <cstring>
#include <iostream>

#include "http/reply.h"

/**
 * Define status enum to error code string function.
 */
namespace status_strings 
{
  boost::asio::const_buffer to_buffer(reply::status_type status)
  {
    switch (status)
    {
      case reply::ok:           return boost::asio::buffer(ok);
      case reply::not_found:    return boost::asio::buffer(not_found);
      case reply::bad_request:  return boost::asio::buffer(bad_request);
      default:                  return boost::asio::buffer(bad_request);
    }
  }
}

/**
 * Declare important symbols in HTTP responses.
 */
namespace misc_strings
{
	const std::string name_value_separator = ": ";
	const std::string crlf = "\r\n";
}

/**
 * Define enum to standard reply messages.
 */
namespace stock_replies {
  std::string to_string(reply::status_type status)
  {
    switch (status)
    {
      case reply::ok:             return ok;
      case reply::bad_request:    return bad_request;
      case reply::not_found:      return not_found;
      default:                    return bad_request;
    }
  }
}

/** 
 * Set up the respond buffer and put it into a vector.
 * Returns a vector of boost buffers.
 */
std::vector<boost::asio::const_buffer> reply::to_buffers()
{
  std::vector<boost::asio::const_buffer> buffers;
  buffers.push_back(status_strings::to_buffer(status));
  
  for (std::size_t i = 0; i < headers.size(); ++i)
  {
    header& head = headers[i];
    buffers.push_back(boost::asio::buffer(head.name));
    buffers.push_back(boost::asio::buffer(misc_strings::name_value_separator));
    buffers.push_back(boost::asio::buffer(head.value));
    buffers.push_back(boost::asio::buffer(misc_strings::crlf));
  }

  buffers.push_back(boost::asio::buffer(misc_strings::crlf));
  buffers.push_back(boost::asio::buffer(content));
  return buffers;
}

/**
 * Define standard status coded response message constructor function.
 */
reply reply::stock_reply(reply::status_type status)
{
  reply reply_;
  reply_.status = status;
  reply_.content = stock_replies::to_string(status);
  reply_.headers.resize(2);
  reply_.headers[0].name = "Content-Length";
  reply_.headers[0].value = std::to_string(reply_.content.size());
  reply_.headers[1].name = "Content-Type";
  reply_.headers[1].value = "text/html";
  return reply_;
}

