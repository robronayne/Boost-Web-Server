#include <cstring>
#include <iostream>
#include <boost/asio/buffers_iterator.hpp>

#include "gtest/gtest.h"
#include "request_handler/_404_handler.h"

// Test the error handler for a file not found status type.
TEST(_404HandlerTest, notFound)
{
  // Sample request to test.
  http::request<http::dynamic_body> request_;  

  // Get the return reply struct from the handler function call.
  http::response<http::dynamic_body> answer;
  _404_handler handler("/", "test url");
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
  
  util utility;
  std::string expected_body = utility.get_stock_reply(404);

  // Check reply struct correctness.
  bool success = (answer.result() == http::status::not_found &&
                  body == expected_body &&
                  answer.has_content_length() &&
                  headers.at(0).first == "Content-Length" &&
                  headers.at(0).second == std::to_string(expected_body.size()) &&
                  headers.at(1).first == "Content-Type" &&
                  headers.at(1).second == "text/html" &&
                  status_ == answer.result());

  EXPECT_TRUE(success);
}