#ifndef ECHO_HANDLER_H
#define ECHO_HANDLER_H

#include <string>
#include <vector>
#include <cstddef>

#include "http/request.h"
#include "request_handler_interface.h"

/**
 * Echo handler class
 *
 * Inherits from request_handler_interface
 *
 * The echo handler constructs the appropriate echo
 * response given a request object.
 *
 * The function set_request() is used to pass a request
 * object and get_reply() constructs the echo response
 * with the appropriate headers.
 */
class echo_handler : public request_handler_interface
{
  public:
    echo_handler();
    echo_handler(request request_, size_t byte_transferred);

    void set_request(request request_, size_t byte_transferred);
    reply get_reply();

  private:
    std::string request_body;
    size_t size;
    reply reply_;
};

#endif