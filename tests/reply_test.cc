#include "gtest/gtest.h"
#include "reply.h"
#include <iostream>
#include <cstring>

class replyFixture : public ::testing::Test{
  protected:
    reply rep;
};


TEST_F(replyFixture, emptyRequest){
  // setting the status code to ok (200)
  rep.status = reply::status_type::ok;

  // test request
  std::string request = "";

  // expected responding header 
  std::string expected = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n";
  // add request to the header
  expected = expected + request;

  // get the return vector of boost::asio:const_buffer from the reply function call
  std::vector<boost::asio::const_buffer> answer;
  answer = rep.to_buffers(request, request.size());

  // get the data out from the vector, 1. cast it to unsign char* 2. cast it to char*
  // 3. convert it to string for compare 
  const unsigned char* unsigned_status_line = static_cast<const unsigned char*> (answer.at(0).data());
  const char* signed_status_line = (char*) unsigned_status_line;
  std::string str_status_line = std::string{signed_status_line};
  std::string result = str_status_line;

  const unsigned char* unsigned_type_line = static_cast<const unsigned char*> (answer.at(1).data());
  const char* signed_type_line = (char*) unsigned_type_line;
  std::string str_type_line = std::string{signed_type_line};
  result = result + str_type_line;

  const unsigned char* unsigned_crlf = static_cast<const unsigned char*> (answer.at(2).data());
  const char* signed_crlf = (char*) unsigned_crlf;
  std::string str_crlf = std::string{signed_crlf};
  result = result + str_crlf;

  const unsigned char* unsigned_request = static_cast<const unsigned char*> (answer.at(3).data());
  const char* signed_request = (char*) unsigned_request;
  std::string str_request = std::string{signed_request};
  result = result + str_request;

  bool success = (expected == result);

  EXPECT_TRUE(success);
}

TEST_F(replyFixture, someTestRequest){
  // setting the status code to ok (200)
  rep.status = reply::status_type::ok;

  // test request
  std::string request = "test request";

  // expected responding header
  std::string expected = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n";
  // adding the request
  expected = expected + request;

  // get the return vector from function call
  std::vector<boost::asio::const_buffer> answer;
  answer = rep.to_buffers(request, request.size());

  const unsigned char* unsigned_status_line = static_cast<const unsigned char*> (answer.at(0).data());
  const char* signed_status_line = (char*) unsigned_status_line;
  std::string str_status_line = std::string{signed_status_line};
  std::string result = str_status_line;

  const unsigned char* unsigned_type_line = static_cast<const unsigned char*> (answer.at(1).data());
  const char* signed_type_line = (char*) unsigned_type_line;
  std::string str_type_line = std::string{signed_type_line};
  result = result + str_type_line;

  const unsigned char* unsigned_crlf = static_cast<const unsigned char*> (answer.at(2).data());
  const char* signed_crlf = (char*) unsigned_crlf;
  std::string str_crlf = std::string{signed_crlf};
  result = result + str_crlf;

  const unsigned char* unsigned_request = static_cast<const unsigned char*> (answer.at(3).data());
  const char* signed_request = (char*) unsigned_request;
  std::string str_request = std::string{signed_request};
  result = result + str_request;

  bool success = (expected == result);

  EXPECT_TRUE(success);
}

TEST_F(replyFixture, formalRequest){
  // setting the status code to ok (200)
  rep.status = reply::status_type::ok;

  // test request
  std::string request = "GET /docs/index.html HTTP/1.1\r\nHost: www.nowhere123.com\r\n";

  // expected responding header
  std::string expected = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n";
  // add request
  expected = expected + request;

  // get the return vector from the function call
  std::vector<boost::asio::const_buffer> answer;
  answer = rep.to_buffers(request, request.size());

  const unsigned char* unsigned_status_line = static_cast<const unsigned char*> (answer.at(0).data());
  const char* signed_status_line = (char*) unsigned_status_line;
  std::string str_status_line = std::string{signed_status_line};
  std::string result = str_status_line;

  const unsigned char* unsigned_type_line = static_cast<const unsigned char*> (answer.at(1).data());
  const char* signed_type_line = (char*) unsigned_type_line;
  std::string str_type_line = std::string{signed_type_line};
  result = result + str_type_line;

  const unsigned char* unsigned_crlf = static_cast<const unsigned char*> (answer.at(2).data());
  const char* signed_crlf = (char*) unsigned_crlf;
  std::string str_crlf = std::string{signed_crlf};
  result = result + str_crlf;

  const unsigned char* unsigned_request = static_cast<const unsigned char*> (answer.at(3).data());
  const char* signed_request = (char*) unsigned_request;
  std::string str_request = std::string{signed_request};
  result = result + str_request;

  bool success = (expected == result);

  EXPECT_TRUE(success);
}