#ifndef MOCK_SESSION_H
#define MOCK_SESSION_H

#include "request_handler_factory/request_handler_factory.h"
#include "gmock/gmock.h"
#include "session_interface.h"

/**
 * Mock session class
 *
 * Inherits from session_interface
 */
class mock_session: public session_interface 
{
  public:
    MOCK_METHOD(tcp::socket&, socket, (), (override));
    MOCK_METHOD(bool, start, (), (override));
    MOCK_METHOD(session_interface*, get_session, (boost::asio::io_service& io_service), (override));
    MOCK_METHOD(bool, set_paths, (std::vector<path> paths), (override));
    MOCK_METHOD(bool, set_routes, ((std::map<std::string, request_handler_factory*> route)), (override));
};

#endif