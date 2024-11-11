#ifndef OPT_H
#define OPT_H

#include <string>

struct Args {
  const std::string USERNAME;
  const std::string DESTINATION;
  const bool IS_BOT;
  const bool IS_MANUAL;
  // int ARG_ERROR;
};

void showUsername(std::string username);

void showHelp();

Args parseArgs(int argc, char* argv[]);

#endif
