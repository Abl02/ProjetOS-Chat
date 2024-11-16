#ifndef CHAT_H
#define CHAT_H

#include <string>

#include "chatter.hpp"

const int BUFFER_LENGTH = 80;

class Chatter;

class Chat {
 public:
  Chat(Chatter sender, Chatter receiver);
 private:
  int sendMsg();
  int receiveMsg();
  int createPipes();
  int startProcess();

  Chatter sender_;
  Chatter receiver_;
  bool open_;

  std::string sendPath_;
  std::string recvPath_;
  pid_t recvPid_;

  char rbuffer_[BUFFER_LENGTH];
  char wbuffer_[BUFFER_LENGTH];
};

#endif
