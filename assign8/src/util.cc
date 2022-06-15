#include "util.h"

/* Gets the appropriate stock reply given the error code */
std::string util::get_stock_reply(int err_code)
{
    std::string return_string;
    switch (err_code)
    {
        case 200:
          return_string = "";
          break;
        case 400:
          return_string = "<html>"
                          "<head><title>Bad Request</title></head>"
                          "<body><h1>400 Bad Request</h1></body>"
                          "</html>";
          break;
        default:
          return_string = "<html>"
                          "<head><title>Not Found</title></head>"
                          "<body><h1>404 Not Found</h1></body>"
                          "</html>";
          break;
    }
    return return_string;
}