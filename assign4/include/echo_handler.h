#ifndef ECHO_HANDLER_H
#define ECHO_HANDLER_H

#include "request_handler_interface.h"
#include <cstddef>
#include <string>
#include <vector>
#include "request.h"

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
    reply rep;

};





#endif