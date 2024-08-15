#ifndef PROFILE_MANAGER
#define PROFILE_MANAGER

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <sstream>
#include <openssl/sha.h>

#include "user_profile.h"
#include "boost/filesystem.hpp"

namespace fs = boost::filesystem;

/**
 * Allows the creation of user profiles in a database (text file)
 * and creates a corresponding user id with each new user.
 */
class profile_manager
{
  public:
    profile_manager(std::string path);
    bool signup(std::string email, std::string name, std::string password);
    bool login(std::string email, std::string password, user_profile &profile);
    int get_user_id(std::string email, std::string name);
    std::string update_cookie(std::string cookie_string);
    bool signup_string(std::string data);

  private:
    std::string hash_pass(std::string password);
    void log_message(std::string message);
    std::string directory;
};

#endif