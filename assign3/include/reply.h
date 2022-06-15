#include <string>
#include <vector>
#include <boost/asio.hpp>

struct reply
{
  enum status_type
  {
    ok = 200,
    not_found = 404,
    unknow = 999
  } status;

  std::vector<boost::asio::const_buffer> to_buffers(const std::string& request, size_t bytes_transferred);

};