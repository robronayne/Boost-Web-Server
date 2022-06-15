#include "boost/filesystem.hpp"

#include "gtest/gtest.h"
#include "request_handler/static_handler.h"

class staticHandlerFixture : public :: testing::Test
{
  protected:
    static_handler handler;
    std::string root = "static_files/static1";
    std::string base_uri = "/static1/";
};

// Test normal file response.
TEST_F(staticHandlerFixture, normalFile)
{
  // Test request.
  std::string filename = "example.html";
  request request_;  
  request_.uri = base_uri + filename;

  // Get the return reply struct from the handler function call.
  reply answer;
  handler.set_request(request_, root);
  answer = handler.get_reply();

  std::string full_path = root + "/" + filename;
  std::ifstream file_(full_path.c_str(), std::ios::in | std::ios::binary);

  // Read from the file into the reply body.
  char c;
  std::string expected = "";
  while (file_.get(c))
  {
    expected += c;
  }
  file_.close();

  // Check reply struct correctness.
  bool success = (answer.status == reply::ok &&
                  answer.content == expected &&
                  answer.headers[0].name == "Content-Length" &&
                  answer.headers[0].value == std::to_string(answer.content.size()) && 
                  answer.headers[1].name == "Content-Type" &&
                  answer.headers[1].value == "text/html");

  EXPECT_TRUE(success);
}

// Test handling for non-existent file.
TEST_F(staticHandlerFixture, badFile)
{
  // Test request.
  std::string filename = "doesnotexist";
  request request_;  
  request_.uri = base_uri + filename;

  // Get the return reply struct from the handler function call.
  reply answer;
  handler.set_request(request_, root);
  answer = handler.get_reply();

  // Check reply struct correctness.
  bool success = (answer.status == reply::not_found);

  EXPECT_TRUE(success);
}