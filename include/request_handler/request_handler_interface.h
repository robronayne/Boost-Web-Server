#ifndef REQUEST_HANDLER_INTERFACE_H
#define REQUEST_HANDLER_INTERFACE_H

#include <vector>

#include "http/reply.h"

/** 
 * Parent class for request interface objects. Request handlers 
 * are designed to return a HTTP reply given different requests 
 * that are received.
 */
class request_handler_interface
{
  public:
    virtual reply get_reply() = 0;
    virtual ~request_handler_interface() {}
};

#endif