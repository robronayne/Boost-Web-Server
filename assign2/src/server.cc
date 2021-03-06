#include <cstdlib>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

#include "server.h"
#include "session.h"

/**
 * Constructor for the server class.
 */
server::server(boost::asio::io_service& io_service, short port) 
  : io_service_(io_service),
    acceptor_(io_service, tcp::endpoint(tcp::v4(), port))
{
  start_accept();
}

/**
 * Create a new server and attempt to configure it.
 */
void server::start_accept()
{
  session* new_session = new session(io_service_);
  acceptor_.async_accept(new_session->socket(),
      boost::bind(&server::handle_accept, this, new_session,
      boost::asio::placeholders::error));
}

/**
 * Pending success of accepting configuration, start a new session.
 */
void server::handle_accept(session* new_session,
    const boost::system::error_code& error)
{
  if (!error)
  {
    new_session->start();
  }
  else
  {
    delete new_session;
  }

  start_accept();
}
