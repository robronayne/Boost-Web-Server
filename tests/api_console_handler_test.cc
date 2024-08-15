#include <cstring>
#include <boost/asio/buffers_iterator.hpp>
#include "gtest/gtest.h"
#include "request_handler/api_console_handler.h"
// Test for Api Console request body
TEST(ApiConsoleHandlerTest, normalRequest)
{
  // Sample request to test.
  http::request<http::dynamic_body> request_;  
  request_.target("/console");
  // Get the return reply struct from the handler function call
  http::response<http::dynamic_body> answer;

  user_profile profile = {0, "", "", true};
  api_console_handler handler("/console", "test url", "test", profile);
  http::status status_ = handler.serve(request_, answer);
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
                  answer.has_content_length() &&
                  headers.at(1).first == "Content-Type" &&
                  headers.at(1).second == "text/html" &&
                  status_ == answer.result());
  EXPECT_TRUE(success);
}

// Test for not logged in to fail
TEST(ApiConsoleHandlerTest, badRequest)
{
  // Sample request to test.
  http::request<http::dynamic_body> request_;  
  request_.target("/console");
  // Get the return reply struct from the handler function call
  http::response<http::dynamic_body> answer;

  user_profile profile = {0, "", "", false};
  api_console_handler handler("/console", "test url", "test", profile);
  http::status status_ = handler.serve(request_, answer);
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
  bool success = (answer.result() == http::status::unauthorized &&
                  answer.has_content_length() &&
                  headers.at(1).first == "Content-Type" &&
                  headers.at(1).second == "text/html" &&
                  status_ == answer.result());
  EXPECT_TRUE(success);
}
