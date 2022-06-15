#include "error_handler.h"

error_handler::error_handler()
{
  err_code = reply::status_type::ok;
}

error_handler::error_handler(reply::status_type ec) : err_code(ec)
{

}

void error_handler::set_error_code(reply::status_type ec)
{
  err_code = ec;
}

reply error_handler::get_reply()
{
  return rep.stock_reply(err_code);
}