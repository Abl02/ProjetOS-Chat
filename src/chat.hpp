#ifndef CHAT_H
#define CHAT_H

#include <cstddef>
#include <memory>
#include <semaphore.h>
#include <string>
#include "args.hpp"
#include "sharedmem.hpp" 

const int BUFFER_LENGTH = 80;

struct Args;

class Chat {
 public:
  Chat(const std::string &sender, const std::string &receiver, bool botMode, bool manualMode);
  Chat(std::unique_ptr<Args> arg);
  ~Chat();

  void afficheMessageEnAttente();

  pid_t getParentPid();
  pid_t getSecondProcessPID();
  bool arePipesOpened();
  bool isManualMode();

 private:
  void showMsg(const std::string& name, const std::string& message);
  int sendMsg();
  int receiveMsg();

  size_t generateID();
  int initSemaphore();
  int syncPipe();
  int createPipe();
  int startProcess();

  std::unique_ptr<Args> arg_;

  SharedMemory shared_memory_;
  size_t chatID_;
  std::string semName_;
  sem_t* syncSem_;
  bool open_;
  std::string sendPath_;
  std::string recvPath_;
  pid_t parentPid_;
  pid_t recvPid_;

  char rbuffer_[BUFFER_LENGTH];
  char wbuffer_[BUFFER_LENGTH];
};

#endif
