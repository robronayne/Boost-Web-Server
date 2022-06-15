#include <cstring>
#include <boost/asio/buffers_iterator.hpp>
#include "gtest/gtest.h"
#include "request_handler_factory/sleep_handler_factory.h"
// Test if the sleep handler factory successfully created an sleep hander (not nullptr)
TEST(sleepHandlerFactoryTest, successfullyCreateSleepHandler)
{
  // create factory pointer
  path p;
  sleep_handler_factory* factory = new sleep_handler_factory("/sleep", p);

  // create handler pointer
  request_handler_interface* handler = factory->create("/sleep", "test url");

  // check if handler is nullptr
  bool success = (handler != nullptr);
  EXPECT_TRUE(success);
}
// Test if the created sleep handler work as expected
TEST(sleepHandlerFactoryTest, createUsableSleepHandler)
{
  // create factory pointer
  path p;
  sleep_handler_factory* factory = new sleep_handler_factory("/sleep", p);

  // create handler pointer
  request_handler_interface* handler = factory->create("/sleep", "test url");
  
  // Sample request to test.
  http::request<http::dynamic_body> request_;
  request_.target("/sleep");

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
                  body == "This request slept for " + std::to_string(SLEEPY_TIME) + " microseconds" &&
                  answer.has_content_length() &&
                  headers.at(1).first == "Content-Type" &&
                  headers.at(1).second == "text/plain" &&
                  status_ == answer.result());
  EXPECT_TRUE(success);
}
