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

// Test for correct response with sample user profile
TEST_F(utilFixture, userProfile)
{
  user_profile sample_profile;

  sample_profile.user_id = 1;
  sample_profile.username = "sample_user";
  sample_profile.email = "sample@example.com";
  
  std::string expected =  "<html>"
                            "<body>"
                              "<h1>User Profile</h1>"
                              "<h3>User ID number: 1</h3>" 
                              "<h3>Username: sample_user</h3>"
                              "<h3>User Email Address: sample@example.com</h3>"
                            "</body>"
                          "</html>";

  bool success = (util_.get_profile_reply(sample_profile) == expected);

  EXPECT_TRUE(success);
}

// Test for correct response with nonexistant file
TEST_F(utilFixture, fileNotExist)
{
  std::string data;
  std::string path = "/doesnotexist.txt";
  
  std::string expected =  "";

  bool success = (!util_.read_file(path, data) && data == "");

  EXPECT_TRUE(success);
}

// Test for correct response with existent file
TEST_F(utilFixture, fileDoesExist)
{
  std::string data;
  std::string path = "../Readme.md";
  
  std::string expected =  "";

  bool success = (util_.read_file(path, data) && data != "");

  EXPECT_TRUE(success);
}