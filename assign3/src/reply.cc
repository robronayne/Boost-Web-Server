#include "reply.h"
#include <string>
#include <cstring>
#include <iostream>

namespace status_strings 
{
//
const std::string ok =
  "HTTP/1.1 200 OK\r\n";
const std::string not_found =
  "HTTP/1.1 404 Not Found\r\n";

boost::asio::const_buffer to_buffer(reply::status_type status)
{
  switch (status)
  {
  case reply::ok:
    return boost::asio::buffer(ok);
  case reply::not_found:
    return boost::asio::buffer(not_found);
  default:
    return boost::asio::buffer(ok);
  }
}
}     //   namespace status_strings


namespace misc_strings
{
	//temporary set to text/plain for this assignment
	const std::string temp_type = "Content-Type: text/plain\r\n";
	const char crlf[] = {'\r', '\n'};
}		//	namespace misc_strings

/** 
	set up the respond buffer and put it into a vector.
	return type: vector<boost::asio::const_buffer>
*/
std::vector<boost::asio::const_buffer> reply::to_buffers(const std::string& request, size_t bytes_transferred)
{
  std::vector<boost::asio::const_buffer> buffers;
  
  buffers.push_back(status_strings::to_buffer(status));               //status line
  buffers.push_back(boost::asio::buffer(misc_strings::temp_type));    //content type
  buffers.push_back(boost::asio::buffer(misc_strings::crlf));         //CRLF
  buffers.push_back(boost::asio::buffer(request, bytes_transferred)); //original request

  return buffers;
}

