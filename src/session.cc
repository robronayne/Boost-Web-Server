#include <cstdlib>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/log/trivial.hpp>
#include <boost/beast/core/buffers_to_string.hpp>
#include <iterator>
#include <regex>

#include "session.h"
#include "echo_handler.h"
#include "error_handler.h"
#include "static_handler.h"

/**
 * Constructor for the session class.
 */
session::session(boost::asio::io_service& io_service) : socket_(io_service)
{

}

/**
 * Accessor for private socket_ variable.
 */
tcp::socket& session::socket()
{
  return socket_;
}

/**
 * Setter for path vector
 */
bool session::set_paths(std::vector<path> paths)
{
  paths_ = paths;
  return true;
}

/**
 * Start a session with the intended configuration. Will read from a socket until \r\n\r\n is entered.
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

/**
 * Write to the designated socket using the data buffer. Will seperate different requests by finding \r\n\r\n.
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

    request_handler_interface* handler;
    if (result == request_parser::good)
    {
      //request_handler_.handle_request(request_, reply_);
      log_message_info("good request", "request parser");
      
      //print request breakdown
      log_message_info(get_info(), "request parser result");

      path request_endpoint = get_endpoint();
      
      if (request_endpoint.type == endpoint_type::echo)
      {
        handler = new echo_handler(request_, bytes_transferred);
      }
      else if (request_endpoint.type == endpoint_type::static_)
      {
        // use static handler instead
        handler = new static_handler(request_, request_endpoint.root);
      }
      else
      {
        handler = new error_handler(reply::status_type::not_found);
      }
    }
    else if (result == request_parser::bad)
    {
      //reply_ = reply::stock_reply(reply::bad_request);
      log_message_info("bad request", "request parser");

      log_message_info(request_.original_req, "request parser result");

      handler = new error_handler(reply::status_type::bad_request);
    }
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
  * Handle write to the socket by using request handler interface
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
  boost::asio::write(socket_,
    handler->get_reply().to_buffers());
  handle_write(boost::system::error_code());
}

/**
 * Temporarily close connection after each request to ensure commands like curl work properly
 */
bool session::handle_write(const boost::system::error_code& error)
{
  if (!error)
  {
    // Close connection after request to ensure http requests are sent properly
    boost::system::error_code ignored_ec;
    socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);

    log_message_info("connection closed successfully", "end session");
    return true;
    /**boost::asio::async_read_until(socket_,
      buffer_,
      "\r\n\r\n",
      boost::bind(&session::handle_read, this,
        boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred));**/
  }
  else
  {
    log_message_error("error closing connection", "end session");
    delete this;
    return false;
  }
}

/**
 * Get a new session instance to decouple from the server class
 */
session_interface* session::get_session(boost::asio::io_service& io_service)
{
  return new session(io_service);
}

/**
 * Log a info level message with client ip
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
 * Function to format request information nicely
 */
std::string session::get_info()
{
  std::string r = "method: " + request_.method
                  + "\nuri: " + request_.uri 
                  + "\nversion: " + std::to_string(request_.http_version_major)
                  + "." + std::to_string(request_.http_version_minor);
  for (int i = 0; i < request_.headers.size(); i++) {
    r += "\nheader_" + std::to_string(i) + ": " 
          + "\n\tname: " + request_.headers.at(i).name
          + "\n\tvalue: " + request_.headers.at(i).value;
  }
  return r;
}

/**
  Function to extract endpoint type from current request
**/
path session::get_endpoint()
{
  path result;
  result.type = endpoint_type::invalid;
  std::regex e ("^((/[a-zA-Z_0-9]+)*/)([a-zA-Z_0-9.]+)$");

  std::smatch match;
  if (std::regex_search(request_.uri, match, e))
  {
    std::string partial = match.str(1);
    for (int i = 0; i < paths_.size(); i++)
    {  
      if (partial == paths_.at(i).endpoint) 
      {
        result = paths_.at(i);
      }
      else if (request_.uri == paths_.at(i).endpoint)
      {
        return paths_.at(i);
      }
    }
  }

  log_message_info(result.endpoint, "endpoint retrieved");
  return result;
}