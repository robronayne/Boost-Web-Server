#include <cstring>
#include <iostream>

#include "gtest/gtest.h"
#include "../src/http/reply.cc"
#include "request_handler/error_handler.h"

class errorHandlerFixture : public :: testing::Test
{
  protected:
    error_handler handler;
};

// Test the error handler for a file not found status type.
TEST_F(errorHandlerFixture, notFoundCode)
{
  // Get the return reply struct from the handler function call.
  reply answer;
  handler.set_error_code(reply::status_type::not_found);
  answer = handler.get_reply();

  // Check reply struct correctness.
  bool success = (answer.status == reply::status_type::not_found &&
                  answer.content == stock_replies::to_string(reply::status_type::not_found) &&
                  answer.headers[0].name == "Content-Length" &&
                  answer.headers[0].value == std::to_string(answer.content.size()) && 
                  answer.headers[1].name == "Content-Type" &&
                  answer.headers[1].value == "text/html");

  EXPECT_TRUE(success);
}

// Test the error handler for a bad request status type, explicitly set.
TEST_F(errorHandlerFixture, badRequestCode)
{
  // Get the return reply struct from the handler function call.
  reply answer;
  handler.set_error_code(reply::status_type::bad_request);
  answer = handler.get_reply();

  // Check reply struct correctness.
  bool success = (answer.status == reply::status_type::bad_request &&
                  answer.content == stock_replies::to_string(reply::status_type::bad_request) &&
                  answer.headers[0].name == "Content-Length" &&
                  answer.headers[0].value == std::to_string(answer.content.size()) && 
                  answer.headers[1].name == "Content-Type" &&
                  answer.headers[1].value == "text/html");
  
  EXPECT_TRUE(success);
}

// Test the error handler for a bad request status type, set by default.
TEST(errorHandlerTest, badRequest)
{
  // Get the return reply struct from the handler function call.
  reply answer;
  error_handler handler(reply::status_type::bad_request);
  answer = handler.get_reply();

  // Check reply struct correctness.
  bool success = (answer.status == reply::status_type::bad_request &&
                  answer.content == stock_replies::to_string(reply::status_type::bad_request) &&
                  answer.headers[0].name == "Content-Length" &&
                  answer.headers[0].value == std::to_string(answer.content.size()) && 
                  answer.headers[1].name == "Content-Type" &&
                  answer.headers[1].value == "text/html");
  
  EXPECT_TRUE(success);
}