#include <cstddef>
#include <string>
#include <boost/log/trivial.hpp>

#include "request_handler/profile_handler.h"

/**
 * Generate a profile handler with location as well as a user profile
 * to serve.
 */
profile_handler::profile_handler(std::string location, std::string request_url, user_profile profile) 
  : location_(location), request_url_(request_url), user(profile) {

}

/**
 * Construct an HTTP structured reply for the received profile request.
 * 
 * Serve profile information based on profile received in the constructor.
 * Send back HTTP 200 OK response when user is logged in, and send back a 
 * HTTP 401 Unauthorized response when no user is logged in.
 */
http::status profile_handler::serve(const http::request<http::dynamic_body> req, http::response<http::dynamic_body>& res)
{
  if (user.login_status == true)
  {
    res.result(http::status::ok);
    beast::ostream(res.body()) << utility.get_profile_reply(user);
    res.content_length((res.body().size()));
    res.set(http::field::content_type, "text/html");

    log_message_info("200");

    return res.result();
  }
  else
  {
    res.result(http::status::unauthorized);
    beast::ostream(res.body()) << utility.get_stock_reply(res.result_int());
    res.content_length((res.body().size()));
    res.set(http::field::content_type, "text/html");

    log_message_info("401");

    return res.result();
  }
}

void profile_handler::log_message_info(std::string res_code)
{
  BOOST_LOG_TRIVIAL(info) << "[ResponseMetrics] "
                          << "response_code: "
                          << res_code
                          << " "
                          << "request_path: "
                          << request_url_
                          << " "
                          << "matched_handler: profile handler";
}