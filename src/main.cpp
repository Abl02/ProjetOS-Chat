#include <cstdio>
#include <cstring>
#include <iostream>

#include "args.hpp"
#include "chat.hpp"
#include "utils.hpp"

using namespace ERROR;

// TODO: Implementation des signaux (je sais pas ou le mettre)

int main(int argc, char* argv[]) {
  int errStatus{};std::string errMsg; 
  Args arg = parseArgs(argc, argv, &errStatus, &errMsg);
  switch (errStatus) {
    case 0: // No error
      break;
    case 1:
      std::cerr << MISSING_ARGUMENT << std::endl << MORE_INFO << std::endl;
      return 1;
    case 2:
      std::cerr << INVALID_USERNAME_LENGTH << std::endl;
      return 2;
    case 3:
      std::cerr << INVALID_USERNAME_CHARACTER << std::endl;
      return 3;
    case 4:
      std::cerr << UNKNOWN_OPTION(errMsg) << std::endl << MORE_INFO << std::endl;
      return 4;
    case 5:
      showHelp();
      return 0;
  }
  std::string usr1(arg.SENDER_NAME);
  std::string usr2(arg.RECEIVER_NAME);
  Chat chat(usr1, usr2);
  return 0;
}
