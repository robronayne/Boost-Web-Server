#ifndef PATH_H
#define PATH_H

#include <string>

/** 
 * A path struct will contain the endpoint type, i.e. "static", "echo", etc.
 * as well as the root for where to locate the file.
 */
enum endpoint_type { static_, echo, invalid };
struct path 
{
  endpoint_type type;
  std::string endpoint;
  std::string root;
};

#endif
