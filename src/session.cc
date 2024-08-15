#include <regex>
#include <cstdlib>
#include <iterator>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/log/trivial.hpp>
#include <boost/beast/core/buffers_to_string.hpp>

#include "session.h"
#include "http/path.h"
#include "request_handler/echo_handler.h"
#include "request_handler/error_handler.h"
#include "request_handler/static_handler.h"

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
  boost::asio::async_read_until(socket_,
                                buffer_,
                                "\r\n\r\n",
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
  if (!error)
  {
    request_parser::result_type result;

    do {
      std::istreambuf_iterator<char> st{&buffer_}, end;
      std::tie(result, std::ignore) = request_parser_.parse(
        request_, st, end);
    } while(result == request_parser::indeterminate);

    // If valid request, check for valid endpoint.
    request_handler_interface* handler;
    if (result == request_parser::good)
    {
      log_message_info("good request", "request parser");
      
      // Print the request breakdown.
      log_message_info(get_info(), "request parser result");

      path request_endpoint = get_endpoint();
      
      // Create echo handler if echo endpoint
      if (request_endpoint.type == endpoint_type::echo)
      {
        // Use echo handler for an echo request.
        handler = new echo_handler(request_, bytes_transferred);
      }
      // Create static handler if static endpoint
      else if (request_endpoint.type == endpoint_type::static_)
      {
        // Use static handler for serving specified file instead.
        handler = new static_handler(request_, request_endpoint.root);
      }
      // Create error handler if no valid endpoint is found
      else
      {
        handler = new error_handler(reply::status_type::not_found);
      }
    }
    // If invalid request, create error handler
    else if (result == request_parser::bad)
    {
      // Use an error handler for a bad request.
      log_message_info("bad request", "request parser");
      log_message_info(request_.original_req, "request parser result");

      handler = new error_handler(reply::status_type::bad_request);
    }

    // Write the output of the request handler to the socket.
    write_to_socket(handler);

    delete handler;
  }
  else
  {
    delete this;
  }
  return request_.original_req;
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
  boost::asio::write(socket_, handler->get_reply().to_buffers());
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

/**
 * Function to format request information nicely.
 */
std::string session::get_info()
{
  std::string request_info = "method: " + request_.method
                           + "\nuri: " + request_.uri 
                           + "\nversion: " + std::to_string(request_.http_version_major)
                           + "." + std::to_string(request_.http_version_minor);
  
  // Add headers to the request info.
  for (int i = 0; i < request_.headers.size(); i++) 
  {
    request_info += "\nheader_" + std::to_string(i) + ": " 
                 + "\n\tname: " + request_.headers.at(i).name
                 + "\n\tvalue: " + request_.headers.at(i).value;
  }
  return request_info;
}

/**
 * Function to extract endpoint type from current request.
 */
path session::get_endpoint()
{
  path result;
  result.type = endpoint_type::invalid;
  std::regex e ("^((/[a-zA-Z_0-9]+)*/)([a-zA-Z_0-9.]+)$");

  std::smatch match;
  if (std::regex_search(request_.uri, match, e))
  {
    // Get partial endpoint from requested uri
    std::string partial = match.str(1);
    for (int i = 0; i < paths_.size(); i++)
    {  
      // If partial match, keep searching until end
      if (partial == paths_.at(i).endpoint) 
      {
        result = paths_.at(i);
      }
      // If full match, return immediately
      else if (request_.uri == paths_.at(i).endpoint)
      {
        return paths_.at(i);
      }
    }
  }

  // Return valid endpoint if found, otherwise invalid
  log_message_info(result.endpoint, "endpoint retrieved");
  return result;
}