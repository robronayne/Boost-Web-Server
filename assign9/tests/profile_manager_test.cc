#include <fstream>
#include <stdio.h>

#include "profile_manager.h"
#include "user_profile.h"
#include "gtest/gtest.h"

class profileManagerFixture : public ::testing::Test
{
  protected:
    const std::string directory = "../tests/";
    const std::string data_path = directory + "user_database.txt";
};

// Test that the correct user ID is returned for existing test user.
TEST_F(profileManagerFixture, existingUserCheck)
{
  // Create/overwrite test database with one user.
  std::ofstream database_create(data_path);
  database_create << "1|1|existing-user@example.com|name|N/A" << std::endl;
  database_create.close();

  // Check ID of existing user.
  profile_manager manager(directory);
  int existing_id = manager.get_user_id("existing-user@example.com", "name");
  bool success = (existing_id == 1);

  EXPECT_TRUE(success);
}

// Test that the correct user ID is returned for new test user.
TEST_F(profileManagerFixture, newUserCheck)
{
  // Create/overwrite test database with one user.
  std::ofstream database_create(data_path);
  database_create << "1|1|existing-user@example.com|name|N/A" << std::endl;
  database_create.close();

  // Add new user to database.
  profile_manager manager(directory);
  int new_id = manager.get_user_id("new-user@example.com", "new-name");
  bool success = (new_id == 2);

  // Reset test database to only original user.
  std::ofstream database_reset(data_path);
  database_reset << "1|1|existing-user@example.com|name|N/A" << std::endl;
  database_reset.close();

  EXPECT_TRUE(success);
}

// Test for correct OAuth cookie update return.
TEST_F(profileManagerFixture, correctOAuthCookie)
{
  // Create/overwrite test database with one user.
  std::ofstream database_create(data_path);
  database_create << "1|1|test-user@example.com|test-user|N/A" << std::endl;
  database_create.close();

  // Ensure user ID was correctly updated.
  profile_manager manager(directory);
  std::string updated_cookie = manager.update_cookie("OAuth|test-user|test-user@example.com");
  bool success = (updated_cookie == "\"1|test-user|test-user@example.com\"");

  EXPECT_TRUE(success);
}

// Test for proper new user signup
TEST_F(profileManagerFixture, newUserSignUp)
{
  // Create/overwrite test database with one user.
  std::ofstream database_create(data_path);
  database_create << "1|1|test-user@example.com|test-user|N/A" << std::endl;
  database_create.close();

  // Attempt to sign up a new user.
  profile_manager manager(directory);
  bool success = manager.signup("new-user@example.com", "new-user", "new-password");

  EXPECT_TRUE(success);
}

// Test for attempt to signup for existing user.
TEST_F(profileManagerFixture, existingUserSignUp)
{
  // Create/overwrite test database with one user.
  std::ofstream database_create(data_path);
  database_create << "1|1|test-user@example.com|test-user|N/A" << std::endl;
  database_create.close();

  // Attempt to sign up a new user.
  profile_manager manager(directory);
  bool success = manager.signup("test-user@example.com", "test-user", "password");

  EXPECT_FALSE(success);
}

// Test for attempt to login to OAuth profile.
TEST_F(profileManagerFixture, loginOAuth)
{
  // Create/overwrite test database with one user.
  std::ofstream database_create(data_path);
  database_create << "1|1|oauth-user@example.com|test-user|N/A" << std::endl;
  database_create.close();

  // Attempt to login to OAuth profile.
  profile_manager manager(directory);
  user_profile profile;
  bool success = manager.login("oauth-user@example.com", "N/A", profile);

  EXPECT_FALSE(success);
}

// Test for proper login attempt.
TEST_F(profileManagerFixture, loginProper)
{
  // Create user profile.
  std::ofstream database_create(data_path);
  database_create << "";
  database_create.close();
  profile_manager manager(directory);
  manager.signup("test-user@example.com", "test-user", "password");

  // Attempt to login to user profile.
  user_profile profile;
  bool success = (manager.login("test-user@example.com", "password", profile) && 
                  profile.username == "test-user" &&
                  profile.email == "test-user@example.com");

  EXPECT_TRUE(success);
}

// Test for attempt to login to with incorrect password.
TEST_F(profileManagerFixture, badPassword)
{
  // Create user profile.
  std::ofstream database_create(data_path);
  database_create << "";
  database_create.close();
  profile_manager manager(directory);
  manager.signup("test-user@example.com", "test-user", "password");

  // Attempt to login to user profile with wrong password.
  user_profile profile;
  bool success = (manager.login("test-user@example.com", "incorrect-password", profile) && 
                  profile.user_id == -1);

  EXPECT_FALSE(success);
}

// Test for correct Ngineer cookie update return.
TEST_F(profileManagerFixture, correctNgineersCookie)
{
  // Create user profile.
  std::ofstream database_create(data_path);
  database_create << "";
  database_create.close();
  profile_manager manager(directory);
  manager.signup("test-user@example.com", "test-user", "password");

  // Ensure user ID was correctly updated.
  std::string updated_cookie = manager.update_cookie("Ngineers|test-user@example.com|password");
  bool success = (updated_cookie == "\"1|test-user|test-user@example.com\"");

  EXPECT_TRUE(success);
}

// Test for malformed cookie return.
TEST_F(profileManagerFixture, malformedCookie)
{
  // Create/overwrite test database with one user.
  std::ofstream database_create(data_path);
  database_create << "1|1|test-user@example.com|test-user|N/A" << std::endl;
  database_create.close();

  // Ensure empty cookie is generated
  profile_manager manager(directory);
  std::string updated_cookie = manager.update_cookie("badcookie");
  bool success = (updated_cookie == "");

  EXPECT_TRUE(success);
}

// Test for proper new user signup string
TEST_F(profileManagerFixture, goodSignupString)
{
  // Create/overwrite test database.
  std::ofstream database_create(data_path);
  database_create << "";
  database_create.close();

  // Attempt to sign up a new user.
  profile_manager manager(directory);
  bool success = manager.signup_string("new-user|new-password|new-user@example.com");

  EXPECT_TRUE(success);
}

// Test for malformed new user signup string
TEST_F(profileManagerFixture, badSignupString)
{
  // Create/overwrite test database.
  std::ofstream database_create(data_path);
  database_create << "";
  database_create.close();

  // Attempt to sign up a new user, should fail.
  profile_manager manager(directory);
  bool success = !manager.signup_string("badsignupstring");

  EXPECT_TRUE(success);
}