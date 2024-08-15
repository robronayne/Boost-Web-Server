#include "gtest/gtest.h"
#include "session.h"

class SessionFixture : public :: testing::Test {
protected:
  boost::asio::io_service io_service;
};

// tests to see that request is built in handle_read
TEST_F(SessionFixture, SessionStart)
{
  session s(io_service);
  std::ostream os(&s.request_);
  std::string example = "hello\r\n\r\n";
  os << example;
  std::string result = s.handle_read(boost::system::error_code(), example.size());
  EXPECT_TRUE(example == result);
}

// test if socket shutdown successfully
TEST_F(SessionFixture, TestSocketShutdown){
  session s(io_service);
  std::ostream os(&s.request_);
  std::string example = "hello\r\n\r\n";
  os << example;
  std::string result = s.handle_read(boost::system::error_code(), example.size());
  boost::system::error_code ec;
  // socket shutdown correctlyA
  EXPECT_TRUE(s.handle_write(ec));
}

// test if session start correctly
TEST_F(SessionFixture, TestSessionStart){
  session s(io_service);
  EXPECT_TRUE(s.start());
}