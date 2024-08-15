#ifndef PATH_H
#define PATH_H

#include <map>
#include <string>

/**
 * A path struct will contain the endpoint type, i.e. "static", "echo", etc.
 * as well as the root for where to locate the file.
 */
enum endpoint_type { static_, echo, 
                     api_, health, 
                     sleep_, profile,
                     auth, login, logout,
                     signup, invalid, 
                     not_found, console};
struct path
{
  endpoint_type type;
  std::string endpoint;
  // A mapping from each name to the uri(s) associated with it.
  std::map<std::string, std::string> info_map;
};

#endif
