#include <iostream>
#include <cstring>

#include "http/reply.h"
#include "gtest/gtest.h"

class replyFixture : public ::testing::Test
{
  protected:
    reply reply_;
  
    void SetUp() override
    {
      reply_.headers.resize(1);
      reply_.headers[0].name = "Content-Type";
      reply_.headers[0].value = "text/plain";
    }

    // Helper function to convert const buffers to strings.
    std::string buffer_to_string(std::vector<boost::asio::const_buffer>& resp, int pos) 
    {
      if (resp.at(pos).size() == 0)
      {
        return "";
      }

      const unsigned char* unsigned_str_ele = static_cast<const unsigned char*> (resp.at(pos).data());
      const char* signed_str_ele = (char*) unsigned_str_ele;
      std::string str_ele = std::string{signed_str_ele};
      return str_ele;
    }
};

// Test reply for empty request.
TEST_F(replyFixture, emptyRequest)
{
  // Setting the status code to OK (200).
  reply_.status = reply::status_type::ok;

  // Test request.
  std::string request = "";

  // Expected responding header.
  std::string expected = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n";
  // Add request to the header.
  expected = expected + request;

  // Set request content.
  reply_.content = request;

  // Get the return vector of boost::asio:const_buffer from the reply function call.
  std::vector<boost::asio::const_buffer> answer;
  answer = reply_.to_buffers();

  /*
   * Get the data out from the vector:
   * 1. Cast it to unsign char* 
   * 2. Cast it to char*
   * 3. Convert it to string for compare 
   */
  std::string result = "";
  for (int i = 0; i < answer.size(); i++)
  {
    result += buffer_to_string(answer, i);
  }
  
  bool success = (expected == result);

  EXPECT_TRUE(success);
}

// Test reply for sample test.
TEST_F(replyFixture, someTestRequest)
{
  // Setting the status code to OK (200).
  reply_.status = reply::status_type::ok;

  // Test request.
  std::string request = "test request";

  // Expected responding header.
  std::string expected = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n";
  // Adding the request.
  expected = expected + request;

  // Set request content.
  reply_.content = request;

  // Get the return vector from function call.
  std::vector<boost::asio::const_buffer> answer;
  answer = reply_.to_buffers();

  std::string result = "";
  std::cout << answer.size() << std::endl;
  for (int i = 0; i < answer.size(); i++)
  {
    result += buffer_to_string(answer, i);
  }

  bool success = (expected == result);

  EXPECT_TRUE(success);
}

// Test reply for regular formal request.
TEST_F(replyFixture, formalRequest)
{
  // Setting the status code to OK (200).
  reply_.status = reply::status_type::ok;

  // Test request.
  std::string request = "GET /docs/index.html HTTP/1.1\r\nHost: www.nowhere123.com\r\n";

  // Expected responding header.
  std::string expected = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n";
  // Add request.
  expected = expected + request;

  // Set request content.
  reply_.content = request;

  // Get the return vector from the function call.
  std::vector<boost::asio::const_buffer> answer;
  answer = reply_.to_buffers();

  std::string result = "";
  for (int i = 0; i < answer.size(); i++)
  {
    result += buffer_to_string(answer, i);
  }

  bool success = (expected == result);

  EXPECT_TRUE(success);
}

// Test reply for sample file request with not found status. 
TEST_F(replyFixture, someTestRequestWithNotFound)
{
  // Setting the status code to not_found (404).
  reply_.status = reply::status_type::not_found;

  // Test request.
  std::string request = "test request";

  // Expected responding header.
  std::string expected = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\n";
  // Adding the request.
  expected = expected + request;

  // Set request content.
  reply_.content = request;

  // Get the return vector from function call.
  std::vector<boost::asio::const_buffer> answer;
  answer = reply_.to_buffers();

  std::string result = "";
  for (int i = 0; i < answer.size(); i++)
  {
    result += buffer_to_string(answer, i);
  }

  bool success = (expected == result);

  EXPECT_TRUE(success);
}

// Test reply for empty file request with not found status.
TEST_F(replyFixture, emptyTestRequestWithNotFound)
{
  // Setting the status code to not_found (404).
  reply_.status = reply::status_type::not_found;

  // Test request.
  std::string request = "";

  // Expected responding header.
  std::string expected = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\n";
  // Adding the request.
  expected = expected + request;

  // Set request content.
  reply_.content = request;

  // Get the return vector from function call.
  std::vector<boost::asio::const_buffer> answer;
  answer = reply_.to_buffers();

  std::string result = "";
  for (int i = 0; i < answer.size(); i++)
  {
    result += buffer_to_string(answer, i);
  }

  bool success = (expected == result);

  EXPECT_TRUE(success);
}

