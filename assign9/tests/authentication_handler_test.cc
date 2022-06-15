#include <cstring>
#include <iostream>
#include <fstream>
#include <boost/asio/buffers_iterator.hpp>

#include "gtest/gtest.h"
#include "request_handler/authentication_handler.h"

// Test for empty request body.
TEST(authenticationHandlerTest, emptyRequest)
{
  // Sample request to test.
  http::request<http::dynamic_body> request_;  
  request_.target("/auth");
  user_profile profile = {0, "", "", true};

  std::ostringstream ostring;
  ostring << request_;
  std::string request_string = ostring.str();


  // Get the return reply struct from the handler function call.
  http::response<http::dynamic_body> answer;
  authentication_handler handler("/auth", "test url", "test", "test",
                                 "/login", "/logout", "/signup", profile);
  http::status status_ = handler.serve(request_, answer);

  EXPECT_TRUE( status_ == http::status::not_found);
}

// Test for login page.
TEST(authenticationHandlerTest, loginHandler)
{
  // Sample request to test.
  http::request<http::dynamic_body> request_;
  request_.target("/auth/login");
  user_profile profile = {0, "", "", true};

  std::ostringstream ostring;
  ostring << request_;
  std::string request_string = ostring.str();

  // Get the return reply struct from the handler function call.
  http::response<http::dynamic_body> answer;
  authentication_handler handler("/auth", "test url", "test", "test",
                                 "/login", "/logout", "/signup", profile);
  http::status status_ = handler.serve(request_, answer);

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

// Test for login-submit user info.
TEST(authenticationHandlerTest, loginSubmitHandler)
{
  // Sample request to test.
  http::request<http::dynamic_body> request_;  
  request_.target("/auth/login-submit");
  request_.method(http::verb::post);
  beast::ostream(request_.body()) << "OAuth|test|testy@testing.com";
  user_profile profile = {0, "", "", false};

  std::ostringstream ostring;
  ostring << request_;
  std::string request_string = ostring.str();

  // Reset database to test user.
  const std::string data_path = "../tests/user_database.txt";
  std::ofstream database_reset(data_path);
  database_reset << "1|1|testy@testing.com|test|N/A" << std::endl;

  // Get the return reply struct from the handler function call.
  http::response<http::dynamic_body> answer;
    authentication_handler handler("/auth", "test url", "../tests", "../tests",
                                   "/login", "/logout", "/signup", profile);
  http::status status_ = handler.serve(request_, answer);

  // Get set-cookie header
  std::string answer_cookie = std::string(answer[http::field::set_cookie]);
  // Check reply struct correctness.
  bool success = (answer.result() == http::status::ok &&
                  answer_cookie == "ngineers=\"1|test|testy@testing.com\"; Path=/" &&
                  status_ == answer.result());

  EXPECT_TRUE(success);
}

// Test for login-submit bad user info.
TEST(authenticationHandlerTest, badLoginSubmitHandler)
{
  // Sample request to test.
  http::request<http::dynamic_body> request_;  
  request_.target("/auth/login-submit");
  request_.method(http::verb::post);
  beast::ostream(request_.body()) << "badcookie";
  user_profile profile = {0, "", "", false};

  std::ostringstream ostring;
  ostring << request_;
  std::string request_string = ostring.str();

  // Reset database to test user.
  const std::string data_path = "../tests/user_database.txt";
  std::ofstream database_reset(data_path);
  database_reset << "1|1|testy@testing.com|test|N/A" << std::endl;

  // Get the return reply struct from the handler function call.
  http::response<http::dynamic_body> answer;
  authentication_handler handler("/auth", "test url", "../tests", "../tests",
                                 "/login", "/logout", "/signup", profile);
  http::status status_ = handler.serve(request_, answer);

  // Get set-cookie header
  std::string answer_cookie = std::string(answer[http::field::set_cookie]);
  // Check reply struct correctness.
  bool success = (answer.result() == http::status::bad_request &&
                  status_ == answer.result());

  EXPECT_TRUE(success);
}

// Test for logout user.
TEST(authenticationHandlerTest, logoutHandler)
{
  // Sample request to test.
  http::request<http::dynamic_body> request_;
  request_.target("/auth/logout");
  request_.method(http::verb::get);
  user_profile profile = {0, "", "", false};

  std::ostringstream ostring;
  ostring << request_;
  std::string request_string = ostring.str();

  // Get the return reply struct from the handler function call.
  http::response<http::dynamic_body> answer;
  authentication_handler handler("/auth", "test url", "test", "test",
                                 "/login", "/logout", "/signup", profile);
  http::status status_ = handler.serve(request_, answer);

  // Get set-cookie header
  std::string answer_cookie = std::string(answer[http::field::set_cookie]);
  // Check reply struct correctness.
  bool success = (answer.result() == http::status::ok &&
                  answer_cookie == "ngineers=; Path=/; Max-Age=0" &&
                  status_ == answer.result());

  EXPECT_TRUE(success);
}

// Test for signup pages.
TEST(authenticationHandlerTest, signupHandler)
{
  // Sample request to test.
  http::request<http::dynamic_body> request_;
  request_.target("/auth/signup");
  user_profile profile = {0, "", "", true};

  std::ostringstream ostring;
  ostring << request_;
  std::string request_string = ostring.str();

  // Get the return reply struct from the handler function call.
  http::response<http::dynamic_body> answer;
  authentication_handler handler("/auth", "test url", "test", "test",
                                 "/login", "/logout", "/signup", profile);
  http::status status_ = handler.serve(request_, answer);

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

// Test for good signup-submit user info.
TEST(authenticationHandlerTest, goodSignupSubmitHandler)
{
  // Sample request to test.
  http::request<http::dynamic_body> request_;  
  request_.target("/auth/signup-submit");
  request_.method(http::verb::post);
  beast::ostream(request_.body()) << "test|password|testy@testing.com";
  user_profile profile = {0, "", "", false};

  std::ostringstream ostring;
  ostring << request_;
  std::string request_string = ostring.str();

  // Get the return reply struct from the handler function call.
  http::response<http::dynamic_body> answer;
  authentication_handler handler("/auth", "test url", "test", "test",
                                 "/login", "/logout", "/signup", profile);
  http::status status_ = handler.serve(request_, answer);

  // Check reply struct correctness.
  bool success = (answer.result() == http::status::ok &&
                  status_ == answer.result());

  EXPECT_TRUE(success);
}

// Test for bad signup-submit user info.
TEST(authenticationHandlerTest, badSignupSubmitHandler)
{
  // Sample request to test.
  http::request<http::dynamic_body> request_;  
  request_.target("/auth/signup-submit");
  request_.method(http::verb::post);
  beast::ostream(request_.body()) << "badsignupsubmit";
  user_profile profile = {0, "", "", false};

  std::ostringstream ostring;
  ostring << request_;
  std::string request_string = ostring.str();

  // Get the return reply struct from the handler function call.
  http::response<http::dynamic_body> answer;
  authentication_handler handler("/auth", "test url", "test", "test",
                                 "/login", "/logout", "/signup", profile);
  http::status status_ = handler.serve(request_, answer);

  // Check reply struct correctness.
  bool success = (answer.result() == http::status::bad_request &&
                  status_ == answer.result());

  EXPECT_TRUE(success);
}