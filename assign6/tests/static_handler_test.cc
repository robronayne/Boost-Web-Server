#include "boost/filesystem.hpp"
#include <boost/asio/buffers_iterator.hpp>

#include "gtest/gtest.h"
#include "request_handler/static_handler.h"

class staticHandlerFixture : public :: testing::Test
{
  protected:
    std::string root = "static_files/static1";
    std::string base_uri = "/static1/";
};

// Test normal file response.
TEST_F(staticHandlerFixture, normalFile)
{
  // Test request.
  std::string filename = "example.html";
  http::request<http::dynamic_body> request_;  
  request_.target(base_uri + filename);

  // Get the return reply struct from the handler function call.
  http::response<http::dynamic_body> answer;
  static_handler handler(base_uri, root, request_.target().to_string());
  http::status status_ = handler.serve(request_, answer);

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

// Test handling for non-existent file.
TEST_F(staticHandlerFixture, badFile)
{
  // Test request.
  std::string filename = "doesnotexist";
  http::request<http::dynamic_body> request_;  
  request_.target(base_uri + filename);

  // Get the return reply struct from the handler function call.
  http::response<http::dynamic_body> answer;
  static_handler handler(base_uri, root, request_.target().to_string());
  http::status status_ = handler.serve(request_, answer);

  // Check reply struct correctness.
  bool success = (answer.result() == http::status::not_found && 
                  status_ == answer.result());

  EXPECT_TRUE(success);
}