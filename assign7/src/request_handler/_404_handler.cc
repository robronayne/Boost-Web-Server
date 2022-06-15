#include "request_handler/_404_handler.h"

/**
 * Generate an 404 handler with specified message for specific request.
 */
_404_handler::_404_handler(std::string location, std::string request_url) : 
    location_(location), request_url_(request_url) {}

/**
 * Construct an HTTP structured reply for the not found request.
 */
http::status _404_handler::serve(const http::request<http::dynamic_body> req, http::response<http::dynamic_body>& res)
{
  res.result(http::status::not_found);
  beast::ostream(res.body()) << utility.get_stock_reply(res.result_int());
  res.content_length((res.body().size()));
  res.set(http::field::content_type, "text/html");
  return http::status::not_found;
}