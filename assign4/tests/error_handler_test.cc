#include "gtest/gtest.h"
#include "error_handler.h"
#include <cstring>
#include <iostream>
#include "../src/reply.cc"

class errorHandlerFixture : public :: testing::Test
{
  protected:
    error_handler handler;
};

TEST_F(errorHandlerFixture, notFoundCode)
{
  // get the return reply struct from the handler function call
  reply answer;
  handler.set_error_code(reply::status_type::not_found);
  answer = handler.get_reply();

  // check reply struct correctness
  bool success = (answer.status == reply::status_type::not_found &&
                  answer.content == stock_replies::to_string(reply::status_type::not_found) &&
                  answer.headers[0].name == "Content-Length" &&
                  answer.headers[0].value == std::to_string(answer.content.size()) && 
                  answer.headers[1].name == "Content-Type" &&
                  answer.headers[1].value == "text/html");

  EXPECT_TRUE(success);
}

TEST_F(errorHandlerFixture, badRequestCode)
{
  // get the return reply struct from the handler function call
  reply answer;
  handler.set_error_code(reply::status_type::bad_request);
  answer = handler.get_reply();

  // check reply struct correctness
  bool success = (answer.status == reply::status_type::bad_request &&
                  answer.content == stock_replies::to_string(reply::status_type::bad_request) &&
                  answer.headers[0].name == "Content-Length" &&
                  answer.headers[0].value == std::to_string(answer.content.size()) && 
                  answer.headers[1].name == "Content-Type" &&
                  answer.headers[1].value == "text/html");
  
  EXPECT_TRUE(success);
}

TEST(errorHandlerTest, badRequest)
{
  // get the return reply struct from the handler function call
  reply answer;
  error_handler handler(reply::status_type::bad_request);
  answer = handler.get_reply();

  // check reply struct correctness
  bool success = (answer.status == reply::status_type::bad_request &&
                  answer.content == stock_replies::to_string(reply::status_type::bad_request) &&
                  answer.headers[0].name == "Content-Length" &&
                  answer.headers[0].value == std::to_string(answer.content.size()) && 
                  answer.headers[1].name == "Content-Type" &&
                  answer.headers[1].value == "text/html");
  
  EXPECT_TRUE(success);
}