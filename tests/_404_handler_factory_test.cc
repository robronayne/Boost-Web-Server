#include <cstring>
#include <boost/asio/buffers_iterator.hpp>

#include "gtest/gtest.h"
#include "request_handler_factory/_404_handler_factory.h"

// Test if the 404 handler factory successfully created an 404 hander (not nullptr)
TEST(_404HandlerFactoryTest, successfullyCreate404Handler)
{
  // create factory pointer
  path p;
  p.info_map["root"] = " ";
  _404_handler_factory* factory = new _404_handler_factory("/", p);

  // test user profile
  user_profile profile;

  // create handler pointer
  request_handler_interface* handler = factory->create("/", " ", profile);

  // check if handler is nullptr
  bool success = (handler != nullptr);

  EXPECT_TRUE(success);
}

// Test if the created 404 handler work as expected
TEST(_404HandlerFactoryTest, createUsable404Handler)
{
  // create factory pointer
  path p;
  p.info_map["root"] = " ";
  _404_handler_factory* factory = new _404_handler_factory("/", p);

  // test user profile
  user_profile profile;

  // create handler pointer
  request_handler_interface* handler = factory->create("/", " ", profile);

  // Sample request to test.
  http::request<http::dynamic_body> request_;  

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
  
  util utility;
  std::string expected_body = utility.get_stock_reply(404);

  // Check reply struct correctness.
  bool success = (answer.result() == http::status::not_found &&
                  body == expected_body &&
                  answer.has_content_length() &&
                  headers.at(0).first == "Content-Length" &&
                  headers.at(0).second == std::to_string(expected_body.size()) &&
                  headers.at(1).first == "Content-Type" &&
                  headers.at(1).second == "text/html" &&
                  status_ == answer.result());

  EXPECT_TRUE(success);
}