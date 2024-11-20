#include <string>
#include <vector>

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