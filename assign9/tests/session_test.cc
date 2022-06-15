#include "session.h"
#include "gtest/gtest.h"

class SessionFixture : public :: testing::Test 
{
  protected:
    boost::asio::io_service io_service;
    path p;
    std::map<std::string, request_handler_factory*> routes;

    void SetUp() override 
    {
      routes[""] = new echo_handler_factory("", p);
      routes["/"] = new _404_handler_factory("", p);
    }
};

// Tests to see that request is built in handle_read.
TEST_F(SessionFixture, SessionStart)
{
  session s(io_service);

  EXPECT_TRUE(s.set_routes(routes));

  http::request<http::dynamic_body> request_;  
  beast::ostream(request_.body()) << "test request";

  std::ostringstream ostring;
  ostring << request_;
  std::string request_string = ostring.str();

  EXPECT_TRUE(s.set_request(request_));
  std::string result = s.handle_read(boost::system::error_code(), request_.body().size());
  std::cout << result << std::endl;
  EXPECT_TRUE(result == request_string);
}

// Tests to see that request is not built in handle_read if invalid.
TEST_F(SessionFixture, InvalidSessionStart)
{
  session s(io_service);
  EXPECT_TRUE(s.set_routes(routes));
  std::ostream os(&s.buffer_);
  std::string example = "hello\r\n\r\n";
  os << example;
  std::string result = s.handle_read(boost::system::error_code(), example.size());
  EXPECT_TRUE(example != result);
}

// Test if socket shutdown successfully.
TEST_F(SessionFixture, TestSocketShutdown)
{
  session s(io_service);
  EXPECT_TRUE(s.set_routes(routes));
  std::ostream os(&s.buffer_);
  std::string example = "hello\r\n\r\n";
  os << example;
  std::string result = s.handle_read(boost::system::error_code(), example.size());
  boost::system::error_code ec;
  // socket shutdown correctly
  EXPECT_TRUE(s.handle_write(ec));
}

// Test if session start correctly.
TEST_F(SessionFixture, TestSessionStart)
{
  session s(io_service);
  EXPECT_TRUE(s.start());
}

// Test the member function match in session class
TEST_F(SessionFixture, TestSuccessfulLongestPrefixMatch)
{
  session s(io_service);
  // Map of routes
  std::map<std::string, request_handler_factory*> routes = {
    {"/static", nullptr},
    {"/static1", nullptr},
    {"/echo", nullptr},
    {"/static/xyz", nullptr},
    {"/static1/zzz", nullptr},
    {"/static1/zzz/icy", nullptr},
    {"/static/xyz/abc", nullptr},
    {"/static/xyz/abc/def", nullptr},
  };

  bool success = (s.match(routes, "/static") == "/static") &&
                  (s.match(routes, "/static1") == "/static1") &&
                  (s.match(routes, "/echo") == "/echo") &&
                  (s.match(routes, "/static/bbb") == "/static") &&
                  (s.match(routes, "/static1/zzz") == "/static1/zzz") &&
                  (s.match(routes, "/static1/zzz/icy/aaa") == "/static1/zzz/icy") &&
                  (s.match(routes, "/static/xyz/bcd") == "/static/xyz") &&
                  (s.match(routes, "/static/xyz/abc/def/efg") == "/static/xyz/abc/def") &&
                  (s.match(routes, "/static/") == "/static") ;
  
  EXPECT_TRUE(success);

}

TEST_F(SessionFixture, TestUnsuccessfulLongestPrefixMatch)
{
  session s(io_service);
  // Map of routes
  std::map<std::string, request_handler_factory*> routes = {
    {"/static", nullptr},
    {"/static1", nullptr},
    {"/echo", nullptr},
    {"/static/xyz", nullptr},
    {"/static1/zzz", nullptr},
    {"/static1/zzz/icy", nullptr},
    {"/static/xyz/abc", nullptr},
    {"/static/xyz/abc/def", nullptr},
  };

  bool success = (s.match(routes, "/static3") == "/") &&
                  (s.match(routes, "/") == "/") &&
                  (s.match(routes, "/echo  ") == "/") &&
                  (s.match(routes, "/static2/xzz/abc") == "/") &&
                  (s.match(routes, "") == "/") ;
  
  EXPECT_TRUE(success);

}

// Test set_routes function
TEST_F(SessionFixture, setRoutes)
{
  session s(io_service);

  std::map<std::string, request_handler_factory*> routes;
  EXPECT_TRUE(s.set_routes(routes));
}

// Test if cookie parses properly.
TEST_F(SessionFixture, TestGoodCookieParsing)
{
  session s(io_service);

  http::request<http::dynamic_body> request_;  
  beast::ostream(request_.body()) << "test request";
  request_.set(http::field::cookie, "ngineers=\"0|test|testy@testing.com\"");

  user_profile test_user;
  bool result = s.update_user(std::string(request_[http::field::cookie]), test_user);
  
  bool success = (result && test_user.user_id == 0 &&
                 test_user.username == "test" && 
                 test_user.email == "testy@testing.com");
  EXPECT_TRUE(success);
}

// Test if bad cookie doesn't parse.
TEST_F(SessionFixture, TestBadCookieParsing)
{
  session s(io_service);

  http::request<http::dynamic_body> request_;  
  beast::ostream(request_.body()) << "test request";
  request_.set(http::field::cookie, "ngineers=\"ryery\"");

  user_profile test_user;
  bool result = s.update_user(std::string(request_[http::field::cookie]), test_user);
  
  EXPECT_TRUE(!result);
}

// Test if no cookies exist.
TEST_F(SessionFixture, TestNoCookieParsing)
{
  session s(io_service);

  http::request<http::dynamic_body> request_;  
  beast::ostream(request_.body()) << "test request";
  request_.set(http::field::cookie, "");

  user_profile test_user;
  bool result = s.update_user(std::string(request_[http::field::cookie]), test_user);
  
  EXPECT_TRUE(!result);
}