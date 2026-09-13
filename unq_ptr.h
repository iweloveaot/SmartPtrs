#include <iostream>
#include <type_traits>
#include <array>

template <typename T>
class UniquePtr {
private:
    T* ptr;

    void deleter() {
        if (std::is_array<T>::value == 1) {
            delete[] ptr;
            std::cout << "array deleted\n";
        } else {
            delete ptr; 
            std::cout << "not array deleted\n";
        }
    }

public:

    UniquePtr(T* p = nullptr) : ptr(p) { std::cout << "unique constructor called\n"; }
    ~UniquePtr() { 
        deleter();
    }

    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    UniquePtr(UniquePtr&& other) noexcept : ptr(other.ptr) {
        other.ptr = nullptr;
        std::cout << "unique move constructor called\n";
    }
    UniquePtr& operator=(UniquePtr&& other) noexcept {
        if (this != &other) {
            deleter();
            ptr = other.ptr;
            other.ptr = nullptr;
        }
        std::cout << "unique move operator= called\n";
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
        std::cout << "reset is called\n";
        if (ptr != p) {
            deleter();
            ptr = p;
        }
    }
 };
