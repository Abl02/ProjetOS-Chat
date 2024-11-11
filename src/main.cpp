#include <cstring>
#include <iostream>
#include <string>
#include "args.h"

int main(int argc, char* argv[]) {
  Args opt = parseArgs(argc, argv);
  showUsername(opt.USERNAME);
  showUsername(opt.DESTINATION);
  std::cout << opt.IS_BOT << " / " << opt.IS_MANUAL << std::endl;
  return 0;
}
