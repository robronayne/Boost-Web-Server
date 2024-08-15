#include <boost/asio.hpp>
#include <iostream>

#include "server.h"
#include "mock_session.h"
#include "session.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
// Use fixture for mock tests
class ServerFixture : public ::testing::Test{
  protected:
    mock_session mock_s;
    short port = 8080;
    boost::asio::io_service io_service;
};

// Test real session, with no mocking
// Figure out how to mock sockets for this
TEST_F(ServerFixture, ServerStart)
{
  session s(io_service);

  server serv(s, io_service, port);

  std::vector<path> paths;
  EXPECT_TRUE(serv.set_paths(paths));

  EXPECT_TRUE(serv.start_accept());
}

// Test mock session with no socket defintion
TEST_F(ServerFixture, BadServerStart)
{
  // Session.start() should not be called with no socket
  EXPECT_CALL(mock_s, start()).Times(0);

  server serv(mock_s, io_service, port);

  EXPECT_FALSE(serv.start_accept());
}

// Test mock session with valid session object
TEST_F(ServerFixture, ServerAccept)
{
  mock_session test_mock;

  // Check session.start() is called once
  EXPECT_CALL(test_mock, start).Times(1);

  // Ensure test session mock is only returned once
  EXPECT_CALL(mock_s, get_session)
      .WillOnce(testing::Return(&test_mock))
      .WillRepeatedly(testing::Return((session_interface*)NULL));

  server serv(mock_s, io_service, port);

  EXPECT_TRUE(serv.handle_accept(mock_s.get_session(io_service), 
    boost::system::error_code()));
}

/** 
// Temporarily remove test until smart pointers are added
TEST_F(ServerFixture, BadServerAccept)
{
  mock_session test_mock;

  EXPECT_CALL(test_mock, start).Times(0);

  EXPECT_CALL(s, get_session)
      .WillOnce(testing::Return(&test_mock))
      .WillRepeatedly(testing::Return((session_interface*)NULL));

  server serv(s, io_service, port);

  EXPECT_FALSE(serv.handle_accept(s.get_session(io_service), 
    boost::system::error_code(1, boost::system::system_category()))));
}**/