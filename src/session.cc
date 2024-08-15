#include <cstdlib>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

#include "reply.h"
#include "session.h"

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
 * Start a session with the intended configuration. Will read from a socket until \r\n\r\n is entered.
 */
bool session::start()
{
  boost::asio::async_read_until(socket_,
      request_,
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
  std::string ret = "";
  if (!error)
  {
    std::istream is(&request_);
    std::string total_request;
    while (total_request.find("\r\n\r\n") == -1) {
      char new_input;
      is.get(new_input);
      total_request += new_input;
    }
    ret = total_request;

    reply rep;
    boost::asio::async_write(socket_,
        rep.to_buffers(total_request, bytes_transferred),
        boost::bind(&session::handle_write, this,
        boost::asio::placeholders::error));
    total_request.clear();
  }
  else
  {
    delete this;
  }
  return ret;
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

    return true;
    /**boost::asio::async_read_until(socket_,
      request_,
      "\r\n\r\n",
      boost::bind(&session::handle_read, this,
        boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred));**/
  }
  else
  {
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