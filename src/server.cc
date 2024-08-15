#include <cstdlib>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/log/trivial.hpp>

#include "server.h"
#include "http/path.h"
#include "request_handler_factory/echo_handler_factory.h"
#include "request_handler_factory/static_handler_factory.h"
#include "request_handler_factory/_404_handler_factory.h"

/** 
 * Constructor for the server class.
 */
server::server(session_interface& new_sesh, boost::asio::io_service& io_service, short port)
  : io_service_(io_service),
    session_(new_sesh),
    acceptor_(io_service, tcp::endpoint(tcp::v4(), port)) {}

/**
 * Setter for path vector
 */
bool server::set_paths(std::vector<path> paths)
{
  paths_ = paths;
  return true;
}

/**
 * Create a new server and attempt to configure it.
 * Returns true if session was acquired successfully, false otherwise
 */
bool server::start_accept()
{
  session_interface* new_session = session_.get_session(io_service_);
  if (new_session == NULL) 
  {
    return false;
  }
  // create map routes. map location to factory pointer
  create_routes();
  new_session->set_routes(routes);
  new_session->set_paths(paths_);
  acceptor_.async_accept(new_session->socket(),
      boost::bind(&server::handle_accept, this, new_session,
      boost::asio::placeholders::error));
  return true;
}

/**
 * Pending success of accepting configuration, start a new session.
 * Returns true if successful, false otherwise
 */
bool server::handle_accept(session_interface* new_session,
    const boost::system::error_code& error)
{
  bool result = false;
  if (!error)
  {
    new_session->start();
    result = true;
  }
  else
  {
    delete new_session;
  }

  start_accept();
  return result;
}

/**
 *  Create a request handler factory based on the input path.
 *  Return a map<string, request_handler_factory*> object, and
 *  key is location, value is the factory pointer
 */
std::map<std::string, request_handler_factory*> server::create_handler_factory(path p)
{
  switch(p.type)
  {
    case endpoint_type::static_:
      routes[p.endpoint] = new static_handler_factory(p.endpoint, p);
    break;
    case endpoint_type::echo:
      routes[p.endpoint] = new echo_handler_factory(p.endpoint, p);
    break;
    case endpoint_type::not_found:
      routes[p.endpoint] = new _404_handler_factory(p.endpoint, p);
    break;
  }
  return routes;
}

/**
 *  Create a route map. It takes the address of map and change it
 *  based on path_. Return true if successfully reach the end.
 */
bool server::create_routes()
{
  for (path p : paths_)
  {
    create_handler_factory(p);
  }
  return true;
}