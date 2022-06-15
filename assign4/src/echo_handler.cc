#include "echo_handler.h"
#include <cstddef>
#include <string>
#include <vector>

// Create basic echo handler
echo_handler::echo_handler()
{
  request_body = "";
  size = 0;
}

// Create echo handler with specified message
echo_handler::echo_handler(request request_, size_t byte_transferred) 
  : request_body(request_.original_req), size(byte_transferred)
{

}

// Set specific message
void echo_handler::set_request(request request_, size_t byte_transferred)
{
  request_body = request_.original_req;
  size = byte_transferred;
}

// Construct echo reply
reply echo_handler::get_reply()
{
  rep.status = reply::ok;
  rep.content = request_body;
  rep.headers.resize(2);
  rep.headers[0].name = "Content-Length";
  rep.headers[0].value = std::to_string(rep.content.size());
  rep.headers[1].name = "Content-Type";
  rep.headers[1].value = "text/plain";
  return rep;
}

