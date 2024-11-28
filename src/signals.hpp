#ifndef SIGNAUX_H
#define SIGNAUX_H

#include "chat.hpp"

void handleSIGINT(int sig);
void handleSIGPIPE(int sig);
void handleSIGTERM(int sig);
void SetupSignaux();
void SetChatInstance(Chat& chatInstance);

#endif
