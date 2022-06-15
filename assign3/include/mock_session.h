#ifndef MOCK_SESSION_H
#define MOCK_SESSION_H

#include "gmock/gmock.h"
#include "session_interface.h"

/**
 * Mock session class.
 */
class mock_session: public session_interface 
{
  public:
    MOCK_METHOD(tcp::socket&, socket, (), (override));
    MOCK_METHOD(bool, start, (), (override));
    MOCK_METHOD(session_interface*, get_session, (boost::asio::io_service& io_service), (override));
};

#endif