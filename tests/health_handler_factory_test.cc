#include <cstring>
#include <boost/asio/buffers_iterator.hpp>

#include "gtest/gtest.h"
#include "request_handler_factory/health_handler_factory.h"

// Test if the health handler factory successfully created an health hander (not nullptr)
TEST(healthHandlerFactoryTest, successfullyCreateHealthHandler)
{
  // create factory pointer
  path p;
  health_handler_factory* factory = new health_handler_factory("/health", p);
  // create handler pointer
  request_handler_interface* handler = factory->create("/health", "test url");

  // check if handler is nullptr
  bool success = (handler != nullptr);

  EXPECT_TRUE(success);
}

// Test if the created health handler work as expected
TEST(healthHandlerFactoryTest, createUsableHealthHandler)
{
  // create factory pointer
  path p;
  health_handler_factory* factory = new health_handler_factory("/health", p);
  // create handler pointer
  request_handler_interface* handler = factory->create("/health", "test url");

  // Sample request to test.
  http::request<http::dynamic_body> request_;
  request_.target("/health");

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
                  body == "OK" &&
                  answer.has_content_length() &&
                  headers.at(1).first == "Content-Type" &&
                  headers.at(1).second == "text/plain" &&
                  status_ == answer.result());

  EXPECT_TRUE(success);
}