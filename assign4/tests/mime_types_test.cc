#include "gtest/gtest.h"
#include "mime_types.h"

class mimeTypesFixture : public ::testing::Test
{
  
};

TEST_F(mimeTypesFixture, gifTest)
{
  // set the extension type and mime type
  std::string extension = "gif";

  bool success = (extension_to_type(extension) == "image/gif");

  EXPECT_TRUE(success);
}

TEST_F(mimeTypesFixture, htmTest)
{
  // set the extension type and mime type
  std::string extension = "htm";

  bool success = (extension_to_type(extension) == "text/html");

  EXPECT_TRUE(success);
}

TEST_F(mimeTypesFixture, htmlTest)
{
  // set the extension type and mime type
  std::string extension = "html";

  bool success = (extension_to_type(extension) == "text/html");

  EXPECT_TRUE(success);
}

TEST_F(mimeTypesFixture, jpgTest)
{
  // set the extension type and mime type
  std::string extension = "jpg";

  bool success = (extension_to_type(extension) == "image/jpeg");

  EXPECT_TRUE(success);
}

TEST_F(mimeTypesFixture, pngTest)
{
  // set the extension type and mime type
  std::string extension = "png";

  bool success = (extension_to_type(extension) == "image/png");

  EXPECT_TRUE(success);
}

TEST_F(mimeTypesFixture, txtTest)
{
  // set the extension type and mime type
  std::string extension = "txt";

  bool success = (extension_to_type(extension) == "text/plain");

  EXPECT_TRUE(success);
}

TEST_F(mimeTypesFixture, jpegTest)
{
  // set the extension type and mime type
  std::string extension = "jpeg";

  bool success = (extension_to_type(extension) == "image/jpeg");

  EXPECT_TRUE(success);
}

TEST_F(mimeTypesFixture, pdfTest)
{
  // set the extension type and mime type
  std::string extension = "pdf";

  bool success = (extension_to_type(extension) == "application/pdf");

  EXPECT_TRUE(success);
}

TEST_F(mimeTypesFixture, zipTest)
{
  // set the extension type and mime type
  std::string extension = "zip";

  bool success = (extension_to_type(extension) == "application/zip");

  EXPECT_TRUE(success);
}

TEST_F(mimeTypesFixture, defaultTest)
{
  // set the extension type and mime type
  std::string extension = "I don't know what type it is";

  bool success = (extension_to_type(extension) == "text/plain");

  EXPECT_TRUE(success);
}