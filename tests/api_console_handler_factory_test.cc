#include <cstring>
#include <boost/asio/buffers_iterator.hpp>
#include "gtest/gtest.h"
#include "request_handler_factory/api_console_handler_factory.h"
// Test if the api console handler factory successfully created an api console hander (not nullptr)
TEST(apiConsoleHandlerFactoryTest, successfullyCreateApiConsoleHandler)
{
  // create factory pointer
  path p;
  user_profile profile = {0, "", "", true};
  api_console_handler_factory* factory = new api_console_handler_factory("/console", p);

  // create handler pointer
  request_handler_interface* handler = factory->create("/console", "test url", profile);

  // check if handler is nullptr
  bool success = (handler != nullptr);
  EXPECT_TRUE(success);
}
