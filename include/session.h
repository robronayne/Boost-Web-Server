#ifndef SESSION_H
#define SESSION_H

#include <cstdlib>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

/**
 * Skeleton session class.
 *
 * Session objects contain socket information and
 * data that is passed from a read or write operation.
 *
 * The functions handle_read() and handle_write() both use
 * the boost library to write to a designated socket or read
 * from one respectively.
 */
class session 
{
  private:
    tcp::socket socket_;
    enum { max_length = 1024 };
    boost::asio::streambuf request_;
    void handle_read(const boost::system::error_code& error,
      size_t bytes_transferred);
    void handle_write(const boost::system::error_code& error);
  public:
    session(boost::asio::io_service& io_service);
    tcp::socket& socket();
    void start();
};

#endif