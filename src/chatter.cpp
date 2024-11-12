#include "chatter.h"

#include <string>

Chatter::Chatter(std::string name) : name_(name){};

Chatter::~Chatter(){};

std::string Chatter::name() const { return name_; }
