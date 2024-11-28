#ifndef UTILS_H
#define UTILS_H

#include <string>

namespace ANSI_CODE {
const std::string RESET = "\033[0m";              // Default
const std::string UNDERLINE = "\033[4m";          // Underline
const std::string GREEN = "\033[32m";             // Green
const std::string BLUE = "\033[34m";              // Blue
const std::string BOLDGREEN = "\033[1m\033[32m";  // Bold Green
const std::string BOLDBLUE = "\033[1m\033[34m";   // Bold Blue
const std::string BOLDWHITE = "\033[1m\033[37m";  // Bold White
}  // namespace ANSI_CODE

namespace ERROR {  // Error message
inline const std::string SENDER_PIPE_ERROR =
    "Error pipe(): Failed to create sender pipe.\n";
inline const std::string RECEIVER_PIPE_ERROR =
    "Error pipe(): Failed to create receiver pipe.\n";
inline const std::string FORK_ERROR =
    "Error fork(): Failed to fork child process.\n";
inline const std::string MISSING_ARGUMENT =
    "chat pseudo_utilisateur pseudo_destinataire [--bot] [--manuel]\n";
inline const std::string INVALID_USERNAME_CHARACTER =
    "chat: The usernames must not contain the following characters: '/', '-', "
    "'[', or ']'.\n";
inline const std::string INVALID_USERNAME_LENGTH =
    "chat: The length of the usernames must be under 30 characters.\n";
inline const std::string MORE_INFO =
    "Try 'chat --help' for more information.\n";
std::string UNKNOWN_OPTION(const std::string &opt);
}  // namespace ERROR

void showHelp();

#endif
