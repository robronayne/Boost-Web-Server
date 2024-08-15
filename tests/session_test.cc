#include "session.h"
#include "gtest/gtest.h"

class SessionFixture : public :: testing::Test 
{
  protected:
    boost::asio::io_service io_service;
};

// Tests to see that request is built in handle_read.
TEST_F(SessionFixture, SessionStart)
{
  session s(io_service);

  std::vector<path> paths;
  EXPECT_TRUE(s.set_paths(paths));

  std::ostream os(&s.buffer_);
  std::string example = "GET /docs/index.html HTTP/1.1\r\nHost: www.nowhere123.com\r\n\r\n";
  os << example;
  std::string result = s.handle_read(boost::system::error_code(), example.size());
  EXPECT_TRUE(example == result);
}

// Tests to see that request is not built in handle_read if invalid.
TEST_F(SessionFixture, InvalidSessionStart)
{
  session s(io_service);
  std::ostream os(&s.buffer_);
  std::string example = "hello\r\n\r\n";
  os << example;
  std::string result = s.handle_read(boost::system::error_code(), example.size());
  EXPECT_TRUE(example != result);
}

// Test if socket shutdown successfully.
TEST_F(SessionFixture, TestSocketShutdown){
  session s(io_service);
  std::ostream os(&s.buffer_);
  std::string example = "hello\r\n\r\n";
  os << example;
  std::string result = s.handle_read(boost::system::error_code(), example.size());
  boost::system::error_code ec;
  // socket shutdown correctly
  EXPECT_TRUE(s.handle_write(ec));
}

// Test if session start correctly.
TEST_F(SessionFixture, TestSessionStart){
  session s(io_service);
  EXPECT_TRUE(s.start());
}