#ifndef SERVER_H
#define SERVER_H

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>

#include "session_interface.h"
#include "request_handler_factory/request_handler_factory.h"
#include <map>
#include <string>

using boost::asio::ip::tcp;

static const int NUM_THREADS = 4;

/**
 * Web server class.
 *
 * The function start_accept() creates a new session,
 * and calls handle_accept() which verifies the session, 
 * and in turn starts the server.
 */
class server 
{
  public:
    server(session_interface& new_s, boost::asio::io_service& io_service, short port);
    
    void run();
    bool start_accept();
    bool handle_accept(session_interface* new_session, const boost::system::error_code& error);
    bool set_paths(std::vector<path> paths);

    std::map<std::string, request_handler_factory*> create_handler_factory(path p);
    bool create_routes();

  private:
    boost::asio::io_service& io_service_;
    session_interface& session_;
    tcp::acceptor acceptor_;
    std::vector<path> paths_;
    std::map<std::string, request_handler_factory*> routes;
};

#endif