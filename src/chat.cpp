#include "chat.h"

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstring>
#include <iostream>

#include "utils.h"

using namespace ERROR;

Chat::Chat(Chatter sender, Chatter receiver)
    : sender_(sender), receiver_(receiver) {
  if (createPipes() == 0) {
    open_ = true;
    startProcess();
  }
}

int Chat::sendMsg() {
  if (!open_)
    return 1;
  else {
    int fd = open(sendPipe_.c_str(), O_WRONLY);
    std::cout << " [" << sender_.name() << "] ";
    std::cin >> wbuffer_;
    ssize_t r = write(fd, wbuffer_, BUFFER_LENGTH);
    close(fd);
    if (r < 0) {
      perror("parent::write()");
      open_ = 0;
    }
  }
  return 0;
}

int Chat::receiveMsg() {
  if (!open_)
    return 1;
  else {
    int fd = open(recvPipe_.c_str(), O_RDONLY);
    ssize_t r = read(fd, rbuffer_, BUFFER_LENGTH);
    close(fd);
    if (r == -1) {
      perror("child::read()");
      open_ = 0;
    }
    std::cout << " [" << receiver_.name() << "] " << rbuffer_ << std::endl;
  }
  return 0;
}

int Chat::createPipes() {
  sendPipe_ = "/tmp/" + sender_.name() + "-" + receiver_.name() + ".chat";
  recvPipe_ = "/tmp/" + receiver_.name() + "-" + sender_.name() + ".chat";
  mkfifo(sendPipe_.c_str(), 0666);
  mkfifo(recvPipe_.c_str(), 0666);
  /*
  if (pipe(sendPipe_) < 0) {
    perror("pipe()");
    std::cerr << SENDER_PIPE_ERROR << std::endl;
    return 1;
  }
  if (pipe(recvPipe_) < 0) {
    close(sendPipe_[READ]);
    close(sendPipe_[WRITE]);
    std::cerr << RECEIVER_PIPE_ERROR << std::endl;
    perror("pipe()");
    return 1;
  }
  */
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
