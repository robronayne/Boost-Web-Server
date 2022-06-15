#include <cstring>
#include <iostream>
#include <boost/asio/buffers_iterator.hpp>

#include "gtest/gtest.h"
#include "request_handler/profile_handler.h"

// Test for user that is logged in.
TEST(profileHandlerTest, verifiedUser)
{
  user_profile test_user;
  test_user.user_id = 1;
  test_user.username = "john.doe";
  test_user.email = "test@example.com";
  test_user.login_status = true;

  // Sample request to test.
  http::request<http::dynamic_body> request_;  
  request_.target("/profile");
  request_.set(http::field::cookie, "ngineers=\"1|john.doe|test@example.com\"");

  // Get the return reply struct from the handler function call.
  http::response<http::dynamic_body> answer;
  profile_handler handler("/profile", "test url", test_user);
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
  std::string expected_body = utility.get_profile_reply(test_user);
  // Check reply struct correctness.
  bool success = (answer.result() == http::status::ok &&
                  body == expected_body &&
                  answer.has_content_length() &&
                  headers.at(1).first == "Content-Type" &&
                  headers.at(1).second == "text/html" &&
                  status_ == answer.result());

  EXPECT_TRUE(success);
}

// Test for user that is not logged in.
TEST(profileHandlerTest, unVerifiedUser)
{
  user_profile test_user;
  test_user.login_status = false;

  // Sample request to test.
  http::request<http::dynamic_body> request_;  
  request_.target("/profile");

  // Get the return reply struct from the handler function call.
  http::response<http::dynamic_body> answer;
  profile_handler handler("/profile", "test url", test_user);
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
  std::string expected_body = utility.get_stock_reply(401);

  // Check reply struct correctness.
  bool success = (answer.result() == http::status::unauthorized &&
                  body == expected_body &&
                  answer.has_content_length() &&
                  headers.at(1).first == "Content-Type" &&
                  headers.at(1).second == "text/html" &&
                  status_ == answer.result());

  EXPECT_TRUE(success);
}
