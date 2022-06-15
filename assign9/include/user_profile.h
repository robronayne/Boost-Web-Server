#ifndef USER_PROFILE_H
#define USER_PROFILE_H

#include <iostream>
#include <string>

/**
 * A user profile struct will contain relevant user
 * information as well as login status.
 *
 * This will allow handlers such as the API handler to
 * restrict or allow access to certain operations, and
 * the profile handler to display information about the
 * current user.
 */
struct user_profile
{
  int user_id; // Relevant later when we add signup feature.
  std::string username;
  std::string email;
  bool login_status;
};

#endif