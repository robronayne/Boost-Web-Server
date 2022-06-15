#include <cstddef>
#include <string>
#include <vector>

#include "request_handler/echo_handler.h"

/**
 * Generate an echo handler with location.
 */
echo_handler::echo_handler(std::string location, std::string request_url) 
  : location_(location), request_url_(request_url) {

}

/**
 * Construct an HTTP structured reply for the received echo request.
 */
http::status echo_handler::serve(const http::request<http::dynamic_body> req, http::response<http::dynamic_body>& res)
{
  std::string input = req.target().to_string();
  if (location_ != input)
  {
    res.result(http::status::not_found);
    beast::ostream(res.body()) << utility.get_stock_reply(res.result_int());
    res.content_length((res.body().size()));
    res.set(http::field::content_type, "text/html");
    return res.result();
  }
  res.result(http::status::ok);
  beast::ostream(res.body()) << req;
  res.content_length((res.body().size()));
  res.set(http::field::content_type, "text/plain");
  return res.result();
}

