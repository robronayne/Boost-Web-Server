#include <cstring>
#include <boost/asio/buffers_iterator.hpp>

#include "gtest/gtest.h"
#include "request_handler_factory/profile_handler_factory.h"

// Test if the profile handler factory successfully created a profile hander.
TEST(profileHandlerFactoryTest, successfullyCreateProfileHandler)
{
  // Create factory pointer.
  path p;

  // Create a test profile.
  user_profile test_user;

  profile_handler_factory* factory = new profile_handler_factory("/profile", p);
  // Create handler pointer.
  request_handler_interface* handler = factory->create("/profile", "test url", test_user);

  // Check if handler is nullptr.
  bool success = (handler != nullptr);

  EXPECT_TRUE(success);
}