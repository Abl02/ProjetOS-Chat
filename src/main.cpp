#include <cstdio>
#include <cstring>
#include <iostream>

#include "args.hpp"
#include "chat.hpp"
#include "chatter.hpp"
#include "utils.hpp"

using namespace ERROR;

// TODO: Implementation des signaux (je sais pas ou le mettre)

int main(int argc, char* argv[]) {
  Args arg = parseArgs(argc, argv);
  switch (arg.error.status) {
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
      std::cerr << INVALID_OPTION(arg.error.message) << std::endl << MORE_INFO << std::endl;
      return 4;
    case 5:
      showHelp();
      return 0;
  }
  Chatter usr1(arg.SENDER_NAME);
  Chatter usr2(arg.RECEIVER_NAME);
  Chat chat(usr1, usr2);
  return 0;
}
