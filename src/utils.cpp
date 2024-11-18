#include <iostream>

#include "utils.hpp"

const std::string ERROR::INVALID_OPTION(std::string opt) {
  return "Error: invalid option '" + opt + "'\n";
}

void showHelp() {
  std::cout << "Usage: chat <username> <destination> [OPTION...]\n"
            << "\n"
            << "Arguments:\n"
            << "  username          First username\n"
            << "  destination       Second username (destination)\n"
            << "\n"
            << "Options:\n"
            << "  -h, --help         Print this help\n"
            << "  -b, --bot          Print the SVN version\n"
            << "  -m, --manuel       Print the proxy version\n";
}
