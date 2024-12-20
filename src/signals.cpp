#include "signals.hpp"

#include <signal.h>
#include <stdlib.h>

#include <iostream>

#include "chat.hpp"

static Chat* chat = nullptr;

void SetChatInstance(Chat& chatInstance) { chat = &chatInstance; }

void handleSIGINT(int sig) {
  // On recup le pid du processus qui envoie le signal
  pid_t Pid = getpid();
  // si c'est le processus principal, que les pipes sont ouverts et que le mode
  // manuel est actif --> affiche messages en memoire. Sinon on termine les 2
  // processus avec code retour 4
  if (Pid == chat->getParentPid()) {
    if (chat->arePipesOpened() && chat->isManualMode() == 1) {
      chat->afficheMessageEnAttente();
    } else {
      std::cerr
          << "Signal SIGINT (" << sig
          << ") capturé par le parent --> terminaison des deux processus.\n";
      if (chat->getSecondProcessPID() > 0) {
        kill(chat->getSecondProcessPID(), SIGTERM);
      }
      chat->DestroyFileDescriptors();
      chat->DestroyPipe();
      exit(4);
    }
  }
  // si c'est le second processus on ignore le signal
  else if (Pid == chat->getSecondProcessPID()) {
    std::cerr << "Signal SIGINT (" << sig
              << ") capturé par second process --> Ignoré.\n";
  }
}

void handleSIGPIPE(int sig) {
  std::cerr << "Signal SIGPIPE (" << sig << "), Pipe cassé.\n";
  chat->DestroyFileDescriptors();
  chat->DestroyPipe();
  kill(chat->getSecondProcessPID(), SIGTERM);
  exit(1);
}

void handleSIGTERM(int sig) {
  std::cerr << "Signal SIGTERM (" << sig
            << ") capturé --> fin du processus enfant.\n";
  exit(4);
}

void SetupSignals() {
  signal(SIGINT, handleSIGINT);
  signal(SIGPIPE, handleSIGPIPE);
  signal(SIGTERM, handleSIGTERM);
}
