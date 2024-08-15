#include <regex>
#include <string>
#include <string_view>
#include <boost/log/trivial.hpp>

#include "http/mime_types.h"
#include "request_handler/static_handler.h"

/**
 * Generate a static handler with file information from specific request.
 */
static_handler::static_handler(std::string location, std::string root_file_path, std::string request_url) : 
  location_(location), root_(root_file_path), request_url_(request_url) {}

/**
 * Generate a static HTTP structured reply for serving a static file.
 */
http::status static_handler::serve(const http::request<http::dynamic_body> req, http::response<http::dynamic_body>& res)
{
  // File name consists of characters in request after the endpoint specified.
  std::string input = req.target().to_string();
  std::string file_name = input.substr(location_.size(), std::string::npos);

  // Determine extension type from substring following final period.
  size_t ext_start = file_name.find_last_of(".");
  if (ext_start != std::string::npos)
  {
    extension = file_name.substr(ext_start + 1);
  }

  std::string full_path = root_ + "/" + file_name;

  // Attempt to serve file.
  boost::filesystem::path boost_path(full_path);
  if (!boost::filesystem::exists(boost_path) || !boost::filesystem::is_regular_file(full_path))
  {
      res.result(http::status::not_found);
      beast::ostream(res.body()) << utility.get_stock_reply(res.result_int());
      res.content_length((res.body().size()));
      res.set(http::field::content_type, "text/html");

      log_message_info("404");

      return res.result();
  }

  // Determine if file exists at local path.
  std::ifstream file_(full_path.c_str(), std::ios::in | std::ios::binary);
  if (!file_)
  {
      res.result(http::status::not_found);
      beast::ostream(res.body()) << utility.get_stock_reply(res.result_int());
      res.content_length((res.body().size()));
      res.set(http::field::content_type, "text/html");

      log_message_info("404");

      return res.result();
  }
  
  // Read from the file into the reply body
  char c;
  std::string reply_body;
  while (file_.get(c))
  {
    reply_body += c;
  }
  file_.close();

  res.result(http::status::ok);
  beast::ostream(res.body()) << reply_body;
  res.content_length((res.body().size()));
  res.set(http::field::content_type, extension_to_type(extension));

  log_message_info("200");

  return res.result();
}

void static_handler::log_message_info(std::string res_code)
{
  BOOST_LOG_TRIVIAL(info) << "[ResponseMetrics] "
                          << "response_code: "
                          << res_code
                          << " "
                          << "request_path: "
                          << request_url_
                          << " "
                          << "matched_handler: static handler";
}
