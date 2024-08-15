#include "profile_manager.h"
#include <boost/log/trivial.hpp>

/**
 * Generate a profile manager that will interact with a database at a
 * specified location.
 * 
 * Profiles will now be stored in the following format:
 * id | oauth flag | email | name | password
 * where the oauth flag will be set to 1 or 0 to differentiate between
 * signup profiles and those generated through OAuth.
 */
profile_manager::profile_manager(std::string path) : 
    directory(path) {}

/**
/**
 * Wrapper for the signup function to process information strings
 */

bool profile_manager::signup_string(std::string data)
{
  std::vector<std::string> info;

  // Extract user info from signup string.
  int begin = 0;
  int end = data.find("|", begin);
  while (end != std::string::npos) 
  {
      info.push_back(data.substr(begin, end - begin));
      begin = end+1;
      end = data.find("|", begin);
  }
  if (begin < data.size())
  {
    info.push_back(data.substr(begin, data.size()));
  }

  // Check malformed request size
  if (info.size() != 3)
  {
    return false;
  }

  return signup(info.at(2), info.at(0), info.at(1));
}
/**
/**
 * Handles signups for new users outside of OAuth.
 *
 * Will return false if user already exists in database (emails must be unique),
 * or if some of the fields provided are invalid.
 */
bool profile_manager::signup(std::string email, std::string name, std::string password)
{
  // Keep track of users detected, intialized to zero.
  int num_users = 0;
  int user_id;

  const std::string data_path = directory + "user_database.txt";
  boost::filesystem::path database_path(data_path);

  if (boost::filesystem::exists(database_path)) 
  {
    std::string line;
    std::ifstream database_in(data_path);

    // Check every line of the database for the email address.
    while (std::getline(database_in, line))
    {
      std::string::size_type id_end = line.find('|');
      std::string::size_type flag_end = line.find('|', id_end+1);
      std::string::size_type email_end = line.find('|', flag_end+1);

      if (id_end != std::string::npos)
      {
        // Increment the number of users.
        num_users++;

        // Compare line substring containing email address.
        std::string::size_type email_length = (email_end-flag_end)-1;
        if (line.substr(flag_end+1, email_length) == email)
        {          
          // User already exists in the database.
          log_message("User: " + email + " already present in database");
          database_in.close();
          return false; 
        }
      }
    }

    database_in.close();
    
    // User ID reflects newest user.
    user_id = num_users + 1;

    std::string filename(data_path);
    std::ofstream database_out;

    database_out.open(filename, std::ios_base::app);
    
    // Write to database with OAuth flag unchecked, hashed password.
    database_out << user_id
                 << "|" << 0 
                 << "|" << email 
                 << "|" << name
                 << "|" << hash_pass(password)
                 << std::endl;

    database_out.close();

    std::string message = "User: " + email + 
                          " successfully added to database with ID: " + std::to_string(user_id);

    log_message(message);
    return true;
  }
  else // File not already present.
  {
    // Create a new database.
    std::ofstream database_create(data_path);
    database_create << 1
                    << "|" << 0 
                    << "|" << email 
                    << "|" << name
                    << "|" << hash_pass(password)
                    << std::endl;

    database_create.close();

    return true;
  }
}

/**
 * Handles logins for users created via the signup feature.
 *
 * Will return true for a successful login, and a user_profile struct
 * that contains all relevant user information.
 * 
 * Upon failure will return false and an empty user profile.
 */
bool profile_manager::login(std::string email, std::string password, user_profile &profile)
{

  // Open the database to search for the user.
  const std::string data_path = directory + "user_database.txt";
  boost::filesystem::path database_path(data_path);

  if (boost::filesystem::exists(database_path)) 
  {
    std::string line;
    std::ifstream database_in(data_path);

    // Check every line of the database for the email address.
    while (std::getline(database_in, line))
    {
      std::string::size_type id_end = line.find('|');
      std::string::size_type flag_end = line.find('|', id_end+1);
      std::string::size_type email_end = line.find('|', flag_end+1);
      std::string::size_type name_end = line.find('|', email_end+1);

      if (id_end != std::string::npos)
      {
        // Compare line substring containing email address.
        std::string::size_type email_length = (email_end-flag_end)-1;
        std::string::size_type name_length = (name_end-email_end)-1;
        if (line.substr(flag_end+1, email_length) == email)
        {
          // First make sure that the user is not an OAuth user.
          if (line[id_end+1] == '0')
          {
            //Check for a password match.
            if (hash_pass(password) == line.substr(name_end+1, 64)) // SHA-256 always 64 characters.
            {
              profile = {std::stoi(line.substr(0, id_end)), 
                         line.substr(email_end+1, name_length),
                         line.substr(flag_end+1, email_length),
                         true};
              database_in.close();
              return true;
            }
            // Invalid login attempt.
            else
            {
              database_in.close();
              profile = {-1, "", "", false};
              return false;
            }
          }
          // Account already exists with Google OAuth.
          else
          {
            database_in.close();
            profile = {-1, "", "", false};
            return false;
          }
        }
      }
    }
  }
  
  // If we get here, the email address was not found within the database.
  profile = {-1, "Email address not present within database", "", false};
  return false;
}

