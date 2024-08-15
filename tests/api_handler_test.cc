#include "request_handler/api_handler.h"
#include "gtest/gtest.h"
#include "entity_manager.h"
#include "entity.h"
#include <string>
#include <boost/filesystem.hpp>
#include <boost/asio/buffers_iterator.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <unordered_map>
#include <unordered_set>
#include <set>

class ApiHandlerTestFixture : public  ::testing::Test
{  
    protected:
        std::string location = "/api";
        std::string root = "../database";
        http::request<http::dynamic_body> request_;
        http::response<http::dynamic_body> response_;
        entity_manager e_manager;    
};

TEST_F(ApiHandlerTestFixture, Create) {
    std::string request_url = "/api/test";
    api_handler handler(location, root, request_url, &e_manager);
    request_.method(http::verb::post);
    request_.target(request_url);

    http::status status_ = handler.serve(request_, response_);

    std::string body { boost::asio::buffers_begin(response_.body().data()),
                        boost::asio::buffers_end(response_.body().data()) };

    std::vector<std::pair<std::string, std::string>> headers;
    for(auto const& field : response_)
    {
        std::pair<std::string, std::string> header;
        header.first = std::string(field.name_string());
        header.second = std::string(field.value());
        headers.push_back(header);
    }

    // Check reply struct correctness.
    bool success = (status_ == http::status::ok &&
                    headers.at(0).first == "Content-Length" &&
                    headers.at(0).second == "9" &&
                    headers.at(1).first == "Content-Type" &&
                    headers.at(1).second == "application/json");

    EXPECT_TRUE(success);
}

TEST_F(ApiHandlerTestFixture, List) {
    std::string request_url = "/api/test";
    api_handler handler(location, root, request_url, &e_manager);
    request_.method(http::verb::post);
    request_.target(request_url);

    http::status status_ = handler.serve(request_, response_);

    request_.method(http::verb::get);
    request_.target(request_url);
    status_ = handler.serve(request_, response_);

    std::vector<std::pair<std::string, std::string>> headers;
    for(auto const& field : response_)
    {
        std::pair<std::string, std::string> header;
        header.first = std::string(field.name_string());
        header.second = std::string(field.value());
        headers.push_back(header);
    }

    // Check reply struct correctness.
    bool success = (status_ == http::status::ok &&
                    headers.at(1).first == "Content-Type" &&
                    headers.at(1).second == "application/json"
                    );

    EXPECT_TRUE(success);
}


TEST_F(ApiHandlerTestFixture, Retrieve) {
    std::string request_url = "/api/test";
    api_handler handler(location, root, request_url, &e_manager);
    request_.method(http::verb::post);
    boost::beast::ostream(request_.body()) << "{\"name\": \"jklm\"}";
    request_.target(request_url);

    http::status status_ = handler.serve(request_, response_);

    request_.method(http::verb::get);
    request_.target(request_url + "/1");
    status_ = handler.serve(request_, response_);

    std::vector<std::pair<std::string, std::string>> headers;
    for(auto const& field : response_)
    {
        std::pair<std::string, std::string> header;
        header.first = std::string(field.name_string());
        header.second = std::string(field.value());
        headers.push_back(header);
    }

    // Check reply struct correctness.
    bool success = (status_ == http::status::ok &&
                    headers.at(1).first == "Content-Type" &&
                    headers.at(1).second == "application/json"
                    );

    EXPECT_TRUE(success);
}

TEST_F(ApiHandlerTestFixture, Update) {
    std::string request_url = "/api/test";
    api_handler handler(location, root, request_url, &e_manager);
    request_.method(http::verb::post);
    boost::beast::ostream(request_.body()) << "{\"name\": \"jklm\"}";
    request_.target(request_url);

    http::status status_ = handler.serve(request_, response_);

    request_.method(http::verb::put);
    request_.target(request_url + "/1");
    boost::beast::ostream(request_.body()) << "{\"name\": \"jklm2\"}";
    status_ = handler.serve(request_, response_);

    std::vector<std::pair<std::string, std::string>> headers;
    for(auto const& field : response_)
    {
        std::pair<std::string, std::string> header;
        header.first = std::string(field.name_string());
        header.second = std::string(field.value());
        headers.push_back(header);
    }

    // Check reply struct correctness.
    /*
    If the response type is plain text, we know the operation succeeded
    Otherwise, the server would have sent html
    */
    bool success = (status_ == http::status::ok &&
                    headers.at(1).first == "Content-Type" &&
                    headers.at(1).second == "text/plain"
                    );

    EXPECT_TRUE(success);
}

