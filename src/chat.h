#ifndef CHAT_H
#define CHAT_H

#include <sched.h>

#include <string>

#include "chatter.h"

#define BUFFER_LENGTH 80

class Chatter;

struct message {
  std::string text;
};

class Chat {
 public:
  Chat(Chatter sender, Chatter receiver);
  ~Chat();

 private:
  int sendMsg();
  int receiveMsg();

  int createPipes();
  int startProcess();

  Chatter sender_;
  Chatter receiver_;
  bool open_;

  std::string sendPipe_;
  std::string recvPipe_;
  pid_t recvPid_;

  char rbuffer_[BUFFER_LENGTH];
  char wbuffer_[BUFFER_LENGTH];
};

#endif
