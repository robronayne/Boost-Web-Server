/**
  Inspiration taken from https://www.boost.org/doc/libs/1_71_0/doc/html/boost_asio/example/cpp11/http/server/reply.hpp
**/
#include "reply.h"
#include <string>
#include <cstring>
#include <iostream>

namespace status_strings 
{
  // Define enum to error code string function
  boost::asio::const_buffer to_buffer(reply::status_type status)
  {
    switch (status)
    {
    case reply::ok:
      return boost::asio::buffer(ok);
    case reply::not_found:
      return boost::asio::buffer(not_found);
    case reply::bad_request:
      return boost::asio::buffer(bad_request);
    default:
      return boost::asio::buffer(bad_request);
    }
  }
} // namespace status_strings

namespace misc_strings
{
	// Declare important symbols in HTTP responses
	const std::string name_value_separator = ": ";
	const std::string crlf = "\r\n";
}	// namespace misc_strings

namespace stock_replies {
  // Define enum to standard reply messages 
  std::string to_string(reply::status_type status)
  {
    switch (status)
    {
      case reply::ok:
        return ok;
      case reply::bad_request:
        return bad_request;
      case reply::not_found:
        return not_found;
      default:
        return bad_request;
    }
  }
} // namespace stock_reples

/** 
	set up the respond buffer and put it into a vector.
	return type: vector<boost::asio::const_buffer>
*/
std::vector<boost::asio::const_buffer> reply::to_buffers()
{
  std::vector<boost::asio::const_buffer> buffers;
  buffers.push_back(status_strings::to_buffer(status));
  for (std::size_t i = 0; i < headers.size(); ++i)
  {
    header& h = headers[i];
    buffers.push_back(boost::asio::buffer(h.name));
    buffers.push_back(boost::asio::buffer(misc_strings::name_value_separator));
    buffers.push_back(boost::asio::buffer(h.value));
    buffers.push_back(boost::asio::buffer(misc_strings::crlf));
  }
  buffers.push_back(boost::asio::buffer(misc_strings::crlf));
  buffers.push_back(boost::asio::buffer(content));
  return buffers;
}

// Define standard status coded response message constructor function
reply reply::stock_reply(reply::status_type status)
{
  reply rep;
  rep.status = status;
  rep.content = stock_replies::to_string(status);
  rep.headers.resize(2);
  rep.headers[0].name = "Content-Length";
  rep.headers[0].value = std::to_string(rep.content.size());
  rep.headers[1].name = "Content-Type";
  rep.headers[1].value = "text/html";
  return rep;
}

