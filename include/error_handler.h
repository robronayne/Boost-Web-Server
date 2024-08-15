#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

#include "request_handler_interface.h"
#include "reply.h"

class error_handler : public request_handler_interface
{
  public:
    error_handler();
    error_handler(reply::status_type ec);

    void set_error_code(reply::status_type ec);
    reply get_reply();

  private:
    reply::status_type err_code;
    reply rep;

};













#endif