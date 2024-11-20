#ifndef UTILS_H
#define UTILS_H

#include <string>

namespace COLORS {
  const std::string RESET = "\033[0m";               // Default
  const std::string BLACK = "\033[30m";              // Black
  const std::string RED = "\033[31m";                // Red
  const std::string GREEN = "\033[32m";              // Green
  const std::string YELLOW = "\033[33m";             // Yellow
  const std::string BLUE = "\033[34m";               // Blue
  const std::string MAGENTA = "\033[35m";            // Magenta
  const std::string CYAN = "\033[36m";               // Cyan
  const std::string WHITE = "\033[37m";              // White
  const std::string BOLDBLACK = "\033[1m\033[30m";   // Bold Black
  const std::string BOLDRED = "\033[1m\033[31m";     // Bold Red
  const std::string BOLDGREEN = "\033[1m\033[32m";   // Bold Green
  const std::string BOLDYELLOW = "\033[1m\033[33m";  // Bold Yellow
  const std::string BOLDBLUE = "\033[1m\033[34m";    // Bold Blue
  const std::string BOLDMAGENTA = "\033[1m\033[35m"; // Bold Magenta
  const std::string BOLDCYAN = "\033[1m\033[36m";    // Bold Cyan
  const std::string BOLDWHITE = "\033[1m\033[37m";   // Bold White
}

namespace ERROR { // Error message
  inline const std::string SENDER_PIPE_ERROR = 
    "Error pipe(): Failed to create sender pipe.\n";
  inline const std::string RECEIVER_PIPE_ERROR = 
    "Error pipe(): Failed to create receiver pipe.\n";
  inline const std::string FORK_ERROR = 
    "Error fork(): Failed to fork child process.\n";
  inline const std::string MISSING_ARGUMENT = 
    "chat pseudo_utilisateur pseudo_destinataire [--bot] [--manuel]\n";
  inline const std::string INVALID_USERNAME_CHARACTER =
    "chat: The usernames must not contain the following characters: '/', '-', '[', or ']'.\n";
  inline const std::string INVALID_USERNAME_LENGTH =
    "chat: The length of the usernames must be under 30 characters.\n";
  inline const std::string MORE_INFO = 
    "Try 'chat --help' for more information.\n";
  const std::string UNKNOWN_OPTION(std::string opt);
}

void showHelp();

#endif
