#include "chat.hpp"

#include <fcntl.h>
#include <semaphore.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <algorithm>
#include <csignal>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <functional>
#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <utility>

#include "signals.hpp"
#include "utils.hpp"

Chat::Chat(const std::string& sender, const std::string& receiver, bool botMode,
           bool manualMode)
    : arg_(std::make_unique<Args>(sender, receiver, botMode, manualMode)),
      sendPath_("/tmp/" + arg_->SENDER_NAME + "-" + arg_->RECEIVER_NAME +
                ".chat"),
      recvPath_("/tmp/" + arg_->RECEIVER_NAME + "-" + arg_->SENDER_NAME +
                ".chat"),
      parentPid_(getpid()),
      recvPid_(-1),
      open_(false) {
  SetChatInstance(*this);
  chatID_ = generateID();
  if (initSemaphore() != 0) throw std::runtime_error("Error during semaphore creation.");
  if (createPipe() != 0) throw std::runtime_error("Error during pipe creation.");
  if (startProcess() != 0) 
  switch (startProcess()) {
    case 1:
      throw std::runtime_error("Error during fork()");
      break;
    case 2:
      throw std::runtime_error("Error during open()");
      break;
    case 3:
      throw std::runtime_error("Error during read() and write()");
      break;
    default:
      break;
  }
}

Chat::Chat(std::unique_ptr<Args> arg)
    : arg_(std::move(arg)),
      sendPath_("/tmp/" + arg_->SENDER_NAME + "-" + arg_->RECEIVER_NAME +
                ".chat"),
      recvPath_("/tmp/" + arg_->RECEIVER_NAME + "-" + arg_->SENDER_NAME +
                ".chat"),
      parentPid_(getpid()),
      recvPid_(-1),
      open_(false) {
  SetChatInstance(*this);
  chatID_ = generateID();
  initSemaphore();
  if (initSemaphore() != 0) throw std::runtime_error("Error during semaphore creation.");
  if (createPipe() != 0) throw std::runtime_error("Error during pipe creation.");
  if (startProcess() != 0) 
  switch (startProcess()) {
    case 1:
      throw std::runtime_error("Error during fork()");
      break;
    case 2:
      throw std::runtime_error("Error during open()");
      break;
    case 3:
      throw std::runtime_error("Error during read() and write()");
      break;
    default:
      break;
  }
}

Chat::~Chat() {
  remove(sendPath_.c_str());
  sem_close(syncSem_);
  sem_unlink(semName_.c_str());
}

pid_t Chat::getParentPid() { return parentPid_; }

pid_t Chat::getSecondProcessPID() { return recvPid_; }

bool Chat::arePipesOpened() { return open_; }

bool Chat::isManualMode() { return arg_->MANUAL_MODE; }

bool Chat::isBotMode() { return arg_->BOT_MODE; }

void Chat::DestroyFileDescriptors() {
  close(Parent_Write_fd);
  close(Child_Read_fd);
}

void Chat::DestroyPipe() { unlink(sendPath_.c_str()); }

