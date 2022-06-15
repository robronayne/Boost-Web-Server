#include <cstddef>
#include <string>
#include <vector>
#include <boost/log/trivial.hpp>

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

    log_message_info("404");

    return res.result();
  }
  res.result(http::status::ok);
  beast::ostream(res.body()) << req;
  res.content_length((res.body().size()));
  res.set(http::field::content_type, "text/plain");

  log_message_info("200");

  return res.result();
}

void echo_handler::log_message_info(std::string res_code)
{
  BOOST_LOG_TRIVIAL(info) << "[ResponseMetrics] "
                          << "response_code: "
                          << res_code
                          << " "
                          << "request_path: "
                          << request_url_
                          << " "
                          << "matched_handler: echo handler";
}