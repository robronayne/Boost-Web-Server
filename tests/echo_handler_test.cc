#include <cstring>

#include "gtest/gtest.h"
#include "request_handler/echo_handler.h"

class echoHandlerFixture : public ::testing::Test
{
  protected:
    echo_handler handler;
};

// Test for empty request body.
TEST_F(echoHandlerFixture, emptyRequest)
{
  // Sample request to test.
  request request_;  
  request_.original_req = "";

  // Get the return reply struct from the handler function call.
  reply answer;
  handler.set_request(request_, request_.original_req.size());
  answer = handler.get_reply();

  // Check reply struct correctness.
  bool success = (answer.status == reply::ok &&
                  answer.content == request_.original_req &&
                  answer.headers[0].name == "Content-Length" &&
                  answer.headers[0].value == std::to_string(answer.content.size()) && 
                  answer.headers[1].name == "Content-Type" &&
                  answer.headers[1].value == "text/plain");

  EXPECT_TRUE(success);
}

// Test for normal request body.
TEST_F(echoHandlerFixture, normalRequest)
{
  // Sample request to test.
  request request_;
  request_.original_req = "test request";

  // Get the return reply struct from the handler function call.
  reply answer;
  handler.set_request(request_, request_.original_req.size());
  answer = handler.get_reply();

  // Check reply struct correctness.
  bool success = (answer.status == reply::ok &&
                  answer.content == request_.original_req &&
                  answer.headers[0].name == "Content-Length" &&
                  answer.headers[0].value == std::to_string(answer.content.size()) && 
                  answer.headers[1].name == "Content-Type" &&
                  answer.headers[1].value == "text/plain");

  EXPECT_TRUE(success);
}

// Test for second constructor.
TEST(echoHandlerTest, normalRequest)
{
  // Sample request to test.
  request request_;
  request_.original_req = "test request";

  // Get the return reply struct from the handler function call.
  reply answer;
  echo_handler handler(request_, request_.original_req.size());
  answer = handler.get_reply();

  // Check reply struct correctness.
  bool success = (answer.status == reply::ok &&
                  answer.content == request_.original_req &&
                  answer.headers[0].name == "Content-Length" &&
                  answer.headers[0].value == std::to_string(answer.content.size()) && 
                  answer.headers[1].name == "Content-Type" &&
                  answer.headers[1].value == "text/plain");

  EXPECT_TRUE(success);
}