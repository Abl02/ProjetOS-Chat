#ifndef SHAREDMEM_H
#define SHAREDMEM_H

#include <string>
#include <vector>

const int MEMORY_SIZE = 4096;

struct SharedMemory {
 public:
  SharedMemory();

  std::vector<std::string> get_messages();
  bool add_message(const std::string& message);

  void reset_memory();

 private:
  char* shared_mem;
  void* create_memory(size_t size);
};

#endif
