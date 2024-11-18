#ifndef ARG_H
#define ARG_H

#include <string>

struct argsError {
  int status;
  std::string message;
};

struct Args {
  Args(int status);
  Args(int status, std::string message);
  Args(std::string sender, std::string receiver, bool bot, bool manual, int status = 0);
  ~Args();
  const std::string SENDER_NAME;
  const std::string RECEIVER_NAME;
  const bool BOT_MODE;
  const bool MANUAL_MODE;
  argsError error;
};

Args parseArgs(int argc, char* argv[]);
 
#endif