/** 
 * Returns the hashed value of the password that will be stored in the database.
 *
 * Also used for determining if correct passwords are used during login. 
 * Implements SHA256 hashing to accomplish this. Credit to:
 * https://stackoverflow.com/questions/2262386/generate-sha256-with-openssl-and-c/10632725
 * 
 * The database is a text file! We don't ~care~ about hash collisions...
 */
std::string profile_manager::hash_pass(std::string password)
{
  unsigned char hash[SHA256_DIGEST_LENGTH];
  SHA256_CTX sha256;
  SHA256_Init(&sha256);
  SHA256_Update(&sha256, password.c_str(), password.size());
  SHA256_Final(hash, &sha256);
  std::stringstream hashed_password;
  for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
  {
      hashed_password << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
  }
  return hashed_password.str();
}

/**
 * Only meant to be used with OAuth users.
 *
 * Add a user to the database that doesn't already exist
 * and assign the user a new user ID number.
 * 
 * Will return user ID if user already exists, otherwise will
 * return a new user ID. 
 */
int profile_manager::get_user_id(std::string email, std::string name)
{
  // Keep track of users detected, intialized to zero.
  int num_users = 0;
  int user_id;

  const std::string data_path = directory + "user_database.txt";
  boost::filesystem::path database_path(data_path);

  if (boost::filesystem::exists(database_path)) 
  {
    std::string line;
    std::ifstream database_in(data_path);

    // Check every line of the database for the email address.
    while (std::getline(database_in, line))
    {
      std::string::size_type id_end = line.find('|');
      std::string::size_type flag_end = line.find('|', id_end+1);
      std::string::size_type email_end = line.find('|', flag_end+1);

      if (id_end != std::string::npos)
      {
        // Increment the number of users.
        num_users++;

        // Compare line substring containing email address.
        std::string::size_type email_length = (email_end-flag_end)-1;
        if (line.substr(flag_end+1, email_length) == email)
        {
          // Determine user ID from line.
          user_id = std::stoi(line.substr(0, id_end));
          
          // User already exists in the database.
          log_message("User: " + email + " already present in database");
          database_in.close();
          return user_id; 
        }
      }
    }

    database_in.close();
    
    // User ID reflects newest user.
    user_id = num_users + 1;

    std::string filename(data_path);
    std::ofstream database_out;
    
    database_out.open(filename, std::ios_base::app);
    
    if (database_out.is_open())
    {
      // Write to database with OAuth flag checked, no password.
      database_out << user_id
                   << "|" << 1 
                   << "|" << email 
                   << "|" << name
                   << "|" << "N/A"
                   << std::endl;

      database_out.close();
      std::string message = "User: " + email + 
                          " successfully added to database with ID: " + std::to_string(user_id);

      log_message(message);
      return user_id;
    }
    else
    {
      // Error writing to database.
      return -1;
    }
  }
  else // File not already present.
  {
    // Create a new database.
    std::ofstream database_create(data_path);
    
    if (database_create)
    {
      database_create << 1
                      << "|" << 1 
                      << "|" << email 
                      << "|" << name 
                      << "|" << "N/A"
                      << std::endl;
      
      database_create.close();
      return 1;
    }
    else
    {
      // Error writing to database.
      return -1;
    }
  }
}

/*
 * Update data present from cookie.
 */
std::string profile_manager::update_cookie(std::string cookie_string)
{
  std::string updated_data;

  std::vector<std::string> info;
  std::string data = cookie_string;

  // Extract user info from cookie.
  int begin = 0;
  int end = data.find("|", begin);
  while (end != std::string::npos) 
  {
      info.push_back(data.substr(begin, end - begin));
      begin = end+1;
      end = data.find("|", begin);
  }
  if (begin < data.size())
  {
    info.push_back(data.substr(begin, data.size()));
  }

  // Check malformed request size
  if (info.size() != 3)
  {
    return updated_data;
  }

  // Check for type of login
  if (info.at(0) == "Ngineers")
  {
    user_profile found_user;
    // Check for user existence
    if(login(info.at(1), info.at(2), found_user))
    {
      updated_data = "\"" + std::to_string(found_user.user_id) + "|" +
                            found_user.username + "|" +
                            found_user.email + "\"";
    }
  }
  else if (info.at(0) == "OAuth")
  {
    // Update the user ID.
    updated_data = "\"" + std::to_string(get_user_id(info.at(2), info.at(1))) + "|" +
                          info.at(1) + "|" +
                          info.at(2) + "\"";
  }

  return updated_data;
}

/**
 * Log message when a new user is successfully added to
 * the database.
 */
void profile_manager::log_message(std::string message) 
{
  BOOST_LOG_TRIVIAL(info) << message;
}