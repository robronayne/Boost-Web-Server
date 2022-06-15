#include "http/path.h"
#include "gtest/gtest.h"
#include "config_parser.h"

TEST(NginxConfigParserTest, SimpleConfig) 
{
  NginxConfigParser parser;
  NginxConfig out_config;

  bool success = parser.Parse("test_config/example_config", &out_config);

  EXPECT_TRUE(success);
}

class NginxTestFixture : public ::testing::Test 
{
  protected:
    NginxConfigParser parser;
    NginxConfig out_config;
};

// The end of a quoted token should be followed by whitespace.
TEST_F(NginxTestFixture, WhitespaceAfterQuote) 
{
  bool success = parser.Parse("test_config/example_config1", &out_config);
  EXPECT_FALSE(success);
}

// Test for backslash-escaping within strings.
TEST_F(NginxTestFixture, BackslashEscaping) 
{
  bool success = parser.Parse("test_config/example_config2", &out_config);
  EXPECT_TRUE(success);
}

// Test parsing of nested block directives.
TEST_F(NginxTestFixture, NestedBlocks) 
{
  bool success = parser.Parse("test_config/example_config3", &out_config);
  EXPECT_TRUE(success);
}

// Test parsing of empty blocks.
TEST_F(NginxTestFixture, EmptyBlocks) 
{
  bool success = parser.Parse("test_config/example_config4", &out_config);
  EXPECT_TRUE(success);
}

// Ensure that all blocks that are opened are closed.
TEST_F(NginxTestFixture, ClosedBlocks) 
{
  bool success = parser.Parse("test_config/example_config5", &out_config);
  EXPECT_FALSE(success);
}

// Test parser's GetPortNum function.
TEST_F(NginxTestFixture, ExtractPort) 
{
  bool success = parser.Parse("test_config/example_config6", &out_config);
  int port = out_config.getPortNum();
  bool match = (port == 80);
  EXPECT_TRUE(match);
}

// Test GetPortNum with a Child Block
TEST_F(NginxTestFixture, ChildBlockPort) 
{
  bool success = parser.Parse("test_config/example_config", &out_config);
  int port = out_config.getPortNum();
  bool match = (port == 80);
  EXPECT_TRUE(match);
}

// Test GetPortNum with no port number
TEST_F(NginxTestFixture, NoPortNum) 
{
  bool success = parser.Parse("test_config/example_config7", &out_config);
  int port = out_config.getPortNum();
  bool match = (port == -1);
  EXPECT_TRUE(match);
}

// Test GetPortNum with out of range port number
TEST_F(NginxTestFixture, BigPortNum) 
{
  bool success = parser.Parse("test_config/example_config8", &out_config);
  int port = out_config.getPortNum();
  bool match = (port == -1);
  EXPECT_TRUE(match);
}

// Massive config to test multiple branches
TEST_F(NginxTestFixture, Branches) 
{
  bool success = parser.Parse("test_config/example_config9", &out_config);
  EXPECT_TRUE(success);
}

// Test with a bogus config file
TEST_F(NginxTestFixture, BogusFile) 
{
  bool success = parser.Parse("test_config/example_config?", &out_config);
  EXPECT_FALSE(success);
}

// Test the ToString Function
TEST_F(NginxTestFixture, ToString) 
{
  bool success = parser.Parse("test_config/example_config6", &out_config);
  std::string parsed_string(out_config.ToString().c_str());
  std::string expected_string = "port 80;\n";
  bool match = (parsed_string == expected_string);
  EXPECT_TRUE(match);
}

// Test for parsing of simple paths, including location and root.
TEST_F(NginxTestFixture, SimplePathParse) 
{
  parser.Parse("test_config/example_config10", &out_config);
  std::vector<path> paths = out_config.getPaths();

  bool echo_match = (paths[0].type == echo &&
                     paths[0].endpoint == "/echo");

  bool static_match = (paths[1].type == static_ &&
                       paths[1].endpoint == "/static" &&
                       paths[1].info_map["root"] == "./files");

  bool api_match = (paths[2].type == api_ &&
                       paths[2].endpoint == "/api" &&
                       paths[2].info_map["data_path"] == "./files");

  bool health_match = (paths[3].type == health &&
                       paths[3].endpoint == "/health");

  bool sleep_match = (paths[4].type == sleep_ &&
                      paths[4].endpoint == "/sleep");

  bool match = (echo_match && static_match && api_match && health_match && sleep_match);
  EXPECT_TRUE(match);
}

// Test with a missing semicolon.
TEST_F(NginxTestFixture, MissingSemicolon)
{
  bool success = parser.Parse("test_config/example_config11", &out_config);
  EXPECT_FALSE(success);
}

// Test good URL with trailing slashes, and one URL with single slash,
// as well as one bad URL.
TEST_F(NginxTestFixture, GoodURL)
{
  parser.Parse("test_config/example_config12", &out_config);
  std::vector<path> paths = out_config.getPaths();
  
  bool trailing_slash = (paths[0].endpoint == "/trailing");
  bool invalid_url = (paths[1].endpoint == "ERROR");
  bool single_slash = (paths[2].endpoint == "/");

  bool match = (trailing_slash && single_slash && invalid_url);
  EXPECT_TRUE(match);
}