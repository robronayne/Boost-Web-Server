#include "request_handler/error_handler.h"

/**
 * Generate an error handler with specified message for specific request.
 */
error_handler::error_handler(http::status ec, std::string request_url) : err_code(ec), request_url_(request_url) {}

/**
 * Construct an HTTP structured reply for the erroneous request.
 */
http::status error_handler::serve(const http::request<http::dynamic_body> req, http::response<http::dynamic_body>& res)
{
  res.result(err_code);
  beast::ostream(res.body()) << utility.get_stock_reply(res.result_int());
  res.content_length((res.body().size()));
  res.set(http::field::content_type, "text/html");

  log_message_info(err_code);

  return err_code;
}

void error_handler::log_message_info(http::status code)
{
  int res_code;
  switch(code)
  {
    case http::status::not_found :
      res_code = 404;
      break;
    case http::status::bad_request :
      res_code = 400;
      break;
  }
  BOOST_LOG_TRIVIAL(info) << "[ResponseMetrics] "
                        << "response_code: "
                        << res_code
                        << " "
                        << "request_path: "
                        << request_url_
                        << " "
                        << "matched_handler: error handler";
}