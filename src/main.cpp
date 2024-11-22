#include <iostream>
#include <memory>
#include <string>
#include <utility>

#include "args.hpp"
#include "chat.hpp"
#include "utils.hpp"
#include "signals.hpp"

using namespace ERROR;

int main(int argc, char* argv[]) {
  SetupSignaux();
  int errStatus{};
  std::string errMsg;
  std::unique_ptr<Args> arg = std::make_unique<Args>(parseArgs(argc, argv, &errStatus, &errMsg));
  switch (errStatus) {
    case 0:  // No error
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
      std::cerr << UNKNOWN_OPTION(errMsg) << std::endl
                << MORE_INFO << std::endl;
      return 4;
  }
  Chat chat(std::move(arg));
  return 0;
}