void Chat::showMsg(const std::string& name, const std::string& message) {
  using namespace ANSI_CODE;
  if (!arg_->BOT_MODE) {
    std::string color{};
    if (name == arg_->SENDER_NAME) color = BOLDBLUE;
    if (name == arg_->RECEIVER_NAME) color = BOLDGREEN;
    std::cout << color << "[" << UNDERLINE << name << RESET << color << "] "
              << RESET << message << std::endl;
  } else {
    std::cout << "[" << name << "] " << message << std::endl;
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
  std::string message;
  while (std::getline(std::cin, message)) {
    size_t size = strlen(message.c_str()) + 1;
    ssize_t bytesWritten;
    bytesWritten = write(Parent_Write_fd, &size, sizeof(size_t));
    if (bytesWritten < 0) {
      perror("Write failed");
      close(Parent_Write_fd);
      return -1;
    }
    bytesWritten = write(Parent_Write_fd, message.c_str(), size);
    if (bytesWritten < 0) {
      perror("Write failed");
      close(Parent_Write_fd);
      return -1;
    }
    if (!arg_->BOT_MODE) {
      showMsg(arg_->SENDER_NAME, message);
    }
    if (arg_->MANUAL_MODE) {
      afficheMessageEnAttente();
    }
  }
  return 0;
}

int Chat::receiveMsg() {
  size_t size;
  ssize_t bytesRead;
  std::string message;
  while ((bytesRead = read(Child_Read_fd, &size, sizeof(size_t))) > 0) {
    char* buffer = new char[size];
    bytesRead = read(Child_Read_fd, buffer, size);
    if (bytesRead < 0) {
      delete[] buffer;
      perror("Read failed");
      close(Child_Read_fd);
      return -1;
    }
    message = buffer;
    if (arg_->MANUAL_MODE) {
      bool IsEnoughSpace;
      IsEnoughSpace = shared_memory_.add_message(message);
      if (!IsEnoughSpace) {
        afficheMessageEnAttente();
        shared_memory_.add_message(message);
      }
    } else {
      showMsg(arg_->RECEIVER_NAME,message);
    }
    delete[] buffer;
  }
  if (bytesRead == -1) {
    perror("Read failed");
    close(Child_Read_fd);
    return -1;
  }
  return 0;
}

size_t Chat::generateID() {
  std::string min_str = std::min(arg_->SENDER_NAME, arg_->RECEIVER_NAME);
  std::string max_str = std::max(arg_->SENDER_NAME, arg_->RECEIVER_NAME);
  return std::hash<std::string>{}(min_str + max_str);
}

int Chat::initSemaphore() {
  semName_ = "/" + std::to_string(chatID_) + ".chat";
  if ((syncSem_ = sem_open(semName_.c_str(), O_CREAT, 0644, 1)) == SEM_FAILED) {
    std::cerr << "sem_open: " << std::strerror(errno) << std::endl;
    return -1;
  }
  return 0;
}

int Chat::syncPipe() {
  struct stat buffer;
  if (stat(recvPath_.c_str(), &buffer) != 0) {
    if (sem_init(syncSem_, 1, 0) == -1) {
      std::cerr << "sem_init: " << std::strerror(errno) << std::endl;
      return -1;
    }
    if (sem_wait(syncSem_) == -1) {
      std::cerr << "sem_wait: " << std::strerror(errno) << std::endl;
      return -1;
    }
  }
  if (sem_post(syncSem_) == -1) {
    std::cerr << "sem_post: " << std::strerror(errno) << std::endl;
    return -1;
  }
  return 0;
}

int Chat::createPipe() {
  if (mkfifo(sendPath_.c_str(), 0666) == -1) {
    std::cerr << "mkfifo: " << std::strerror(errno) << std::endl;
  }
  if (syncPipe() == 0) {
    open_ = true;
  } else {
    return -1;
  }
  return 0;
}

int Chat::startProcess() {
  recvPid_ = fork();
  if (recvPid_ == 0) {
    // Receiver (child) process
    if (open_) {
      Child_Read_fd = open(recvPath_.c_str(), O_RDONLY);
      if (Child_Read_fd == -1) {
        std::cerr << "open: " << std::strerror(errno) << std::endl;
        return 2;
      }
      if (receiveMsg() == -1) {
        return 3;
      }
    }
  } else if (recvPid_ > 0) {
    // Sender (parent) process
    if (open_) {
      Parent_Write_fd = open(sendPath_.c_str(), O_WRONLY);
      if (Parent_Write_fd == -1) {
        std::cerr << "open: " << std::strerror(errno) << std::endl;
        return 2;
      }
      if (sendMsg() == -1) {
        return 3;
      }
    }
    kill(recvPid_, SIGTERM);
  } else {
    std::cerr << "fork: " << std::strerror(errno) << std::endl;
    return 1;
  }
  return 0;
}
