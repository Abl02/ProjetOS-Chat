#ifndef ARG_H
#define ARG_H

#include <cstddef>
#include <string>

const size_t MAX_NAME_LENGTH = 30;
const char INVALID_CHARS[] = "/-[]";

/* Container for all command-line arguments. */
struct Args {
  /* Default constructor */
  Args(std::string sender, std::string receiver, bool bot, bool manual);
  /* Constructor for empty arguments (flags set to 0 and strings set to "") */
  Args();
  ~Args();
  const std::string SENDER_NAME;
  const std::string RECEIVER_NAME;
  const bool BOT_MODE;
  const bool MANUAL_MODE;
};

/**
 * Parses command-line arguments and returns an 'Args' object containing them.
 *
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line argument.
 * @param err Optional. A pointer to an integer used to transfer error status
 *            that occurs during the parsing of the program arguments. If no
 * error handling is required, pass 'nullptr'.
 *            - status 0: Normal behaviour.
 *            - status 1: Missing argument.
 *            - status 2: Invalid username length.
 *            - status 3: Invalid character in username.
 *            - status 4: Unknown option.
 * @param msg Optional. A pointer to a 'std::string' used to transfer dynamic
 * error messages to the caller, e.g., the name of an invalid option. Pass
 *            'nullptr' if no specific error message handling is needed.
 *
 * @return An 'Args' object containing the parsed arguments.
 */
Args parseArgs(int argc, char* argv[], int* err = nullptr,
               std::string* msg = nullptr);

#endif
