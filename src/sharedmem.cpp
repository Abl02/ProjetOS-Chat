#include "sharedmem.hpp"

#include <sys/mman.h>

#include <cstring>
#include <string>
#include <vector>

SharedMemory::SharedMemory() {
  shared_mem = static_cast<char*>(create_memory(MEMORY_SIZE));
  shared_mem[0] = '\0';
}

std::vector<std::string> SharedMemory::get_messages() {
  std::vector<std::string> result;
  std::string tmp;
  for (size_t i = 0; shared_mem[i] != '\0'; ++i) {
    if (shared_mem[i] != '\n') {
      tmp += shared_mem[i];
    } else {
      result.push_back(tmp);
      tmp.clear();
    }
  }
  return result;
}

bool SharedMemory::add_message(const std::string& message) {
  size_t len = strlen(shared_mem);
  if (len + message.size() + 1 < MEMORY_SIZE) {
    strcat(shared_mem, message.c_str());
    strcat(shared_mem, "\n");
    return true;
  }
  return false;
}

void SharedMemory::reset_memory() { memset(shared_mem, 0, MEMORY_SIZE); }

void* SharedMemory::create_memory(size_t size) {
  const int protection = PROT_READ | PROT_WRITE;
  const int visibility = MAP_SHARED | MAP_ANONYMOUS;
  return mmap(NULL, size, protection, visibility, -1, 0);
}
