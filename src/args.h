#ifndef OPT_H
#define OPT_H

#include <string>

struct Args {
  const std::string SENDER_NAME;
  const std::string RECEIVER_NAME;
  const bool BOT_MODE;
  const bool MANUAL_MODE;
  ~Args();
  // int ARG_ERROR;
};

void showHelp();

Args parseArgs(int argc, char* argv[]);

#endif
