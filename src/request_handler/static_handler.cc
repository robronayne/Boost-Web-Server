#include <regex>

#include "http/mime_types.h"
#include "request_handler/static_handler.h"

/**
 * Constructor for a generic static handler, containing no file information.
 */
static_handler::static_handler()
{
  ec = reply::status_type::not_found;
  extension = "";
  uri = "";
}

/**
 * Generate a static handler with file information from specific request.
 */
static_handler::static_handler(request request_, std::string root) : 
  uri(request_.uri), method(request_.method), root_(root) {}

/**
 * Setter method for modifying the HTTP request received.
 */
void static_handler::set_request(request request_, std::string root)
{
  uri = request_.uri;
  method = request_.method;
  root_ = root;
}

/**
 * Generate a static HTTP structured reply for serving a static file.
 */
reply static_handler::get_reply()
{
  // Regex expression for matching filename.
  std::regex exp ("^(/[a-zA-Z_0-9]+)*/([a-zA-Z_0-9.]+)$");

  std::smatch match;
  std::regex_search(uri, match, exp);

  std::string file_name = match.str(2);

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
    ec = reply::status_type::not_found;
    return reply_.stock_reply(ec);
  }

  // Determine if file exists at local path.
  std::ifstream file_(full_path.c_str(), std::ios::in | std::ios::binary);
  if (!file_)
  {
    ec = reply::status_type::not_found;
    return reply_.stock_reply(ec);
  }
  ec = reply::status_type::ok;
  
  // Read from the file into the reply body
  char c;
  std::string reply_body;
  while (file_.get(c))
  {
    reply_body += c;
  }
  file_.close();

  reply_.status = reply::ok;
  reply_.headers.resize(2);
  reply_.content = reply_body;
  reply_.headers[0].name = "Content-Length";
  reply_.headers[0].value = std::to_string(reply_.content.size());
  reply_.headers[1].name = "Content-Type";
  reply_.headers[1].value = extension_to_type(extension);
  return reply_;
}
