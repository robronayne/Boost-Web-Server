#include "gtest/gtest.h"
#include "reply.h"
#include <iostream>
#include <cstring>

class replyFixture : public ::testing::Test{
  protected:
    reply rep;
  
  void SetUp() override
  {
    rep.headers.resize(1);
    rep.headers[0].name = "Content-Type";
    rep.headers[0].value = "text/plain";
  }

  // Helper function to convert const buffers to strings
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


TEST_F(replyFixture, emptyRequest){
  // setting the status code to ok (200)
  rep.status = reply::status_type::ok;

  // test request
  std::string request = "";

  // expected responding header 
  std::string expected = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n";
  // add request to the header
  expected = expected + request;

  // set request content
  rep.content = request;

  // get the return vector of boost::asio:const_buffer from the reply function call
  std::vector<boost::asio::const_buffer> answer;
  answer = rep.to_buffers();

  // get the data out from the vector, 1. cast it to unsign char* 2. cast it to char*
  // 3. convert it to string for compare 
  std::string result = "";
  for (int i = 0; i < answer.size(); i++)
  {
    result += buffer_to_string(answer, i);
  }
  
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

  // set request content
  rep.content = request;

  // get the return vector from function call
  std::vector<boost::asio::const_buffer> answer;
  answer = rep.to_buffers();

  std::string result = "";
  std::cout << answer.size() << std::endl;
  for (int i = 0; i < answer.size(); i++)
  {
    result += buffer_to_string(answer, i);
  }

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

  // set request content
  rep.content = request;

  // get the return vector from the function call
  std::vector<boost::asio::const_buffer> answer;
  answer = rep.to_buffers();

  std::string result = "";
  for (int i = 0; i < answer.size(); i++)
  {
    result += buffer_to_string(answer, i);
  }

  bool success = (expected == result);

  EXPECT_TRUE(success);
}

TEST_F(replyFixture, someTestRequestWithNotFound){
  // setting the status code to not_found (404)
  rep.status = reply::status_type::not_found;

  // test request
  std::string request = "test request";

  // expected responding header
  std::string expected = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\n";
  // adding the request
  expected = expected + request;

  // set request content
  rep.content = request;

  // get the return vector from function call
  std::vector<boost::asio::const_buffer> answer;
  answer = rep.to_buffers();

  std::string result = "";
  for (int i = 0; i < answer.size(); i++)
  {
    result += buffer_to_string(answer, i);
  }

  bool success = (expected == result);

  EXPECT_TRUE(success);
}

TEST_F(replyFixture, emptyTestRequestWithNotFound){
  // setting the status code to not_found (404)
  rep.status = reply::status_type::not_found;

  // test request
  std::string request = "";

  // expected responding header
  std::string expected = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\n";
  // adding the request
  expected = expected + request;

  // set request content
  rep.content = request;

  // get the return vector from function call
  std::vector<boost::asio::const_buffer> answer;
  answer = rep.to_buffers();

  std::string result = "";
  for (int i = 0; i < answer.size(); i++)
  {
    result += buffer_to_string(answer, i);
  }

  bool success = (expected == result);

  EXPECT_TRUE(success);
}

TEST_F(replyFixture, FormalRequestWithNotFound){
  // setting the status code to not_found (404)
  rep.status = reply::status_type::not_found;

  // test request
  std::string request = "GET /docs/index.html HTTP/1.1\r\nHost: www.nowhere123.com\r\n";

  // expected responding header
  std::string expected = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\n";
  // adding the request
  expected = expected + request;

  // set request content
  rep.content = request;

  // get the return vector from function call
  std::vector<boost::asio::const_buffer> answer;
  answer = rep.to_buffers();

  std::string result = "";
  for (int i = 0; i < answer.size(); i++)
  {
    result += buffer_to_string(answer, i);
  }

  bool success = (expected == result);

  EXPECT_TRUE(success);
}

TEST_F(replyFixture, formalBadRequest){
  // as the default reponse is set to ok for now, the test result should be the same as ok(200)

  // setting the status code to bad request(400)
  rep.status = reply::status_type::bad_request;

  // test request
  std::string request = "GET /docs/index.html HTTP/1.1\r\nHost: www.nowhere123.com\r\n";

  // expected responding header
  std::string expected = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\n\r\n";
  // add request
  expected = expected + request;

  // set request content
  rep.content = request;

  // get the return vector from the function call
  std::vector<boost::asio::const_buffer> answer;
  answer = rep.to_buffers();

  std::string result = "";
  for (int i = 0; i < answer.size(); i++)
  {
    result += buffer_to_string(answer, i);
  }

  bool success = (expected == result);

  EXPECT_TRUE(success);
}

TEST_F(replyFixture, formalUnknownRequest){
  // as the default reponse is set to ok for now, the test result should be the same as ok(200)

  // setting the status code to bad request(400)
  rep.status = reply::status_type::unknown;

  // test request
  std::string request = "GET /docs/index.html HTTP/1.1\r\nHost: www.nowhere123.com\r\n";

  // expected responding header
  std::string expected = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\n\r\n";
  // add request
  expected = expected + request;

  // set request content
  rep.content = request;

  // get the return vector from the function call
  std::vector<boost::asio::const_buffer> answer;
  answer = rep.to_buffers();

  std::string result = "";
  for (int i = 0; i < answer.size(); i++)
  {
    result += buffer_to_string(answer, i);
  }

  bool success = (expected == result);

  EXPECT_TRUE(success);
}

TEST(replyTest, stockOkRequest){
  reply rep_ok = reply::stock_reply(reply::ok);

  bool success = (rep_ok.status == reply::ok &&
                  rep_ok.content == stock_replies::ok &&
                  rep_ok.headers[0].name == "Content-Length" &&
                  rep_ok.headers[0].value == std::to_string(rep_ok.content.size()) && 
                  rep_ok.headers[1].name == "Content-Type" &&
                  rep_ok.headers[1].value == "text/html");

  EXPECT_TRUE(success);
}

TEST(replyTest, stockBadRequest){
  reply rep_ok = reply::stock_reply(reply::bad_request);

  bool success = (rep_ok.status == reply::bad_request &&
                  rep_ok.content == stock_replies::bad_request &&
                  rep_ok.headers[0].name == "Content-Length" &&
                  rep_ok.headers[0].value == std::to_string(rep_ok.content.size()) && 
                  rep_ok.headers[1].name == "Content-Type" &&
                  rep_ok.headers[1].value == "text/html");

  EXPECT_TRUE(success);
}

TEST(replyTest, stockNotFoundRequest){
  reply rep_ok = reply::stock_reply(reply::not_found);

  bool success = (rep_ok.status == reply::not_found &&
                  rep_ok.content == stock_replies::not_found &&
                  rep_ok.headers[0].name == "Content-Length" &&
                  rep_ok.headers[0].value == std::to_string(rep_ok.content.size()) && 
                  rep_ok.headers[1].name == "Content-Type" &&
                  rep_ok.headers[1].value == "text/html");

  EXPECT_TRUE(success);
}

TEST(replyTest, stockDefaultRequest){
  reply rep_ok = reply::stock_reply(reply::unknown);

  bool success = (rep_ok.status == reply::unknown &&
                  rep_ok.content == stock_replies::bad_request &&
                  rep_ok.headers[0].name == "Content-Length" &&
                  rep_ok.headers[0].value == std::to_string(rep_ok.content.size()) && 
                  rep_ok.headers[1].name == "Content-Type" &&
                  rep_ok.headers[1].value == "text/html");

  EXPECT_TRUE(success);
}