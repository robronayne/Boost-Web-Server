#include <cstring>
#include <boost/asio/buffers_iterator.hpp>

#include "boost/filesystem.hpp"
#include "gtest/gtest.h"
#include "request_handler_factory/static_handler_factory.h"

// Test if the static handler factory successfully created an static hander (not nullptr)
TEST(staticHandlerFactoryTest, successfullyCreateStaticHandler)
{
  // create factory pointer
  path p;
  p.info_map["root"] = "test path";

  // test user profile
  user_profile profile;

  static_handler_factory* factory = new static_handler_factory("/static1", p);
  // create handler pointer
  request_handler_interface* handler = factory->create("/static1", "test url", profile);

  // check if handler is nullptr
  bool success = (handler != nullptr);

  EXPECT_TRUE(success);
}

// Test if the created static handler work as expected
TEST(staticHandlerFactoryTest, createUsableStaticHandler)
{
  // create factory pointer
  path p;
  p.info_map["root"] = "static_files/static1";

  // test user profile
  user_profile profile;

  static_handler_factory* factory = new static_handler_factory("/static1", p);
  // create handler pointer
  request_handler_interface* handler = factory->create("/static1", "/static1/example.html", profile);

  std::string root = "static_files/static1";
  std::string base_uri = "/static1/";

  // Test request.
  std::string filename = "example.html";
  http::request<http::dynamic_body> request_;  
  request_.target(base_uri + filename);

  // Get the return reply struct from the handler function call.
  http::response<http::dynamic_body> answer;
  http::status status_ = handler->serve(request_, answer);

  std::string full_path = root + "/" + filename;
  std::ifstream file_(full_path.c_str(), std::ios::in | std::ios::binary);

  // Read from the file into the reply body.
  char c;
  std::string expected = "";
  while (file_.get(c))
  {
    expected += c;
  }
  file_.close();

  std::string body { boost::asio::buffers_begin(answer.body().data()),
                     boost::asio::buffers_end(answer.body().data()) };

  std::vector<std::pair<std::string, std::string>> headers;
  for(auto const& field : answer)
  {
    std::pair<std::string, std::string> header;
    header.first = std::string(field.name_string());
    header.second = std::string(field.value());
    headers.push_back(header);
  }

  // Check reply struct correctness.
  bool success = (answer.result() == http::status::ok &&
                  body == expected &&
                  answer.has_content_length() &&
                  headers.at(0).first == "Content-Length" &&
                  headers.at(0).second == std::to_string(expected.size()) && 
                  headers.at(1).first == "Content-Type" &&
                  headers.at(1).second == "text/html" &&
                  status_ == answer.result());

  EXPECT_TRUE(success);

}