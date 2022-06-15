#include <cstring>
#include <boost/asio.hpp>

#include "http/request.h"
#include "gtest/gtest.h"
#include "request_parser.h"

class requestParserFixture : public ::testing::Test
{
  protected:
    request_parser parser_;
    request request_;
    boost::asio::streambuf buf;
    request_parser::result_type result;
};

// Test for normal request.
TEST_F(requestParserFixture, normalRequest)
{
  // Set test input.
  std::ostream os(&buf);
  std::string example = "GET /docs/index.html HTTP/1.1\r\nHost: www.nowhere123.com"
                        "\r\n .test\r\nContent-Type: plain/text\r\n\r\n";
  os << example;

  // Parse input.
  std::istreambuf_iterator<char> st{&buf}, end;
  std::tie(result, std::ignore) = parser_.parse(
    request_, st, end);

  // Check request struct correctness.
  bool success = (request_.original_req == "GET /docs/index.html HTTP/1.1\r\nHost: www.nowhere123.com"
                                           "\r\n .test\r\nContent-Type: plain/text\r\n\r\n" &&
                  request_.method == "GET" &&
                  request_.uri == "/docs/index.html" &&
                  request_.http_version_major == 1 &&
                  request_.http_version_minor == 1 &&
                  request_.headers[0].name == "Host" &&
                  request_.headers[0].value == "www.nowhere123.com.test" &&
                  request_.headers[1].name == "Content-Type" &&
                  request_.headers[1].value == "plain/text" &&
                  result == request_parser::good);

  EXPECT_TRUE(success);
}

// Test for incomplete request.
TEST_F(requestParserFixture, indeterminateRequest)
{
  // Reset parser state.
  parser_.reset();

  // Set test input.
  std::ostream os(&buf);
  std::string example = "GET";
  os << example;

  // Parse input.
  std::istreambuf_iterator<char> st{&buf}, end;
  std::tie(result, std::ignore) = parser_.parse(
    request_, st, end);

  // Check request struct correctness.
  bool success = (request_.original_req == example &&
                  result == request_parser::indeterminate);

  EXPECT_TRUE(success);
}

// Test for no method in header.
TEST_F(requestParserFixture, emptyRequest)
{
  // Reset parser state.
  parser_.reset();

  // Set test input.
  std::ostream os(&buf);
  std::string example = "(\r\n\r\n";
  os << example;

  // Parse input.
  std::istreambuf_iterator<char> st{&buf}, end;
  std::tie(result, std::ignore) = parser_.parse(
    request_, st, end);

  // Check request struct correctness.
  bool success = (request_.original_req == "(" &&
                  result == request_parser::bad);

  EXPECT_TRUE(success);
}

// Test for no URI in header.
TEST_F(requestParserFixture, onlyMethodRequest)
{
  // Reset parser state.
  parser_.reset();

  // Set test input.
  std::ostream os(&buf);
  std::string example = "GET\r\n\r\n";
  os << example;

  // Parse input.
  std::istreambuf_iterator<char> st{&buf}, end;
  std::tie(result, std::ignore) = parser_.parse(
    request_, st, end);

  // Check request struct correctness.
  bool success = (request_.method == "GET" &&
                  result == request_parser::bad);

  EXPECT_TRUE(success);
}

// Test for no HTTP information in header.
TEST_F(requestParserFixture, onlyMethodURIRequest)
{
  // Reset parser state.
  parser_.reset();

  // Set test input.
  std::ostream os(&buf);
  std::string example = "GET /\r\n\r\n";
  os << example;

  // Parse input.
  std::istreambuf_iterator<char> st{&buf}, end;
  std::tie(result, std::ignore) = parser_.parse(
    request_, st, end);

  // Check request struct correctness.
  bool success = (request_.method == "GET" &&
                  request_.uri == "/" &&
                  result == request_parser::bad);

  EXPECT_TRUE(success);
}

// Test for partial HTTP information in header.
TEST_F(requestParserFixture, partialHTTPRequest)
{
  bool success = true;
  std::string inputs[] = {"", "H", "HT", "HTT", "HTTP",
                          "HTTP/", "HTTP/1", "HTTP/1."};
  
  for (int i = 0; i < 8; i++)
  {
    // Reset parser state.
    parser_.reset();
    boost::asio::streambuf v;

    // Set test input.
    std::ostream os(&v);
    std::string example = "GET / " + inputs[i] + "\r\n\r\n";
    os << example;

    // Parse input.
    std::istreambuf_iterator<char> st{&v}, end;
    std::tie(result, std::ignore) = parser_.parse(
      request_, st, end);

    // Check request struct correctness.
    success = (request_.method == "GET" &&
               request_.uri == "/" &&
               result == request_parser::bad &&
               success);

    request_.method.clear();
    request_.uri.clear();
  }

  EXPECT_TRUE(success);
}

// Test for incorrectly terminated request header.
TEST_F(requestParserFixture, invalidTerminationRequest)
{ 
  // Reset parser state.
  parser_.reset();

  // Set test input.
  std::ostream os(&buf);
  std::string example = "GET / HTTP/1.1\t\n";
  os << example;

  // Parse input.
  std::istreambuf_iterator<char> st{&buf}, end;
  std::tie(result, std::ignore) = parser_.parse(
    request_, st, end);

  // Check request struct correctness.
  bool success = (request_.method == "GET" &&
                  request_.uri == "/" &&
                  request_.http_version_major == 1 &&
                  request_.http_version_minor == 1 &&
                  result == request_parser::bad);

  EXPECT_TRUE(success);
}

// Test for more than single digits in HTTP major version.
TEST_F(requestParserFixture, longHTTPMajorVersionRequest)
{ 
  // Reset parser state.
  parser_.reset();

  // Set test input.
  std::ostream os(&buf);
  std::string example = "GET / HTTP/10.1\r\n\r\n";
  os << example;

  // Parse input.
  std::istreambuf_iterator<char> st{&buf}, end;
  std::tie(result, std::ignore) = parser_.parse(
      request_, st, end);

  // Check request struct correctness.
  bool success = (request_.method == "GET" &&
                    request_.uri == "/" &&
                    request_.http_version_major == 10 &&
                    result == request_parser::good);

  EXPECT_TRUE(success);
}

// Test for more than single digits in HTTP minor version.
TEST_F(requestParserFixture, longHTTPMinorVersionRequest)
{
  // Reset parser state.
  parser_.reset();

  // Set test input.
  std::ostream os(&buf);
  std::string example = "GET / HTTP/1.01\r\n\r\n";
  os << example;

  // Parse input.
  std::istreambuf_iterator<char> st{&buf}, end;
  std::tie(result, std::ignore) = parser_.parse(
      request_, st, end);

  // Check request struct correctness.
  bool success = (request_.method == "GET" &&
                  request_.uri == "/" &&
                  request_.http_version_major == 1 &&
                  request_.http_version_minor == 1 &&
                  result == request_parser::good);

  EXPECT_TRUE(success);
}

/*
 * Test for second constructor
 *
  TEST(echoHandlerTest, normalRequest)
  {
    // test request
    std::string request = "test request";

    // get the return reply struct from the handler function call
    reply answer;
    echo_handler handler(request, request.size());
    answer = handler.get_reply();

    // check reply struct correctness
    bool success = (answer.status == reply::ok &&
                    answer.content == request &&
                    answer.headers[0].name == "Content-Length" &&
                    answer.headers[0].value == std::to_string(answer.content.size()) && 
                    answer.headers[1].name == "Content-Type" &&
                    answer.headers[1].value == "text/plain");

    EXPECT_TRUE(success);
  }
 *
 */