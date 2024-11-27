#include <algorithm>
#include <cstddef>
#include <cstdio>
#include <fcntl.h>
#include <functional>
#include <memory>
#include <string>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <ostream>
#include <utility>
#include <signal.h>
#include <stdlib.h> 
#include <semaphore.h>

#include "chat.hpp"
#include "utils.hpp"
#include "signals.hpp"

// TODO: Revoire tout le error handling
// TODO: add implementation pour destructor
//   desaloué les ptr
// TODO: implementation pour --bot --manuel
// TODO: pipes envoie une copie du message recue sur cin
// TODO: Shared memory
//   Le process send doit stocker les message envoyé sur la memoire partagé
//   pareil pour le process receiver

Chat::Chat(const std::string &sender, const std::string &receiver, bool botMode, bool manualMode)
    : arg_(std::make_unique<Args>(sender, receiver, botMode, manualMode)),
      sendPath_("/tmp/" + arg_->SENDER_NAME + "-" + arg_->RECEIVER_NAME + ".chat"),
      recvPath_("/tmp/" + arg_->RECEIVER_NAME + "-" + arg_->SENDER_NAME + ".chat") {
  chatID_ = generateID();
  initSemaphore();
  if (createPipe() == 0) {
    startProcess();
  }
}

Chat::Chat(std::unique_ptr<Args> arg)
    : arg_(std::move(arg)),
      sendPath_("/tmp/" + arg_->SENDER_NAME + "-" + arg_->RECEIVER_NAME + ".chat"),
      recvPath_("/tmp/" + arg_->RECEIVER_NAME + "-" + arg_->SENDER_NAME + ".chat"),
      parentPid_(getpid()),
      recvPid_(-1){
  SetChatInstance(*this);
  chatID_ = generateID();
  initSemaphore();
  if (createPipe() == 0) {
    startProcess();
  }
}

Chat::~Chat() {
  remove(sendPath_.c_str());
  sem_close(syncSem_);
  sem_unlink(semName_.c_str());
}

pid_t Chat::getParentPid() {
    return parentPid_;
}

pid_t Chat::getSecondProcessPID() {
    return recvPid_;
}

bool Chat::arePipesOpened() {
    return open_;
}

bool Chat::isManualMode() {
    return arg_->MANUAL_MODE;
}

void Chat::DestroyFileDescriptors(){
  close(Parent_Write_fd);
  close(Child_Read_fd);
}

void Chat::DestroyPipe(){
  unlink(sendPath_.c_str());
}

void Chat::showMsg(const std::string& name, const std::string& message) {
  using namespace ANSI_CODE;
  if (!arg_->BOT_MODE) {
    std::string color{};
    if (name == arg_->SENDER_NAME) color = BOLDBLUE;
    if (name == arg_->RECEIVER_NAME) color = BOLDGREEN;
    std::cout << color << "["
            << UNDERLINE << name << RESET
            << color << "] " << RESET << message << std::endl;
  } else {
    std::cout << "["
            << name
            << "] " << message << std::endl;
  }
}

void Chat::afficheMessageEnAttente() {
  std::vector<std::string> messages = shared_memory_.get_messages();
   if (messages.empty()) {
        return;
    }
    for (const auto& message : messages) {
        showMsg(arg_->RECEIVER_NAME, message);
    }
    shared_memory_.reset_memory();
}

int Chat::sendMsg() {
  if (Parent_Write_fd == -1) {
    perror("Failed to open");
    return -1;
  }
  std::string message{};
  std::getline(std::cin, message);
  if(!std::cin){
    DestroyFileDescriptors();
    DestroyPipe();
    exit(0);
  }
  if (!arg_->BOT_MODE) {
    showMsg(arg_->SENDER_NAME, message);
  }
  if (arg_->MANUAL_MODE) {
    afficheMessageEnAttente();
  }
  ssize_t bytesWritten = write(Parent_Write_fd, message.c_str(), message.size());
  if (bytesWritten == -1) {
    perror("Write failed");
    close(Parent_Write_fd);
    return -1;
  }
  // close(Write_fd);
  return 0;
}

int Chat::receiveMsg() {
  if (Child_Read_fd == -1) {
    perror("Failed to open");
    return -1;
  }
  std::string message{};
  char rbuffer[BUFFER_LENGTH];
  ssize_t bytesWritten = read(Child_Read_fd, rbuffer, BUFFER_LENGTH);
  if (bytesWritten == -1) {
    perror("Read failed");
    close(Child_Read_fd);
    return -1;
  }
  if(bytesWritten == 0){ 
    DestroyFileDescriptors();
    DestroyPipe();
    exit(0);
  }
  message += rbuffer;
  if (arg_->MANUAL_MODE) {
    std::cout << '\a' << std::flush;
    shared_memory_.add_message(message);
  } else {
    showMsg(arg_->RECEIVER_NAME,message);
  }
  // close(Read_fd);
  return 0;
}

size_t Chat::generateID() {
  std::string min_str = std::min(arg_->SENDER_NAME, arg_->RECEIVER_NAME);
  std::string max_str = std::max(arg_->SENDER_NAME, arg_->RECEIVER_NAME);
  return std::hash<std::string>{}(min_str+max_str);
}

int Chat::initSemaphore() {
  semName_ = "/" + std::to_string(chatID_) + ".chat";
  if ((syncSem_ = sem_open(semName_.c_str(), O_CREAT, 0644, 1)) == SEM_FAILED) {
    perror("semaphore initilization");
    return 1;
  }
  return 0;
}

int Chat::syncPipe() {
  struct stat buffer;   
  if (stat (recvPath_.c_str(), &buffer) != 0) {
    sem_init(syncSem_, 1, 0);
    int sem_value{};
    sem_getvalue(syncSem_, &sem_value);
    sem_wait(syncSem_);
    sem_getvalue(syncSem_, &sem_value);
  }
  sem_post(syncSem_);
  return 0;
}

int Chat::createPipe() {
  // TODO: Check error
  // TODO: Implementation de semaphore binaire pour attendre la creation du
  //   pipe
  if (mkfifo(sendPath_.c_str(), 0666) == -1) {
    perror("Couldn't execute mkfifo");
  }
  syncPipe();
  open_ = true;
  return 0;
}

int Chat::startProcess() {
  recvPid_ = fork();
  if (recvPid_ == 0) {
    // Receiver (child) process
    Child_Read_fd = open(recvPath_.c_str(), O_RDONLY);
    while (open_) {
      receiveMsg();
    }
  } else if (recvPid_ > 0) {
    // Sender (parent) process
    Parent_Write_fd = open(sendPath_.c_str(), O_WRONLY);
    while (open_) {
      sendMsg();
    }
  } else {
    // std::cerr << FORK_ERROR << std::endl;
    perror("fork()");
    return 1;
  }
  return 0;
}
