#include "util.h"
#include "gtest/gtest.h"

class utilFixture : public ::testing::Test
{
  protected:
    util util_;
};

// Test for correct response with ok error code
TEST_F(utilFixture, okRequest)
{
  std::string expected = "";
  bool success = (util_.get_stock_reply(200) == expected);

  EXPECT_TRUE(success);
}

// Test for correct response with bad request error code
TEST_F(utilFixture, badRequest)
{
  std::string expected = "<html>"
                         "<head><title>Bad Request</title></head>"
                         "<body><h1>400 Bad Request</h1></body>"
                         "</html>";
  bool success = (util_.get_stock_reply(400) == expected);

  EXPECT_TRUE(success);
}

// Test for correct response with not found error code
TEST_F(utilFixture, requestNotFound)
{
  std::string expected = "<html>"
                         "<head><title>Not Found</title></head>"
                         "<body><h1>404 Not Found</h1></body>"
                         "</html>";
  bool success = (util_.get_stock_reply(404) == expected);

  EXPECT_TRUE(success);
}