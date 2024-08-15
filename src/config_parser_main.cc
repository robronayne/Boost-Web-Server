#include "config_parser.h"

/*
 * Driver for a configuration parser object.
 * Usage: ./config_parser <path to config file>
 */
int main(int argc, char* argv[]) {
  // Ensure that the argument size matches proper call.
  if (argc != 2) {
    printf("Usage: ./config_parser <path to config file>\n");
    return 1;
  }

  NginxConfig config;
  NginxConfigParser config_parser;
  config_parser.Parse(argv[1], &config);

  // Display the output from the parsed configuration file.
  printf("%s", config.ToString().c_str());
  return 0;
}
