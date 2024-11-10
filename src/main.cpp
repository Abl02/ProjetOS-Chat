
#include <cstdlib>
#include <iostream>
#include <string>

const std::string MISSING_ARGUMENT = "Error: missing argument, use: chat <your_username> <other_username>";
const std::string MORE_INFO = "try 'chat --help' for more information";

struct Args {
  const std::string USERNAME;
  const std::string DESTINATION;
};

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

void showUsername(std::string username) {
  std::cout << username << std::endl;
}

Args parseArgs(int argc, char* argv[]) {
  //check if --help option is present
  if (argc > 1 && (std::string(argv[1]) == "-h" || std::string(argv[1]) == "--help")) {
    showHelp();
    exit(0);
  }

  if (argc < 3) {
    std::cerr << MISSING_ARGUMENT << std::endl << MORE_INFO << std::endl;
    exit(1);
  }

  std::string username = argv[1];
  std::string destination = argv[2];

  if (argc > 3) {
    for (int i=3;i<argc;++i) {
      if (argv[i][0] == '-') {
        if (argv[i][1] == '-') {
          // double quote options
        }
        else {
          // single quote options
        }
      }
    }
  }

  return {username,destination};
}

int main(int argc, char* argv[]) {
  Args options = parseArgs(argc, argv);
  //showUsername(options.USERNAME);
  //showUsername(options.DESTINATION);
  return 0;
}
