#include <fcntl.h>
#include <ostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstring>
#include <iostream>

#include "chat.hpp"
#include "utils.hpp"


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

void deleteLastLine() {
  std::cout << "\033[F";
  std::cout << "\033[K";
  std::cout.flush();
}

Chat::Chat(std::string sender, std::string receiver)
  : sender_(sender), receiver_(receiver),
    sendPath_("/tmp/" + sender + "-" + receiver + ".chat"),
    recvPath_("/tmp/" + receiver + "-" + sender + ".chat") {
  if (createPipes() == 0) {
    startProcess();
  }
}

int Chat::sendMsg() {
  // TODO: modify implementation (optimisation, storing of msg, error handling)
  if (!open_) return 1;
  int fd = open(sendPath_.c_str(), O_WRONLY);
  if (fd == -1) {
    perror("Failed to open");
    return -1;
  }
  std::cin.getline(wbuffer_, BUFFER_LENGTH);
  deleteLastLine();
  std::cout << " [" <<"\x1B[4m" << sender_ << "\x1B[0m" << "] " << wbuffer_ <<std::endl;
  // Check size
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
  // Check size
  ssize_t r = read(fd, rbuffer_, BUFFER_LENGTH);
  std::cout << " [" <<"\x1B[4m" << receiver_ << "\x1B[0m" << "] "<< rbuffer_ << std::endl;
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
  mkfifo(sendPath_.c_str(), 0666);
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
