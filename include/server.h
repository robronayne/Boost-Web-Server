#ifndef SERVER_H
#define SERVER_H

#include <boost/bind.hpp>
#include <boost/asio.hpp>

#include "session.h"

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
    tcp::acceptor acceptor_;
    void start_accept();
    void handle_accept(session* new_session,
      const boost::system::error_code& error);
  public:
    server(boost::asio::io_service& io_service, short port);
};    

#endif