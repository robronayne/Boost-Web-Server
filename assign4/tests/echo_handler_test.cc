#include "gtest/gtest.h"
#include "echo_handler.h"
#include <cstring>

class echoHandlerFixture : public ::testing::Test
{
  protected:
    echo_handler handler;
};

// Test for empty request body
TEST_F(echoHandlerFixture, emptyRequest)
{
// test request
  request request;  
  request.original_req = "";

  // get the return reply struct from the handler function call
  reply answer;
  handler.set_request(request, request.original_req.size());
  answer = handler.get_reply();

  // check reply struct correctness
  bool success = (answer.status == reply::ok &&
                  answer.content == request.original_req &&
                  answer.headers[0].name == "Content-Length" &&
                  answer.headers[0].value == std::to_string(answer.content.size()) && 
                  answer.headers[1].name == "Content-Type" &&
                  answer.headers[1].value == "text/plain");

  EXPECT_TRUE(success);
}

// Test for normal request body
TEST_F(echoHandlerFixture, normalRequest)
{
  // test request
  request request;
  request.original_req = "test request";

  // get the return reply struct from the handler function call
  reply answer;
  handler.set_request(request, request.original_req.size());
  answer = handler.get_reply();

  // check reply struct correctness
  bool success = (answer.status == reply::ok &&
                  answer.content == request.original_req &&
                  answer.headers[0].name == "Content-Length" &&
                  answer.headers[0].value == std::to_string(answer.content.size()) && 
                  answer.headers[1].name == "Content-Type" &&
                  answer.headers[1].value == "text/plain");

  EXPECT_TRUE(success);
}

// Test for second constructor
TEST(echoHandlerTest, normalRequest)
{
  // test request
  request request;
  request.original_req = "test request";

  // get the return reply struct from the handler function call
  reply answer;
  echo_handler handler(request, request.original_req.size());
  answer = handler.get_reply();

  // check reply struct correctness
  bool success = (answer.status == reply::ok &&
                  answer.content == request.original_req &&
                  answer.headers[0].name == "Content-Length" &&
                  answer.headers[0].value == std::to_string(answer.content.size()) && 
                  answer.headers[1].name == "Content-Type" &&
                  answer.headers[1].value == "text/plain");

  EXPECT_TRUE(success);
}