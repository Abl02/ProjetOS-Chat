#include <cstdio>
#include <fcntl.h>
#include <memory>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <ostream>
#include <utility>

#include "chat.hpp"
#include "utils.hpp"
#include "signaux.hpp"

using namespace ERROR;

// TODO: Revoire tout le error handling
// TODO: add implementation pour destructor
//   detruire les pipes
//   desaloué les ptr
// TODO: add render function
// TODO: implementation pour --bot --manuel
// TODO: pipes envoie une copie du message recue sur cin
// TODO: Shared memory
//   Le process send doit stocker les message envoyé sur la memoire partagé
//   pareil pour le process receiver
// TODO: ptr to chatter

Chat::Chat(std::string sender, std::string receiver, bool manual_mode)
    : arg_(std::make_unique<Args>(sender, receiver, 0, manual_mode)),
      sendPath_("/tmp/" + arg_->SENDER_NAME + "-" + arg_->RECEIVER_NAME + ".chat"),
      recvPath_("/tmp/" + arg_->RECEIVER_NAME + "-" + arg_->SENDER_NAME + ".chat") {
  if (createPipes() == 0) {
    startProcess();
  }
}

Chat::Chat(std::unique_ptr<Args> arg)
    : arg_(std::move(arg)),parentPid_(getpid()), recvPid_(-1),
      sendPath_("/tmp/" + arg_->SENDER_NAME + "-" + arg_->RECEIVER_NAME + ".chat"),
      recvPath_("/tmp/" + arg_->RECEIVER_NAME + "-" + arg_->SENDER_NAME + ".chat") {
      SetChatInstance(*this);
  if (createPipes() == 0) {
    startProcess();  
  }
}

Chat::~Chat() {
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


int Chat::sendMsg() {
  // TODO: modify implementation (optimisation, storing of msg, error handling)
  if (!open_) return 1;
  int fd = open(sendPath_.c_str(), O_WRONLY);
  if (fd == -1) {
    perror("Failed to open");
    return -1;
  }
  if (arg_->MANUAL_MODE) {
    afficheMessageEnAttente();
  }
  std::cin.getline(wbuffer_, BUFFER_LENGTH);
  std::cout << " ["
            << "\x1B[4m" << arg_->SENDER_NAME << "\x1B[0m"
            << "] " << wbuffer_ << std::endl;
  ssize_t r = write(fd, wbuffer_, BUFFER_LENGTH);
  close(fd);
  if (r < 0) {
    perror("parent::write()");
    open_ = 0;
    return -1;
  }
  return 0;
}

int Chat::receiveMsg() {
  // TODO: modify implementation (optimisation, storing of msg, error handling)
  if (!open_) return 1;
  int fd = open(recvPath_.c_str(), O_RDONLY);
  if (fd == -1) {
    perror("Failed to open");
    return -1;
  }
  ssize_t r = read(fd, rbuffer_, BUFFER_LENGTH);
  if (arg_->MANUAL_MODE) {
    std::cout << '\a' << std::flush;
    std::string message(rbuffer_); 
    shared_memory_.add_message(message);
  } else {
    std::cout << " [" << "\x1B[4m" << arg_->RECEIVER_NAME << "\x1B[0m" << "] " << rbuffer_ << std::endl;
  }
  close(fd);
  if (r == -1) {
    perror("child::read()");
    open_ = 0;
  }
  return 0;
}

int Chat::createPipes() {
  // TODO: Check error
  // TODO: Implementation de semaphore binaire pour attendre la creation du
  //   pipe
  if (mkfifo(sendPath_.c_str(), 0666) == -1) {
    perror("mkfifo");
  }
  struct stat buffer;   
  while (stat (recvPath_.c_str(), &buffer) != 0) {
    sleep(1); // Temporraire a remplacer par un semaphore binaire
  }
  open_ = true;
  return 0;
}

int Chat::startProcess() {
  recvPid_ = fork();
  if (recvPid_ == 0) {
    // Receiver (child) process
    while (open_) {
      receiveMsg();
    }
  } else if (recvPid_ > 0) {
    // Sender (parent) process
    while (open_) {
      sendMsg();
    }
  } else {
    std::cerr << FORK_ERROR << std::endl;
    perror("fork()");
    return 1;
  }
  return 0;
}

void Chat::afficheMessageEnAttente() {
  std::vector<std::string> messages = shared_memory_.get_messages();
   if (messages.empty()) {
        return;
    }
  std::cout << "Messages en attente :\n";
    for (const auto& message : messages) {
        std::cout << message << "\n";
    }
    shared_memory_.reset_memory();
}