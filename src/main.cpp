#include <iostream>
#include <memory>
#include <string>
#include <utility>

#include "args.hpp"
#include "chat.hpp"
#include "signals.hpp"
#include "utils.hpp"

int main(int argc, char* argv[]) {
  using namespace ERROR;
  SetupSignals();
  int errStatus{};
  std::string errMsg;
  std::unique_ptr<Args> arg =
      std::make_unique<Args>(parseArgs(argc, argv, &errStatus, &errMsg));
  switch (errStatus) {
    case 0:  // No error
      break;
    case 1:
      std::cerr << MISSING_ARGUMENT << std::endl;
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
  try {
  Chat chat(std::move(arg));
  } catch (const std::runtime_error& err) {
    std::cerr << "Chat: " << err.what() << std::endl;
    return 5;
  }
  return 0;
}
