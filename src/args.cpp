#include <cstring>
#include <string>

#include "args.hpp"

Args::Args(std::string sender, std::string receiver, bool bot, bool manual, int status)
  :SENDER_NAME(sender),RECEIVER_NAME(receiver),BOT_MODE(bot), MANUAL_MODE(manual), error({status,""}){};

Args::Args(int status)
  :SENDER_NAME(""),RECEIVER_NAME(""),BOT_MODE(false), MANUAL_MODE(false),error({status,""}) {};

Args::Args(int status, std::string message)
  :SENDER_NAME(""),RECEIVER_NAME(""),BOT_MODE(false), MANUAL_MODE(false),error({status,message}) {};

Args::~Args() {};

// check if --help option is present in first position
bool isHelpRequest(int argc, char* argv[]) {
  return argc > 1 && (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help"));
}

int processOptions(int argc, char* argv[], bool& bot, bool& manual) {
  // return the index of the first found invalid option
  for (int i = 3; i < argc; ++i) {
    char* opt = argv[i];
    if (!strcmp(opt, "-b") || !strcmp(opt, "--bot"))
      bot = 1;
    else if (!strcmp(opt, "-m") || !strcmp(opt, "--manuel"))
      manual = 1;
    else if (!strcmp(opt, "-h") || !strcmp(opt, "--help"))
      continue;
    else return i;
  }
  return 0;
}

Args parseArgs(int argc, char* argv[]) {
  if (isHelpRequest(argc,argv)) return {5}; // Status 5: Help Request
  if (argc < 3) return {1}; // Status 1: Missing Arguments
  char* senderName = argv[1];
  char* receiverName = argv[2];
  bool bot = false;
  bool manual = false;
  // Check if usernames has valid length
  if (!(strlen(senderName) <= 30)) return {2}; // Status 2: Invalid Length
  if (!(strlen(receiverName) <= 30)) return {2};
  // Check if usernames contains invalid chars
  if (strpbrk(senderName, "/-[]") != 0) return {3}; // Status 3: Invalid Char
  if (strpbrk(receiverName, "/-[]") != 0) return {3};
  if (argc > 3) {
    int id=processOptions(argc, argv, bot, manual);
    if (id != 0) return {4,std::string(argv[id])}; // Status 4: Invalid Option
  }
  return {std::string(senderName),std::string(receiverName), bot, manual};
}
