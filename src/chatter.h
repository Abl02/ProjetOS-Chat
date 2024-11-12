#ifndef CHATTER_H
#define CHATTER_H

#include <string>

class Chatter {
 public:
  Chatter(std::string name);
  ~Chatter();
  std::string name() const;

 private:
  std::string name_;
};

#endif
