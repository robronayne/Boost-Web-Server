#include "request_handler/error_handler.h"

/**
 * Generate an error handler with specified message for specific request.
 */
error_handler::error_handler(http::status ec) : err_code(ec) {}

/**
 * Construct an HTTP structured reply for the erroneous request.
 */
http::status error_handler::serve(const http::request<http::dynamic_body> req, http::response<http::dynamic_body>& res)
{
  res.result(err_code);
  beast::ostream(res.body()) << utility.get_stock_reply(res.result_int());
  res.content_length((res.body().size()));
  res.set(http::field::content_type, "text/html");
  return err_code;
}