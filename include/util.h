#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <string>
#include "user_profile.h"
#include "boost/filesystem.hpp"

#include <fstream>

namespace fs = boost::filesystem;

class util
{
    public:
      std::string get_stock_reply(int err_code);
      std::string get_profile_reply(user_profile user);
      bool read_file(std::string path, std::string& data);
};

#endif