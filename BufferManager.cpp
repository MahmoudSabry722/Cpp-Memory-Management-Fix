#include <iostream>
#include <vector>
#include <memory>
#include <cstring>

class BufferManager {
private:
    size_t size;
    char* data;

public:
    // 1. Constructor
    BufferManager(size_t s) : size(s), data(new char[s]) {
        std::cout << "Resource acquired: " << size << " bytes.\n";
    }

    // 2. Destructor
    ~BufferManager() {
        delete[] data;
        std::cout << "Resource released.\n";
    }

    // 3. Copy Constructor
    BufferManager(const BufferManager& other) : size(other.size), data(new char[other.size]) {
        std::memcpy(data, other.data, size);
        std::cout << "Copy constructor called.\n";
    }

    // 4. Move Constructor
    BufferManager(BufferManager&& other) noexcept : size(other.size), data(other.data) {
        other.size = 0;
        other.data = nullptr;
        std::cout << "Move constructor called.\n";
    }

    // 5. Copy Assignment Operator (Added to fix the bug)
    BufferManager& operator=(const BufferManager& other) {
        if (this != &other) { // Prevent self-assignment
            delete[] data;    // Free existing memory
            size = other.size;
            data = new char[size];
            std::memcpy(data, other.data, size);
            std::cout << "Copy assignment called.\n";
        }
        return *this;
    }

    // 6. Move Assignment Operator (Added to complete Rule of Five)
    BufferManager& operator=(BufferManager&& other) noexcept {
        if (this != &other) { // Prevent self-assignment
            delete[] data;    // Free existing memory
            size = other.size;
            data = other.data;
            
            // Nullify the source object
            other.size = 0;
            other.data = nullptr;
            std::cout << "Move assignment called.\n";
        }
        return *this;
    }

    // Write data function
    void setData(const char* src, size_t length) {
        if (length <= size && src != nullptr) {
            std::memcpy(data, src, length);
        }
    }
};

BufferManager createBuffer() {
    BufferManager buf(1024);
    return buf; 
}

int main() {
    std::cout << "--- Start ---\n";

    std::vector<BufferManager> vec;
    vec.reserve(2);

    std::cout << "Pushing first...\n";
    vec.push_back(createBuffer());

    // Changed the comment to reflect correct vector behavior
    std::cout << "Pushing second (No reallocation because of reserve(2))...\n";
    BufferManager b(512);
    vec.push_back(std::move(b));
    
    std::cout << "--- End ---\n";
    return 0;
}
