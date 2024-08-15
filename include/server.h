#ifndef SERVER_H
#define SERVER_H

#include <boost/bind.hpp>
#include <boost/asio.hpp>

#include "session.h"
#include "session_interface.h"

using boost::asio::ip::tcp;

/**
 * Skeleton web server class.
 *
 * The function start_accept() creates a new session,
 * and calls handle_accept() which verifies the session, 
 * and in turn starts the server.
 */
class server 
{
  private:
    boost::asio::io_service& io_service_;
    session_interface& session_;
    tcp::acceptor acceptor_;
  public:
    bool start_accept();
    bool handle_accept(session_interface* new_session,
      const boost::system::error_code& error);
    server(session_interface& new_s, boost::asio::io_service& io_service, short port);
};

#endif