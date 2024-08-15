#include "gtest/gtest.h"
#include "http/mime_types.h"

class mimeTypesFixture : public ::testing::Test {};

// Test the MIME matching for gif files.
TEST_F(mimeTypesFixture, gifTest)
{
  // Set the extension type and mime type.
  std::string extension = "gif";

  bool success = (extension_to_type(extension) == "image/gif");

  EXPECT_TRUE(success);
}

// Test the MIME matching for htm files.
TEST_F(mimeTypesFixture, htmTest)
{
  // Set the extension type and mime type.
  std::string extension = "htm";

  bool success = (extension_to_type(extension) == "text/html");

  EXPECT_TRUE(success);
}

// Test the MIME matching for html files.
TEST_F(mimeTypesFixture, htmlTest)
{
  // Set the extension type and mime type.
  std::string extension = "html";

  bool success = (extension_to_type(extension) == "text/html");

  EXPECT_TRUE(success);
}

// Test the MIME matching for jpg files.
TEST_F(mimeTypesFixture, jpgTest)
{
  // Set the extension type and mime type.
  std::string extension = "jpg";

  bool success = (extension_to_type(extension) == "image/jpeg");

  EXPECT_TRUE(success);
}

// Test the MIME matching for png files.
TEST_F(mimeTypesFixture, pngTest)
{
  // Set the extension type and mime type.
  std::string extension = "png";

  bool success = (extension_to_type(extension) == "image/png");

  EXPECT_TRUE(success);
}

// Test the MIME matching for txt files.
TEST_F(mimeTypesFixture, txtTest)
{
  // Set the extension type and mime type.
  std::string extension = "txt";

  bool success = (extension_to_type(extension) == "text/plain");

  EXPECT_TRUE(success);
}

// Test the MIME matching for jpeg files.
TEST_F(mimeTypesFixture, jpegTest)
{
  // Set the extension type and mime type.
  std::string extension = "jpeg";

  bool success = (extension_to_type(extension) == "image/jpeg");

  EXPECT_TRUE(success);
}

// Test the MIME matching for pdf files.
TEST_F(mimeTypesFixture, pdfTest)
{
  // Set the extension type and mime type.
  std::string extension = "pdf";

  bool success = (extension_to_type(extension) == "application/pdf");

  EXPECT_TRUE(success);
}

// Test the MIME matching for zip files.
TEST_F(mimeTypesFixture, zipTest)
{
  // Set the extension type and mime type.
  std::string extension = "zip";

  bool success = (extension_to_type(extension) == "application/zip");

  EXPECT_TRUE(success);
}

// Test the MIME matching for unknown file types.
TEST_F(mimeTypesFixture, defaultTest)
{
  // Set the extension type and mime type.
  std::string extension = "I don't know what type it is";

  bool success = (extension_to_type(extension) == "text/plain");

  EXPECT_TRUE(success);
}