// Test reply for unknown file request.
TEST_F(replyFixture, FormalRequestWithNotFound)
{
  // Setting the status code to not_found (404).
  reply_.status = reply::status_type::not_found;

  // Test request.
  std::string request = "GET /docs/index.html HTTP/1.1\r\nHost: www.nowhere123.com\r\n";

  // Expected responding header.
  std::string expected = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\n";
  // Adding the request.
  expected = expected + request;

  // Set request content.
  reply_.content = request;

  // Get the return vector from function call.
  std::vector<boost::asio::const_buffer> answer;
  answer = reply_.to_buffers();

  std::string result = "";
  for (int i = 0; i < answer.size(); i++)
  {
    result += buffer_to_string(answer, i);
  }

  bool success = (expected == result);

  EXPECT_TRUE(success);
}

// Test reply for a bad request (400).
TEST_F(replyFixture, formalBadRequest)
{
  // Setting the status code to bad request(400).
  reply_.status = reply::status_type::bad_request;

  // Test request.
  std::string request = "GET /docs/index.html HTTP/1.1\r\nHost: www.nowhere123.com\r\n";

  // Expected responding header.
  std::string expected = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\n\r\n";
  // Add request.
  expected = expected + request;

  // Set request content.
  reply_.content = request;

  // Get the return vector from the function call.
  std::vector<boost::asio::const_buffer> answer;
  answer = reply_.to_buffers();

  std::string result = "";
  for (int i = 0; i < answer.size(); i++)
  {
    result += buffer_to_string(answer, i);
  }

  bool success = (expected == result);

  EXPECT_TRUE(success);
}

// Test reply for an unkown request.
TEST_F(replyFixture, formalUnknownRequest)
{
  // Setting the status code to unknown request.
  reply_.status = reply::status_type::unknown;

  // Test request.
  std::string request = "GET /docs/index.html HTTP/1.1\r\nHost: www.nowhere123.com\r\n";

  // Expected responding header.
  std::string expected = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\n\r\n";
  // Add request.
  expected = expected + request;

  // Set request content.
  reply_.content = request;

  // Get the return vector from the function call.
  std::vector<boost::asio::const_buffer> answer;
  answer = reply_.to_buffers();

  std::string result = "";
  for (int i = 0; i < answer.size(); i++)
  {
    result += buffer_to_string(answer, i);
  }

  bool success = (expected == result);

  EXPECT_TRUE(success);
}

// Test stock reply against expected output for OK request.
TEST(replyTest, stockOkRequest)
{
  reply rep_ok = reply::stock_reply(reply::ok);

  bool success = (rep_ok.status == reply::ok &&
                  rep_ok.content == stock_replies::ok &&
                  rep_ok.headers[0].name == "Content-Length" &&
                  rep_ok.headers[0].value == std::to_string(rep_ok.content.size()) && 
                  rep_ok.headers[1].name == "Content-Type" &&
                  rep_ok.headers[1].value == "text/html");

  EXPECT_TRUE(success);
}

// Test stock reply against expected output for bad request.
TEST(replyTest, stockBadRequest)
{
  reply bad_rep = reply::stock_reply(reply::bad_request);

  bool success = (bad_rep.status == reply::bad_request &&
                  bad_rep.content == stock_replies::bad_request &&
                  bad_rep.headers[0].name == "Content-Length" &&
                  bad_rep.headers[0].value == std::to_string(bad_rep.content.size()) && 
                  bad_rep.headers[1].name == "Content-Type" &&
                  bad_rep.headers[1].value == "text/html");

  EXPECT_TRUE(success);
}

// Test stock reply against expected output for missing file request.
TEST(replyTest, stockNotFoundRequest)
{
  reply rep_missing = reply::stock_reply(reply::not_found);

  bool success = (rep_missing.status == reply::not_found &&
                  rep_missing.content == stock_replies::not_found &&
                  rep_missing.headers[0].name == "Content-Length" &&
                  rep_missing.headers[0].value == std::to_string(rep_missing.content.size()) && 
                  rep_missing.headers[1].name == "Content-Type" &&
                  rep_missing.headers[1].value == "text/html");

  EXPECT_TRUE(success);
}

// Test stock reply against expected output for default request.
TEST(replyTest, stockDefaultRequest)
{
  reply rep_ok = reply::stock_reply(reply::unknown);

  bool success = (rep_ok.status == reply::unknown &&
                  rep_ok.content == stock_replies::bad_request &&
                  rep_ok.headers[0].name == "Content-Length" &&
                  rep_ok.headers[0].value == std::to_string(rep_ok.content.size()) && 
                  rep_ok.headers[1].name == "Content-Type" &&
                  rep_ok.headers[1].value == "text/html");

  EXPECT_TRUE(success);
}