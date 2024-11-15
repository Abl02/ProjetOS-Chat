#include "utils.h"

namespace ERROR {
// Error message
const std::string SENDER_PIPE_ERROR =
    "Error pipe(): Failed to create sender pipe";
const std::string RECEIVER_PIPE_ERROR =
    "Error pipe(): Failed to create receiver pipe";
const std::string FORK_ERROR = "Error fork(): Failed to fork child process";
const std::string MISSING_ARGUMENT =
    "Error: missing argument, Usage: chat <username> <destination>";
const std::string MORE_INFO = "try 'chat --help' for more information";
const std::string INVALID_OPTION(std::string opt) {
  return "Error: invalid option '" + opt + "'\n";
};
}  // namespace ERROR

