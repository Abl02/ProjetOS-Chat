#include <cstring>
#include <iostream>
#include <string>
#include "args.h"

const std::string MISSING_ARGUMENT = "Error: missing argument, use: chat <your_username> <other_username>";
const std::string MORE_INFO= "try 'chat --help' for more information";

const std::string INVALID_OPTION(std::string opt) {
  return "Error: invalid option '" + opt + "'\n";
}

void showUsername(std::string username) {
  std::cout << "[" << username << "]" << std::endl;
}

void showHelp() {
  std::cout
  << "Usage: chat <username> <destination> [OPTION...]\n"
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

Args parseArgs(int argc, char* argv[]) {
  //check if --help option is present
  if (argc > 1 && (std::string(argv[1]) == "-h" || std::string(argv[1]) == "--help")) {
    showHelp();
    exit(0);
  }
  // Check for arguments
  if (argc < 3) {
    std::cerr << MISSING_ARGUMENT << std::endl << MORE_INFO << std::endl;
    exit(1); // add exception
  }
  std::string username = argv[1];
  std::string destination = argv[2];
  bool bot = 0;
  bool manual = 0;
  // Check for options
  if (argc > 3) {
    for (int i=3;i<argc;++i) {
      if      (strcmp(argv[i], "-b")      == 0) bot = 1;
      else if (strcmp(argv[i], "-m")      == 0) manual = 1;
      else if (strcmp(argv[i], "--bot")   == 0) bot = 1;
      else if (strcmp(argv[i], "--manuel") == 0) manual = 1;
      else {
        std::cerr << INVALID_OPTION(argv[i]) << std::endl << MORE_INFO << std::endl;
        exit(1); // add exception
      }
    }
  }

  return {username,destination,bot,manual};
}
