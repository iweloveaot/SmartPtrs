#include <iostream>
#include <type_traits>
#include <array>

template <typename T>
class SharedPtr {
private:
    T* ptr;
    int* ref_count;

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

    SharedPtr(T* p = nullptr) : ptr(p), ref_count(new int(1)) { std::cout << "shrd constructor called\n"; }
    
    SharedPtr(const SharedPtr& other)
        : ptr(other.ptr), ref_count(other.ref_count) {
            ++(*ref_count);
            std::cout << "shrd copy constructor called\n";
    }
    SharedPtr& operator=(const SharedPtr& other) {
        if (this != &other) {
            if (--(*ref_count) == 0) {
                deleter();
                delete ref_count;
            }
            ptr = other.ptr;
            ref_count = other.ref_count;
            ++(*ref_count);
        }
        std::cout << "shrd copy operator= called\n";
        return *this;
    }

    SharedPtr(SharedPtr&& other) noexcept
        : ptr(other.ptr), ref_count(other.ref_count) {
            other.ptr = nullptr;
            other.ref_count = nullptr;
            std::cout << "shrd move constructor called\n";
    }
    SharedPtr& operator=(SharedPtr&& other) noexcept {
        if (this != &other) {
            if (--(*ref_count) == 0) {
                deleter();
                delete ref_count;
            }
            ptr = other.ptr;
            ref_count = other.ref_count;
            other.ptr = nullptr;
            other.ref_count = nullptr;
        }
        std::cout << "shrd move operator= called\n";
        return *this;
    }

    ~SharedPtr() {
        if (--(*ref_count) == 0) {
            deleter();
            delete ref_count;
        }
    }

    T& operator*() const { return *ptr; }
    T* operator->() const { return ptr; }
    T& operator[](int index) const { return ptr[index]; } 

    T* get() const { return ptr; }
    void reset(T* p = nullptr) {
        if (ptr != p) {
            if (--(*ref_count) == 0) {
                deleter();
                delete ref_count;
            }
            ptr = p;
            ref_count = new int(1);            
        }
    }

    int share_count() { return ref_count ? *ref_count : 0; }
 };
