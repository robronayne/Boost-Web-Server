#include "gtest/gtest.h"
#include "request_parser.h"
#include "request.h"

#include <cstring>
#include <boost/asio.hpp>

class requestParserFixture : public ::testing::Test
{
  protected:
    request_parser p;
    request r;
    boost::asio::streambuf b;
    request_parser::result_type result;
};

// Test for normal request
TEST_F(requestParserFixture, normalRequest)
{
  // set test input
  std::ostream os(&b);
  std::string example = "GET /docs/index.html HTTP/1.1\r\nHost: www.nowhere123.com"
                        "\r\n .test\r\nContent-Type: plain/text\r\n\r\n";
  os << example;

  // parse input
  std::istreambuf_iterator<char> st{&b}, end;
  std::tie(result, std::ignore) = p.parse(
    r, st, end);

  // check request struct correctness
  bool success = (r.original_req == "GET /docs/index.html HTTP/1.1\r\nHost: www.nowhere123.com"
                                    "\r\n .test\r\nContent-Type: plain/text\r\n\r\n" &&
                  r.method == "GET" &&
                  r.uri == "/docs/index.html" &&
                  r.http_version_major == 1 &&
                  r.http_version_minor == 1 &&
                  r.headers[0].name == "Host" &&
                  r.headers[0].value == "www.nowhere123.com.test" &&
                  r.headers[1].name == "Content-Type" &&
                  r.headers[1].value == "plain/text" &&
                  result == request_parser::good);

  EXPECT_TRUE(success);
}

// Test for incomplete request
TEST_F(requestParserFixture, indeterminateRequest)
{
  // reset parser state
  p.reset();

  // set test input
  std::ostream os(&b);
  std::string example = "GET";
  os << example;

  // parse input
  std::istreambuf_iterator<char> st{&b}, end;
  std::tie(result, std::ignore) = p.parse(
    r, st, end);

  // check request struct correctness
  bool success = (r.original_req == example &&
                  result == request_parser::indeterminate);

  EXPECT_TRUE(success);
}

// Test for no method in header
TEST_F(requestParserFixture, emptyRequest)
{
  // reset parser state
  p.reset();

  // set test input
  std::ostream os(&b);
  std::string example = "(\r\n\r\n";
  os << example;

  // parse input
  std::istreambuf_iterator<char> st{&b}, end;
  std::tie(result, std::ignore) = p.parse(
    r, st, end);

  // check request struct correctness
  bool success = (r.original_req == "(" &&
                  result == request_parser::bad);

  EXPECT_TRUE(success);
}

// Test for no uri in header
TEST_F(requestParserFixture, onlyMethodRequest)
{
  // reset parser state
  p.reset();

  // set test input
  std::ostream os(&b);
  std::string example = "GET\r\n\r\n";
  os << example;

  // parse input
  std::istreambuf_iterator<char> st{&b}, end;
  std::tie(result, std::ignore) = p.parse(
    r, st, end);

  // check request struct correctness
  bool success = (r.method == "GET" &&
                  result == request_parser::bad);

  EXPECT_TRUE(success);
}

// Test for no HTTP information in header
TEST_F(requestParserFixture, onlyMethodURIRequest)
{
  // reset parser state
  p.reset();

  // set test input
  std::ostream os(&b);
  std::string example = "GET /\r\n\r\n";
  os << example;

  // parse input
  std::istreambuf_iterator<char> st{&b}, end;
  std::tie(result, std::ignore) = p.parse(
    r, st, end);

  // check request struct correctness
  bool success = (r.method == "GET" &&
                  r.uri == "/" &&
                  result == request_parser::bad);

  EXPECT_TRUE(success);
}

// Test for partial HTTP information in header
TEST_F(requestParserFixture, partialHTTPRequest)
{
  bool success = true;
  std::string inputs[] = {"", "H", "HT", "HTT", "HTTP",
                          "HTTP/", "HTTP/1", "HTTP/1."};
  
  for (int i = 0; i < 8; i++)
  {
    // reset parser state
    p.reset();
    boost::asio::streambuf v;

    // set test input
    std::ostream os(&v);
    std::string example = "GET / " + inputs[i] + "\r\n\r\n";
    os << example;

    // parse input
    std::istreambuf_iterator<char> st{&v}, end;
    std::tie(result, std::ignore) = p.parse(
        r, st, end);

    // check request struct correctness
    success = (r.method == "GET" &&
               r.uri == "/" &&
               result == request_parser::bad &&
               success);

    r.method.clear();
    r.uri.clear();
  }

  EXPECT_TRUE(success);
}

// Test for incorrectly terminated request header
TEST_F(requestParserFixture, invalidTerminationRequest)
{ 
  // reset parser state
  p.reset();

  // set test input
  std::ostream os(&b);
  std::string example = "GET / HTTP/1.1\t\n";
  os << example;

  // parse input
  std::istreambuf_iterator<char> st{&b}, end;
  std::tie(result, std::ignore) = p.parse(
    r, st, end);

  // check request struct correctness
  bool success = (r.method == "GET" &&
                  r.uri == "/" &&
                  r.http_version_major == 1 &&
                  r.http_version_minor == 1 &&
                  result == request_parser::bad);

  EXPECT_TRUE(success);
}

// Test for more than single digits in http major version
TEST_F(requestParserFixture, longHTTPMajorVersionRequest)
{ 
  // reset parser state
  p.reset();

  // set test input
  std::ostream os(&b);
  std::string example = "GET / HTTP/10.1\r\n\r\n";
  os << example;

  // parse input
  std::istreambuf_iterator<char> st{&b}, end;
  std::tie(result, std::ignore) = p.parse(
      r, st, end);

  // check request struct correctness
  bool success = (r.method == "GET" &&
                    r.uri == "/" &&
                    r.http_version_major == 10 &&
                    result == request_parser::good);

  EXPECT_TRUE(success);
}

// Test for more than single digits in http minor version
TEST_F(requestParserFixture, longHTTPMinorVersionRequest)
{
  // reset parser state
  p.reset();

  // set test input
  std::ostream os(&b);
  std::string example = "GET / HTTP/1.01\r\n\r\n";
  os << example;

  // parse input
  std::istreambuf_iterator<char> st{&b}, end;
  std::tie(result, std::ignore) = p.parse(
      r, st, end);

  // check request struct correctness
  bool success = (r.method == "GET" &&
                  r.uri == "/" &&
                  r.http_version_major == 1 &&
                  r.http_version_minor == 1 &&
                  result == request_parser::good);

  EXPECT_TRUE(success);
}

// Test for second constructor
/**TEST(echoHandlerTest, normalRequest)
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
}**/