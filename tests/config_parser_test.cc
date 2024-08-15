#include "gtest/gtest.h"
#include "config_parser.h"

TEST(NginxConfigParserTest, SimpleConfig) {
  NginxConfigParser parser;
  NginxConfig out_config;

  bool success = parser.Parse("example_config", &out_config);

  EXPECT_TRUE(success);
}

class NginxTestFixture : public ::testing::Test {
  protected:
    NginxConfigParser parser;
    NginxConfig out_config;
};

// The end of a quoted token should be followed by whitespace.
TEST_F(NginxTestFixture, WhitespaceAfterQuote) {
  bool success = parser.Parse("example_config1", &out_config);
  EXPECT_FALSE(success);
}

// Test for backslash-escaping within strings.
TEST_F(NginxTestFixture, BackslashEscaping) {
  bool success = parser.Parse("example_config2", &out_config);
  EXPECT_TRUE(success);
}

// Test parsing of nested block directives.
TEST_F(NginxTestFixture, NestedBlocks) {
  bool success = parser.Parse("example_config3", &out_config);
  EXPECT_TRUE(success);
}

// Test parsing of empty blocks.
TEST_F(NginxTestFixture, EmptyBlocks) {
  bool success = parser.Parse("example_config4", &out_config);
  EXPECT_TRUE(success);
}

// Ensure that all blocks that are opened are closed.
TEST_F(NginxTestFixture, ClosedBlocks) {
  bool success = parser.Parse("example_config5", &out_config);
  EXPECT_FALSE(success);
}

// Test parser's GetPortNum function.
TEST_F(NginxTestFixture, ExtractPort) {
  bool success = parser.Parse("example_config6", &out_config);
  int port = out_config.getPortNum();
  bool match = (port == 80);
  EXPECT_TRUE(match);
}

// Test GetPortNum with a Child Block
TEST_F(NginxTestFixture, ChildBlockPort) {
  bool success = parser.Parse("example_config", &out_config);
  int port = out_config.getPortNum();
  bool match = (port == 80);
  EXPECT_TRUE(match);
}

// Test GetPortNum with no port number
TEST_F(NginxTestFixture, NoPortNum) {
  bool success = parser.Parse("example_config7", &out_config);
  int port = out_config.getPortNum();
  bool match = (port == -1);
  EXPECT_TRUE(match);
}

// Test GetPortNum with out of range port number
TEST_F(NginxTestFixture, BigPortNum) {
  bool success = parser.Parse("example_config8", &out_config);
  int port = out_config.getPortNum();
  bool match = (port == -1);
  EXPECT_TRUE(match);
}

// Massive Config to test multiple branches
TEST_F(NginxTestFixture, Branches) {
  bool success = parser.Parse("example_config9", &out_config);
  EXPECT_TRUE(success);
}

// Test with a bogus config file
TEST_F(NginxTestFixture, BogusFile) {
  bool success = parser.Parse("example_config?", &out_config);
  EXPECT_FALSE(success);
}

// Test the ToString Function
TEST_F(NginxTestFixture, ToString) {
  bool success = parser.Parse("example_config6", &out_config);
  std::string parsed_string(out_config.ToString().c_str());
  std::string expected_string = "listen 80;\n";
  bool match = (parsed_string == expected_string);
  EXPECT_TRUE(match);
}

// Test for parsing of simple paths, including location and root.
TEST_F(NginxTestFixture, SimplePathParse) {
  parser.Parse("example_config10", &out_config);
  std::vector<path> paths = out_config.getPaths();

  bool match1 = (paths[0].type == static_ && 
                 paths[0].endpoint == "/static1/" && 
                 paths[0].root == "/usr/bin");
  bool match2 = (paths[1].type == static_ && 
                 paths[1].endpoint == "/static2/" && 
                 paths[1].root == "/foo/bar");
  bool match3 = (paths[2].type == echo && 
                 paths[2].endpoint == "/echo" &&
                 paths[2].root == "");

  bool match = (match1 && match2 && match3);
  EXPECT_TRUE(match);
}