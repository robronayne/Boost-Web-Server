#include <cstring>
#include <boost/asio/buffers_iterator.hpp>

#include "gtest/gtest.h"
#include "request_handler_factory/echo_handler_factory.h"

// Test if the echo handler factory successfully created an echo hander (not nullptr)
TEST(echoHandlerFactoryTest, successfullyCreateEchoHandler)
{
  // create factory pointer
  path p;
  p.info_map["root"] = "test path";
  echo_handler_factory* factory = new echo_handler_factory("/echo", p);

  // test user profile
  user_profile profile;

  // create handler pointer
  request_handler_interface* handler = factory->create("/echo", "test url", profile);

  // check if handler is nullptr
  bool success = (handler != nullptr);

  EXPECT_TRUE(success);
}

// Test if the created echo handler work as expected
TEST(echoHandlerFactoryTest, createUsableEchoHandler)
{
  // create factory pointer
  path p;
  p.info_map["root"] = "test path";
  echo_handler_factory* factory = new echo_handler_factory("/echo", p);

  // test user profile
  user_profile profile;

  // create handler pointer
  request_handler_interface* handler = factory->create("/echo", "test url", profile);

  // Sample request to test.
  http::request<http::dynamic_body> request_;
  request_.target("/echo");
  beast::ostream(request_.body()) << "test request";

  std::ostringstream ostring;
  ostring << request_;
  std::string request_string = ostring.str();

  // Get the return reply struct from the handler function call.
  http::response<http::dynamic_body> answer;
  http::status status_ = handler->serve(request_, answer);

  std::string body { boost::asio::buffers_begin(answer.body().data()),
                     boost::asio::buffers_end(answer.body().data()) };

  std::vector<std::pair<std::string, std::string>> headers;
  for(auto const& field : answer)
  {
    std::pair<std::string, std::string> header;
    header.first = std::string(field.name_string());
    header.second = std::string(field.value());
    headers.push_back(header);
  }

  // Check reply struct correctness.
  bool success = (answer.result() == http::status::ok &&
                  body == request_string &&
                  answer.has_content_length() &&
                  headers.at(1).first == "Content-Type" &&
                  headers.at(1).second == "text/plain" &&
                  status_ == answer.result());

  EXPECT_TRUE(success);
}