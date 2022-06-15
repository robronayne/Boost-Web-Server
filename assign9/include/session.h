#ifndef SESSION_H
#define SESSION_H

#include <cstdlib>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <map>
#include <string>

#include "request_handler_factory/request_handler_factory.h"
#include "session_interface.h"
#include "request_handler/request_handler_interface.h"
#include "request_handler_factory/echo_handler_factory.h"
#include "request_handler_factory/_404_handler_factory.h"
#include "request_handler_factory/static_handler_factory.h"


using boost::asio::ip::tcp;

/**
 * Session class
 *
 * Inherits from session_interface
 * 
 * Session objects contain socket information and
 * data that is passed from a read or write operation.
 *
 * The functions handle_read() and handle_write() both use
 * the boost library to write to a designated socket or read
 * from one respectively.
 */
class session : public session_interface
{
  public:
    session(boost::asio::io_service& io_service);
    session_interface* get_session(boost::asio::io_service& io_service);
    
    bool start();
    tcp::socket& socket();
    void write_to_socket(request_handler_interface* handler);

    bool handle_write(const boost::system::error_code& error);  
    std::string handle_read(const boost::system::error_code& error,
      size_t bytes_transferred);

    void log_message_info(std::string message, std::string log_type);
    void log_message_error(std::string message, std::string log_type);
    boost::asio::streambuf buffer_;
    
    bool set_paths(std::vector<path> paths);

    bool set_routes(std::map<std::string, request_handler_factory*> route);

    // longest prefix matching for location and url.
    std::string match(std::map<std::string, request_handler_factory*> routes, std::string url);
    
    bool set_request(http::request<http::dynamic_body> request);
    bool update_user(std::string cookie_string, user_profile& user);

    void logIn() {profile.login_status = true;}
    void logOut() {profile.login_status = false;}

  private:
    tcp::socket socket_;
    enum { max_length = 1024 };
    
    std::vector<path> paths_;
    std::string client_ip;
    
    http::request<http::dynamic_body> request_;

    std::string get_info();

    std::map<std::string, request_handler_factory*> routes_;

    // default user profile
    user_profile profile = {0, "", "", false};
};
#endif