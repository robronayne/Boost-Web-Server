#include "request_handler/error_handler.h"

/**
 * Constructor for generic error; defaults to OK.
 */
error_handler::error_handler()
{
  err_code = reply::status_type::ok;
}

/**
 * Generate an error handler with specified message for specific request.
 */
error_handler::error_handler(reply::status_type ec) : err_code(ec) {}

/**
 * Set the error code for the error handler object.
 */
void error_handler::set_error_code(reply::status_type ec)
{
  err_code = ec;
}

/**
 * Construct an HTTP structured reply for the erroneous request.
 */
reply error_handler::get_reply()
{
  return reply_.stock_reply(err_code);
}