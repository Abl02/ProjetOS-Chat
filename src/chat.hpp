#ifndef CHAT_H
#define CHAT_H

#include <memory>
#include <string>
#include "args.hpp"
#include "sharedmem.hpp" 

const int BUFFER_LENGTH = 80;

class Args;

class Chat {
 public:
  Chat(std::string sender, std::string receiver, bool manual_mode);
  Chat(std::unique_ptr<Args> arg);
  ~Chat();

  void afficheMessageEnAttente();

  pid_t getParentPid();
  pid_t getSecondProcessPID();
  bool arePipesOpened();
  bool isManualMode();


 private:
  int sendMsg();
  int receiveMsg();
  int createPipes();
  int startProcess();

  std::unique_ptr<Args> arg_;

  bool open_;
  std::string sendPath_;
  std::string recvPath_;

  SharedMemory shared_memory_;

  pid_t parentPid_;
  pid_t recvPid_;

  char rbuffer_[BUFFER_LENGTH];
  char wbuffer_[BUFFER_LENGTH];
};

#endif
