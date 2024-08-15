#include <regex>
#include <cstdlib>
#include <iterator>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/log/trivial.hpp>
#include <boost/beast/core/buffers_to_string.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>

#include "session.h"
#include "http/path.h"
#include "request_handler/echo_handler.h"
#include "request_handler/error_handler.h"
#include "request_handler/static_handler.h"

namespace http = boost::beast::http;

/**
 * Constructor for the session class.
 */
session::session(boost::asio::io_service& io_service) : socket_(io_service) {}

/**
 * Accessor for private socket_ variable.
 */
tcp::socket& session::socket()
{
  return socket_;
}

/**
 * Setter for path vector.
 */
bool session::set_paths(std::vector<path> paths)
{
  paths_ = paths;
  return true;
}

/**
 * Setter for request.
 */
bool session::set_request(http::request<http::dynamic_body> request)
{
  request_ = request;
  return true;
}

/* 
 * Start a session with the intended configuration. 
 * Will read from a socket until \r\n\r\n is entered.
 */
bool session::start()
{
  boost::system::error_code ec;
  socket_.remote_endpoint(ec);
  
  if (ec) 
  {
    client_ip = "127.0.0.1";
  }
  else 
  {
    client_ip = socket_.remote_endpoint(ec).address().to_string();
  }
  
  log_message_info("status: accepting incoming requests", "start session");
  http::async_read(socket_,
                   buffer_,
                   request_,
                   boost::bind(&session::handle_read, this,
                   boost::asio::placeholders::error,
                   boost::asio::placeholders::bytes_transferred));
  return true;
}

/*
 * Write to the designated socket using the data buffer. 
 * Will seperate different requests by finding \r\n\r\n.
 */
std::string session::handle_read(const boost::system::error_code& error,
    size_t bytes_transferred)
{
  std::ostringstream ostring;
  ostring << request_;
  std::string request_string = ostring.str();
  if (!error)
  {
    
    // If valid request, check for valid endpoint.
    
    log_message_info("good request", "request parser");
    
    // Print the request breakdown.
    log_message_info(request_string, "request parser result");

    // get the location string
    std::string location = match(routes_, std::string(request_.target()));
    // create factory from the location
    request_handler_factory* factory = routes_[location];
    // create handler
    request_handler_interface* handler = factory->create(location, std::string(request_.target()));
    
    // Write the output of the request handler to the socket.
    write_to_socket(handler);

    delete handler;
  }
  else
  {
    log_message_info("bad request", "request parser");
    log_message_info(request_string, "request parser result");
    delete this;
  }
  return request_string;
}

/**
 * Handle write to the socket by using request handler interface.
 */
void session::write_to_socket(request_handler_interface* handler)
{
  boost::system::error_code ec;
  socket_.remote_endpoint(ec);
  if (ec) {
    log_message_error(ec.message(), "error writing to socket");
    return;
  }

  log_message_info("wrote to socket", "request handler");
  http::response <http::dynamic_body> response_;
  handler->serve(request_, response_);
  http::write(socket_, response_);
  handle_write(boost::system::error_code());
}

/* 
 * Temporarily close connection after each request.
 * Commands like curl work require this in order to work properly.
 */
bool session::handle_write(const boost::system::error_code& error)
{
  if (!error)
  {
    // Close connection after request to ensure http requests are sent properly.
    boost::system::error_code ignored_ec;
    socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);

    log_message_info("connection closed successfully", "request received");
    return true;
  }
  else
  {
    log_message_error("error closing connection", "request received");
    delete this;
    return false;
  }
}

/**
 * Get a new session instance to decouple from the server class.
 */
session_interface* session::get_session(boost::asio::io_service& io_service)
{
  return new session(io_service);
}

/**
 * Log a info level message with client IP.
 */
void session::log_message_info(std::string message, std::string log_type) 
{
  BOOST_LOG_TRIVIAL(info) << "Client "
                          << client_ip
                          << " " 
                          << log_type
                          << ":\n"
                          << message;
}

/**
 * Log an error level message with client IP.
 */
void session::log_message_error(std::string message, std::string log_type) 
{
  BOOST_LOG_TRIVIAL(error) << "Client "
                           << client_ip
                           << " " 
                           << log_type
                           << ":\n"
                           << message;
}

/*
 *  longest prefix match to find the location
 */
std::string session::match(std::map<std::string, request_handler_factory*> routes, std::string url)
{
  const char delim = '/';
  // start with original url
  size_t pos = url.length();
  std::string string_to_match = url.substr(0, pos);

  while (pos != std::string::npos) 
  {
    string_to_match = string_to_match.substr(0, pos);
    if (routes.find(string_to_match) != routes.end())
    {
      return string_to_match;
    }
    // use reverse find to string from the end of string to the start of string
    pos = string_to_match.rfind(delim);
  }
  return "/"; 
}

/**
 *  Set the map routes_ to input route
 */
bool session::set_routes(std::map<std::string, request_handler_factory*> route)
{
  routes_ = route;
  return true;
}