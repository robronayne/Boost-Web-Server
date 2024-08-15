#include <cstddef>
#include <string>
#include <vector>

#include "request_handler/echo_handler.h"

/**
 * Create generic echo handler for an unspecified request.
 */
echo_handler::echo_handler()
{
  request_body = "";
  size = 0;
}

/**
 * Generate an echo handler with specified message from a request.
 */
echo_handler::echo_handler(request request_, size_t byte_transferred) 
  : request_body(request_.original_req), size(byte_transferred) {}

/**
 * For the current handler, set the request body to the original request.
 */
void echo_handler::set_request(request request_, size_t byte_transferred)
{
  request_body = request_.original_req;
  size = byte_transferred;
}

/**
 * Construct an HTTP structured reply for the received echo request.
 */
reply echo_handler::get_reply()
{
  reply_.status = reply::ok;
  reply_.content = request_body;
  reply_.headers.resize(2);
  reply_.headers[0].name = "Content-Length";
  reply_.headers[0].value = std::to_string(reply_.content.size());
  reply_.headers[1].name = "Content-Type";
  reply_.headers[1].value = "text/plain";
  return reply_;
}

