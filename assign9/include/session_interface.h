#ifndef SESSION_INTERFACE_H
#define SESSION_INTERFACE_H

#include <cstdlib>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

#include "config_parser.h"
#include "request_handler_factory/request_handler_factory.h"
#include <map>

using boost::asio::ip::tcp;

/**
 * Session interface class
 *
 * Session objects contain socket information and
 * data that is passed from a read or write operation.
 */
class session_interface 
{
  public:
    virtual tcp::socket& socket() = 0;
    virtual bool start() = 0;
    virtual session_interface* get_session(boost::asio::io_service& io_service) = 0;
    virtual bool set_paths(std::vector<path> paths) = 0;
    virtual bool set_routes(std::map<std::string, request_handler_factory*> route) = 0;
};

#endif