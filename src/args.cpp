#include <cstdlib>
#include <cstring>
#include <string>

#include "args.hpp"
#include "utils.hpp"

Args::Args(std::string sender, std::string receiver, bool bot, bool manual)
    : SENDER_NAME(sender),
      RECEIVER_NAME(receiver),
      BOT_MODE(bot),
      MANUAL_MODE(manual){};

Args::Args()
    : SENDER_NAME(""), RECEIVER_NAME(""), BOT_MODE(false), MANUAL_MODE(false){};

Args::~Args(){};

/* Check if --help option is present in first position. */
bool isHelpRequest(int argc, char* argv[]) {
  return argc > 1 && (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help"));
}

/**
 * Processes command-line options and sets flags for bot and manual modes.
 *
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line argument.
 * @param bot Reference to a boolean flag indicating bot mode.
 * @param manual Reference to a boolean flag indicating manual mode.
 * @return The index of the first invalid option. Returns 0 if all options are
 * valid.
 */
int processOptions(int argc, char* argv[], bool& bot, bool& manual) {
  for (int i = 3; i < argc; ++i) {
    char* opt = argv[i];
    if (!strcmp(opt, "-b") || !strcmp(opt, "--bot")) {
      bot = true;
    }
    else if (!strcmp(opt, "-m") || !strcmp(opt, "--manuel")) {
      manual = true;
    }
    else if (!strcmp(opt, "-h") || !strcmp(opt, "--help")) {
      continue;
    }
    else {
      return i;
    }
  }
  return 0;
}

/**
 * Checks if the username has a valid length and does not contain any invalid
 * characters.
 *
 * @param name The username to check.
 * @param err Pointer to integer to store error status.
 * @return 'true' if the username is valid, 'false' otherwise.
 */
bool isValidName(const std::string &name, int* err) {
  if (name.length() > MAX_NAME_LENGTH) {
    if (err) *err = 2;  // Status 2: Invalid Length
    return false;
  }
  if (name.find_first_of(INVALID_CHARS) != std::string::npos) {
    if (err) *err = 3;  // Status 3: Invalid char
    return false;
  }
  return true;
}

Args parseArgs(int argc, char* argv[], int* err, std::string* msg) {
  if (isHelpRequest(argc, argv)) {
    showHelp();
    exit(0);  // Help request
  }
  if (argc < 3) {
    if (err) *err = 1;  // Status 1: Missing Argument
    return {};
  }
  std::string senderName = std::string(argv[1]);
  std::string receiverName = std::string(argv[2]);
  bool bot = false;
  bool manual = false;
  if (!isValidName(senderName, err)) return {};
  if (!isValidName(receiverName, err)) return {};
  if (argc > 3) {
    int index = processOptions(argc, argv, bot, manual);
    if (index != 0) {
      if (err) *err = 4;  // Status 4: Invalid Option
      if (msg) *msg = std::string(argv[index]);
    }
  }
  return {senderName, receiverName, bot, manual};
}
