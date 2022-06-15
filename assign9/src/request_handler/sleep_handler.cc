#include "request_handler/sleep_handler.h"

/**
 * Generate a sleep handler with location.
 */
sleep_handler::sleep_handler(std::string location, std::string request_url) 
  : location_(location), request_url_(request_url) {

}

/**
 * Puts a request to sleep before returning a response
 */
http::status sleep_handler::serve(const http::request<http::dynamic_body> req, http::response<http::dynamic_body>& res)
{
  usleep(SLEEPY_TIME);
  std::string input = req.target().to_string();
  res.result(http::status::ok);
  std::string body = "This request slept for " + std::to_string(SLEEPY_TIME) + " microseconds";
  beast::ostream(res.body()) << body;
  res.content_length((res.body().size()));
  res.set(http::field::content_type, "text/plain");

  log_message_info("200");

  return res.result();
}

void sleep_handler::log_message_info(std::string res_code)
{
  BOOST_LOG_TRIVIAL(info) << "[ResponseMetrics] "
                          << "response_code: "
                          << res_code
                          << " "
                          << "request_path: "
                          << request_url_
                          << " "
                          << "matched_handler: sleep handler";
}