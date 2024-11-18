#ifndef CHAT_H
#define CHAT_H

#include <string>

const int BUFFER_LENGTH = 80;

class Chat {
 public:
  Chat(std::string sender, std::string receiver);
 private:
  int sendMsg();
  int receiveMsg();
  int createPipes();
  int startProcess();

  std::string sender_;
  std::string receiver_;
  bool open_;

  std::string sendPath_;
  std::string recvPath_;
  pid_t recvPid_;

  char rbuffer_[BUFFER_LENGTH];
  char wbuffer_[BUFFER_LENGTH];
};

#endif
