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
        case 401:
          return_string = "<html>"
                          "<head><title>Unauthorized</title></head>"
                          "<body><h1>401 Unauthorized</h1></body>"
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

/**
 * Accepts a user_profile as a parameter to display
 * relevant user information.
 */
std::string util::get_profile_reply(user_profile user)
{
  std::string return_string;
  
  return_string = "<html>"
                    "<body>"
                      "<h1>User Profile</h1>"
                      "<h3>User ID number: " + std::to_string(user.user_id) + "</h3>"
                      "<h3>Username: " + user.username + "</h3>"
                      "<h3>User Email Address: " + user.email + "</h3>"
                    "</body>"
                  "</html>";

  // Later we can potentially display CRUD operations
  // that the user has performed in a history section.

  return return_string;
}

bool util::read_file(std::string path, std::string& data)
{
  // check if file is valid
  fs::path filePath(path);
  if (!exists(filePath) || !is_regular_file(filePath))
  {
    return false;
  }
  // read file
  std::ifstream f(path);
  std::string temp_data;
  char c;
  if (f.is_open())
  {
    while (f.get(c))
    {
      temp_data += c;
    }
    f.close();
  }
  else
  {
      return false;
  }

  data = temp_data;
  return true;
}