#include "request_handler/health_handler.h"

/**
 * Generate an health handler with location.
 */
health_handler::health_handler(std::string location, std::string request_url) 
  : location_(location), request_url_(request_url) {

}

/**
 * Construct an HTTP 200 OK reply for the received health request.
 */
http::status health_handler::serve(const http::request<http::dynamic_body> req, http::response<http::dynamic_body>& res)
{
  std::string input = req.target().to_string();
  if (location_ != input)
  {
    res.result(http::status::not_found);
    beast::ostream(res.body()) << utility.get_stock_reply(res.result_int());
    res.content_length((res.body().size()));
    res.set(http::field::content_type, "text/html");

    log_message_info("404");

    return res.result();
  }
  res.result(http::status::ok);
  beast::ostream(res.body()) << "OK";
  res.content_length((res.body().size()));
  res.set(http::field::content_type, "text/plain");

  log_message_info("200");

  return res.result();
}

void health_handler::log_message_info(std::string res_code)
{
  BOOST_LOG_TRIVIAL(info) << "[ResponseMetrics] "
                          << "response_code: "
                          << res_code
                          << " "
                          << "request_path: "
                          << request_url_
                          << " "
                          << "matched_handler: health handler";
}