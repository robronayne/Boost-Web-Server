#ifndef SESSION_INTERFACE_H
#define SESSION_INTERFACE_H

#include <cstdlib>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include "config_parser.h"

using boost::asio::ip::tcp;

/**
 * Skeleton session interface.
 *
 * Session objects contain socket information and
 * data that is passed from a read or write operation.
 */
class session_interface 
{
  public:
    // virtual ~session_interface() {};
    virtual tcp::socket& socket() = 0;
    virtual bool start() = 0;
    virtual session_interface* get_session(boost::asio::io_service& io_service) = 0;
    virtual bool set_paths(std::vector<path> paths) = 0;
};

#endif