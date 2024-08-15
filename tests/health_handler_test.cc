#include <cstring>
#include <boost/asio/buffers_iterator.hpp>

#include "gtest/gtest.h"
#include "request_handler/health_handler.h"

// Test for health request body.
TEST(healthHandlerTest, normalRequest)
{
  // Sample request to test.
  http::request<http::dynamic_body> request_;  
  request_.target("/health");

  // Get the return reply struct from the handler function call.
  http::response<http::dynamic_body> answer;
  health_handler handler("/health", "test url");
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
                  body == "OK" &&
                  answer.has_content_length() &&
                  headers.at(1).first == "Content-Type" &&
                  headers.at(1).second == "text/plain" &&
                  status_ == answer.result());

  EXPECT_TRUE(success);
}

// Test for bad request target
TEST(healthHandlerTest, badRequestTarget)
{
  // Sample request to test.
  http::request<http::dynamic_body> request_;  
  request_.target("/health/bad");
  beast::ostream(request_.body()) << "test request";

  std::ostringstream ostring;
  ostring << request_;
  std::string request_string = ostring.str();

  // Get the return reply struct from the handler function call.
  http::response<http::dynamic_body> answer;
  health_handler handler("/health", "test url");
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
  bool success = (answer.result() == http::status::not_found && 
                  status_ == answer.result());

  EXPECT_TRUE(success);
}