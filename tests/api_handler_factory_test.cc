#include <cstring>
#include <boost/asio/buffers_iterator.hpp>

#include <boost/filesystem.hpp>
#include "gtest/gtest.h"
#include "request_handler_factory/request_handler_factory.h"
#include "request_handler_factory/api_handler_factory.h"
#include "request_handler/api_handler.h"

TEST(apiHandlerFactoryTest, successfullyCreateAPIHandler)
{
  std::string location = "/api";
  std::string request_url = "/api/test";
  path p;
  p.info_map["data_path"] = "../database";

  api_handler_factory* factory = new api_handler_factory(location, p);

  // test user profile
  user_profile profile;

  request_handler_interface* handler = factory->create(location, "test url", profile);

  bool success = (handler != nullptr);

  EXPECT_TRUE(success);
}