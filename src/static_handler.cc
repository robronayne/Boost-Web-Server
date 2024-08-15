#include "static_handler.h"
#include "mime_types.h"

#include <regex>

static_handler::static_handler()
{
  ec = reply::status_type::not_found;
  extension = "";
  uri = "";
}

static_handler::static_handler(request request_, std::string root) : 
  uri(request_.uri), method(request_.method), root_(root)
{

}

void static_handler::set_request(request request_, std::string root)
{
  uri = request_.uri;
  method = request_.method;
  root_ = root;
}

reply static_handler::get_reply()
{
  std::regex e ("^(/[a-zA-Z_0-9]+)*/([a-zA-Z_0-9.]+)$");

  std::smatch match;
  std::regex_search(uri, match, e);

  std::string file_name = match.str(2);

  size_t ext_start = file_name.find_last_of(".");
  if (ext_start != std::string::npos)
  {
    extension = file_name.substr(ext_start + 1);
  }

  std::string full_path = root_ + "/" + file_name;

  // Attempt to serve file
  boost::filesystem::path boost_path(full_path);
  if (!boost::filesystem::exists(boost_path) || !boost::filesystem::is_regular_file(full_path))
  {
    ec = reply::status_type::not_found;
    return rep.stock_reply(ec);
  }

  // Determine if file exists at local path
  std::ifstream file_(full_path.c_str(), std::ios::in | std::ios::binary);
  if (!file_)
  {
    ec = reply::status_type::not_found;
    return rep.stock_reply(ec);
  }
  

  // Read from the file into the reply body
  ec = reply::status_type::ok;
  char c;
  std::string reply_body;
  while (file_.get(c))
  {
    reply_body += c;
  }
  file_.close();

  rep.status = reply::ok;
  rep.headers.resize(2);
  rep.content = reply_body;
  rep.headers[0].name = "Content-Length";
  rep.headers[0].value = std::to_string(rep.content.size());
  rep.headers[1].name = "Content-Type";
  rep.headers[1].value = extension_to_type(extension);
  return rep;



}
