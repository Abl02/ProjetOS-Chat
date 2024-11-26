#include "signals.hpp"
#include "chat.hpp"  

#include <iostream>
#include <signal.h>
#include <stdlib.h> 

static Chat* chat = nullptr;

void SetChatInstance(Chat& chatInstance) {
    chat = &chatInstance;
}

void handleSIGINT(int sig) {
    // On recup le pid du processus qui envoie le signal
    pid_t Pid = getpid();

    // si c'est le processus principal, que les pipes sont ouverts et que le mode manuel est actif --> affiche messages en memoire. Sinon on termine les 2 processus avec code retour 4
    if (Pid == chat->getParentPid()) {
        if (chat->arePipesOpened() && chat->isManualMode() == 1) {
            std::cout << "manual et ouvert" << std::endl;
            chat->afficheMessageEnAttente();
        } 
        else {
            std::cout << "Signal SIGINT (" << sig << ") capturé par le parent --> terminaison des deux processus.\n";
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
        std::cout << "Signal SIGINT (" << sig << ") capturé par second process --> Ignoré.\n";
    }
    }


void handleSIGPIPE(int sig) {
    std::cerr << "Signal SIGPIPE (" << sig << "), Pipe cassé.\n";
    chat->DestroyFileDescriptors();
    chat->DestroyPipe();
    exit(1); 
}


void handleSIGTERM(int sig) {
    std::cout << "Signal SIGTERM (" << sig << ") capturé --> fin du processus enfant.\n";
    exit(4); 
}


void SetupSignaux() {
    signal(SIGINT, handleSIGINT);
    signal(SIGPIPE, handleSIGPIPE);
    signal(SIGTERM, handleSIGTERM);
}
