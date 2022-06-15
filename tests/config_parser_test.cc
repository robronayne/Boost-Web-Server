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