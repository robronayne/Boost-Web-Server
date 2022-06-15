#include <iostream>
#include <boost/asio.hpp>
#include "boost/filesystem.hpp"

#include "server.h"
#include "session.h"
#include "mock_session.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

class ServerFixture : public ::testing::Test
{
  protected:
    mock_session mock_s;
    short port = 8080;
    boost::asio::io_service io_service;
};

// Test real session, with no mocking.
TEST_F(ServerFixture, ServerStart)
{
  session s(io_service);

  server serv(s, io_service, port);

  std::vector<path> paths;
  EXPECT_TRUE(serv.set_paths(paths));

  EXPECT_TRUE(serv.start_accept());
}

// Test mock session with no socket defintion.
TEST_F(ServerFixture, BadServerStart)
{
  // Session.start() should not be called with no socket
  EXPECT_CALL(mock_s, start()).Times(0);

  server serv(mock_s, io_service, port);

  EXPECT_FALSE(serv.start_accept());
}

// Test mock session with valid session object.
TEST_F(ServerFixture, ServerAccept)
{
  mock_session test_mock;

  // Check session.start() is called once.
  EXPECT_CALL(test_mock, start).Times(1);

  // Ensure test session mock is only returned once.
  EXPECT_CALL(mock_s, get_session)
      .WillOnce(testing::Return(&test_mock))
      .WillRepeatedly(testing::Return((session_interface*)NULL));

  server serv(mock_s, io_service, port);

  EXPECT_TRUE(serv.handle_accept(mock_s.get_session(io_service), 
    boost::system::error_code()));
}

// Test if create_handler_factory successfully create pointers (not nullptr)
TEST_F(ServerFixture, createStaticFactorySuccessfully)
{
  session s(io_service);

  server serv(s, io_service, port);

  std::map<std::string, request_handler_factory*> routes;
  // setup dummy path
  path p;
  p.type = endpoint_type::static_;
  p.endpoint = "/static";
  p.info_map["root"] = "test root";

  // create routes map
  routes = serv.create_handler_factory(p);

  bool success = (routes[p.endpoint] != nullptr);

  EXPECT_TRUE(success);

}

// Test if create_handler_factory successfully create pointers (not nullptr)
TEST_F(ServerFixture, createEchoFactorySuccessfully)
{
  session s(io_service);

  server serv(s, io_service, port);

  std::map<std::string, request_handler_factory*> routes;
  // setup dummy path
  path p;
  p.type = endpoint_type::echo;
  p.endpoint = "/echo";
  p.info_map["root"] = "test root";

  // create routes map
  routes = serv.create_handler_factory(p);

  bool success = (routes[p.endpoint] != nullptr);

  EXPECT_TRUE(success);

}

// Test if create_handler_factory successfully create pointers (not nullptr)
TEST_F(ServerFixture, create404FactorySuccessfully)
{
  session s(io_service);

  server serv(s, io_service, port);

  std::map<std::string, request_handler_factory*> routes;
  // setup dummy path
  path p;
  p.type = endpoint_type::not_found;
  p.endpoint = "/";
  p.info_map["root"] = " ";

  // create routes map
  routes = serv.create_handler_factory(p);

  bool success = (routes[p.endpoint] != nullptr);

  EXPECT_TRUE(success);

}

// Test if the createRoutes successfully reach the end
TEST_F(ServerFixture, createRoutes)
{
  session s(io_service);

  server serv(s, io_service, port);

  // setup dummy path
  path p;
  p.type = endpoint_type::echo;
  p.endpoint = "/echo";
  p.info_map["root"] = " ";

  // create path vector
  std::vector<path> path_vec;
  path_vec.push_back(p);
  serv.set_paths(path_vec);

  bool success = serv.create_routes();

  EXPECT_TRUE(success);

}
/* 
 * Temporarily remove test until smart pointers are added
 *
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
  }
 * 
 */