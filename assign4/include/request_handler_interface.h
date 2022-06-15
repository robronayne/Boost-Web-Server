#ifndef REQUEST_HANDLER_INTERFACE_H
#define REQUEST_HANDLER_INTERFACE_H

#include <vector>

#include "reply.h"

class request_handler_interface
{
  public:
    virtual reply get_reply() = 0;
    virtual ~request_handler_interface() {}

};



#endif