#include "gtest/gtest.h"
#include "static_handler.h"
#include "boost/filesystem.hpp"

class staticHandlerFixture : public :: testing::Test
{
  protected:
    static_handler handler;
    std::string root = "static_files/static1";
    std::string base_uri = "/static1/";
};

// test normal file response
TEST_F(staticHandlerFixture, normalFile)
{
// test request
  std::string f = "example.html";
  request request;  
  request.uri = base_uri + f;

  // get the return reply struct from the handler function call
  reply answer;
  handler.set_request(request, root);
  answer = handler.get_reply();

  std::string full_path = root + "/" + f;
  std::ifstream file_(full_path.c_str(), std::ios::in | std::ios::binary);

  // Read from the file into the reply body
  char c;
  std::string expected = "";
  while (file_.get(c))
  {
    expected += c;
  }
  file_.close();

  // check reply struct correctness
  bool success = (answer.status == reply::ok &&
                  answer.content == expected &&
                  answer.headers[0].name == "Content-Length" &&
                  answer.headers[0].value == std::to_string(answer.content.size()) && 
                  answer.headers[1].name == "Content-Type" &&
                  answer.headers[1].value == "text/html");

  EXPECT_TRUE(success);
}

// test bad file response
TEST_F(staticHandlerFixture, badFile)
{
// test request
  std::string f = "doesnotexist";
  request request;  
  request.uri = base_uri + f;

  // get the return reply struct from the handler function call
  reply answer;
  handler.set_request(request, root);
  answer = handler.get_reply();\

  // check reply struct correctness
  bool success = (answer.status == reply::not_found);

  EXPECT_TRUE(success);
}