#include <iostream>

#include "utils.hpp"

const std::string ERROR::UNKNOWN_OPTION(std::string opt) {
  return "Error: Unknown option '" + opt + "'\n";
}

void showHelp() {
  std::cout << "Usage: chat pseudo_utilisateur pseudo_destinataire [--bot] [--manuel]\n"
            << "\n"
            << "Arguments:\n"
            << "  username          First username\n"
            << "  destination       Second username (destination)\n"
            << "\n"
            << "Options:\n"
            << "  -h, --help         Print this help\n"
            << "  -b, --bot          Enable bot mode. Disables nickname highlighting and message display.\n"
            << "  -m, --manuel       Enable manual mode. Hides incoming messages until triggered\n";
}
