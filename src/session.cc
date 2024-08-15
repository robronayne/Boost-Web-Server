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
void session::start()
{
  boost::asio::async_read_until(socket_,
      request_,
      "\r\n\r\n",
      boost::bind(&session::handle_read, this,
        boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred));
}

/**
 * Write to the designated socket using the data buffer. Will seperate different requests by finding \r\n\r\n.
 */
void session::handle_read(const boost::system::error_code& error,
    size_t bytes_transferred)
{
  if (!error)
  {
    std::istream is(&request_);
    std::string total_request;
    while (total_request.find("\r\n\r\n") == -1) {
      char new_input;
      is.get(new_input);
      total_request += new_input;
    }

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
}

/**
 * Read from the designated socket into the data buffer. Process the same as start().
 */
void session::handle_write(const boost::system::error_code& error)
{
  if (!error)
  {
    boost::asio::async_read_until(socket_,
      request_,
      "\r\n\r\n",
      boost::bind(&session::handle_read, this,
        boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred));
  }
  else
  {
    delete this;
  }
}