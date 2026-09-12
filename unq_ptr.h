#include <type_traits>
#include <array>

template <typename T>
class UniquePtr {
private:
    T* ptr;
public:

    UniquePtr(T* p = nullptr) : ptr(p) {}
    ~UniquePtr() { 
        if (std::is_array<*ptr>::value == 1)
            delete[] ptr;
        else
            delete ptr; 
    }

    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    UniquePtr(UniquePtr&& other) noexcept : ptr(other.ptr) {
        other.ptr = nullptr;
    }
    UniquePtr& operator=(UniquePtr&& other) noexcept {
        if (this != &other) {
            if (std::is_array<*ptr>::value == 1)
                delete[] ptr;
            else
                delete ptr; 
            ptr = other.ptr;
            other.ptr = nullptr;
        }
        return *this;
    }

    T& operator*() const { return *ptr; }
    T* operator->() const { return ptr; }
    T& operator[](int index) const { return ptr[index]; } 

    
    T* get() const { return ptr; }
    T* release() {
        T* tmp = ptr;
        ptr = nullptr;
        return tmp;
    }
    void reset(T* p = nullptr) {
        if (ptr != p) {
            if (std::is_array<*ptr>::value == 1)
                delete[] ptr;
            else
                delete ptr; 
            ptr = p;
        }
    }
 };