TEST_F(ApiHandlerTestFixture, Delete) {
    std::string request_url = "/api/test";
    api_handler handler(location, root, request_url, &e_manager);
    request_.method(http::verb::post);
    boost::beast::ostream(request_.body()) << "{\"name\": \"jklm\"}";
    request_.target(request_url);

    http::status status_ = handler.serve(request_, response_);

    request_.method(http::verb::delete_);
    request_.target(request_url + "/1");
    status_ = handler.serve(request_, response_);

    std::vector<std::pair<std::string, std::string>> headers;
    for(auto const& field : response_)
    {
        std::pair<std::string, std::string> header;
        header.first = std::string(field.name_string());
        header.second = std::string(field.value());
        headers.push_back(header);
    }

    // Check reply struct correctness.
    /*
    If the response type is plain text, we know the operation succeeded
    Otherwise, the server would have sent html
    */
    bool success = (status_ == http::status::ok &&
                    headers.at(1).first == "Content-Type" &&
                    headers.at(1).second == "text/plain"
                    );

    EXPECT_TRUE(success);
}


TEST_F(ApiHandlerTestFixture, RetrieveNotFound) {
    std::string request_url = "/api/test/11";
    api_handler handler(location, root, request_url, &e_manager);
    request_.method(http::verb::get);
    request_.target(request_url);

    http::status status_ = handler.serve(request_, response_);

    std::vector<std::pair<std::string, std::string>> headers;
    for(auto const& field : response_)
    {
        std::pair<std::string, std::string> header;
        header.first = std::string(field.name_string());
        header.second = std::string(field.value());
        headers.push_back(header);
    }

    // Check reply struct correctness.
    bool success = (status_ == http::status::bad_request &&
                    headers.at(0).first == "Content-Length" &&
                    headers.at(0).second == "89" &&
                    headers.at(1).first == "Content-Type" &&
                    headers.at(1).second == "text/html"
                    );

    EXPECT_TRUE(success);
}

TEST_F(ApiHandlerTestFixture, UpdateNotFound) {
    std::string request_url = "/api/test/11";
    api_handler handler(location, root, request_url, &e_manager);
    request_.method(http::verb::put);
    request_.target(request_url);

    http::status status_ = handler.serve(request_, response_);

    std::vector<std::pair<std::string, std::string>> headers;
    for(auto const& field : response_)
    {
        std::pair<std::string, std::string> header;
        header.first = std::string(field.name_string());
        header.second = std::string(field.value());
        headers.push_back(header);
    }

    // Check reply struct correctness.
    bool success = (status_ == http::status::ok &&
                    headers.at(1).first == "Content-Type" &&
                    headers.at(1).second == "application/json");

    EXPECT_TRUE(success);
}

TEST_F(ApiHandlerTestFixture, DeleteNotFound) {
    std::string request_url = "/api/test/1111";
    api_handler handler(location, root, request_url, &e_manager);
    request_.method(http::verb::delete_);
    request_.target(request_url);

    http::status status_ = handler.serve(request_, response_);

    std::vector<std::pair<std::string, std::string>> headers;
    for(auto const& field : response_)
    {
        std::pair<std::string, std::string> header;
        header.first = std::string(field.name_string());
        header.second = std::string(field.value());
        headers.push_back(header);
    }

    // Check reply struct correctness.
    bool success = (status_ == http::status::bad_request &&
                    headers.at(1).first == "Content-Type" &&
                    headers.at(1).second == "text/html"
                    );

    EXPECT_TRUE(success);
}

TEST_F(ApiHandlerTestFixture, InvalidVerb) {
    std::string request_url = "/api/test/11";
    api_handler handler(location, root, request_url, &e_manager);
    request_.method_string("lll");
    request_.target(request_url);

    http::status status_ = handler.serve(request_, response_);

    std::vector<std::pair<std::string, std::string>> headers;
    for(auto const& field : response_)
    {
        std::pair<std::string, std::string> header;
        header.first = std::string(field.name_string());
        header.second = std::string(field.value());
        headers.push_back(header);
    }

    // Check reply struct correctness.
    bool success = (status_ == http::status::bad_request &&
                    headers.at(0).first == "Content-Length" &&
                    headers.at(0).second == "89" &&
                    headers.at(1).first == "Content-Type" &&
                    headers.at(1).second == "text/html"
                    );

    EXPECT_TRUE(success);
}