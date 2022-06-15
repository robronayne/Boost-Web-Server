#include "request_handler/api_console_handler.h"

/**
 * Generate an API console handler with location.
 */
api_console_handler::api_console_handler(std::string location, std::string request_url, std::string root, user_profile profile) 
  : location_(location), request_url_(request_url), root_(root), profile_(profile) {

}

/**
 * Sends logged in users to an api console where they can send requests to the api handler through the browser
 */
http::status api_console_handler::serve(const http::request<http::dynamic_body> req, http::response<http::dynamic_body>& res)
{
  if (profile_.login_status == false)
  {
    res.result(http::status::unauthorized);
    beast::ostream(res.body()) << utility.get_stock_reply(res.result_int());
    res.content_length((res.body().size()));
    res.set(http::field::content_type, "text/html");
    return res.result();
  }

  res.result(http::status::ok);
  std::string console_page;
  utility.read_file(root_ + "/" + "console.html", console_page);
  beast::ostream(res.body()) << console_page;
  res.content_length((res.body().size()));
  res.set(http::field::content_type, "text/html");
  
  return res.result();
}