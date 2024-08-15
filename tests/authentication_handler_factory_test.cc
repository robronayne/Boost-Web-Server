#include <cstring>
#include <boost/asio/buffers_iterator.hpp>

#include "gtest/gtest.h"
#include "request_handler_factory/authentication_handler_factory.h"

// Test if the echo handler factory successfully created an echo hander (not nullptr)
TEST(authenticationHandlerFactoryTest, successfullyCreateAuthHandler)
{
  // create factory pointer
  path p;
  p.info_map["root"] = "test path";
  authentication_handler_factory* factory = new authentication_handler_factory("/echo", p);

  // test user profile
  user_profile profile;

  // create handler pointer
  request_handler_interface* handler = factory->create("/echo", "test url", profile);

  // check if handler is nullptr
  bool success = (handler != nullptr);

  EXPECT_TRUE(success);
}