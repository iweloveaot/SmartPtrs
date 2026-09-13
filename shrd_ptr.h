#include <iostream>
#include <type_traits>
#include <array>

template <typename T>
class SharedPtr {
private:
    T* ptr;
    int* ref_count;

public:

    SharedPtr(T* p = nullptr) : ptr(p), ref_count(p ? new int(1) : nullptr) {}
    
    SharedPtr(const SharedPtr& other)
        : ptr(other.ptr), ref_count(other.ref_count) {
            ++(*ref_count);
    }
    SharedPtr& operator=(const SharedPtr& other) {
        if (this != &other) {
            if (ref_count && --(*ref_count) == 0) {
                delete ptr;
                delete ref_count;
            }
            ptr = other.ptr;
            ref_count = other.ref_count;
            ++(*ref_count);
        }
        return *this;
    }

    SharedPtr(SharedPtr&& other) noexcept
        : ptr(other.ptr), ref_count(other.ref_count) {
            other.ptr = nullptr;
            other.ref_count = nullptr;
    }
    SharedPtr& operator=(SharedPtr&& other) noexcept {
        if (this != &other) {
            if (ref_count && --(*ref_count) == 0) {
                delete ptr;
                delete ref_count;
            }
            ptr = other.ptr;
            ref_count = other.ref_count;
            other.ptr = nullptr;
            other.ref_count = nullptr;
        }
        return *this;
    }

    ~SharedPtr() {
        if (ref_count && --(*ref_count) == 0) {
            delete ptr;
            delete ref_count;
        }
    }

    T& operator*() const { return *ptr; }
    T* operator->() const { return ptr; }

    T* get() const { return ptr; }
    void reset(T* p = nullptr) {
        if (ptr != p) {
            if (--(*ref_count) == 0) {
                delete ptr;
                delete ref_count;
            }
            ptr = p;
            ref_count = p ? new int(1) : nullptr;            
        }
    }

    int share_count() { return ref_count ? *ref_count : 0; }
 };


 template <typename T>
class SharedPtr<T[]> {
private:
    T* ptr;
    int* ref_count;

public:

    SharedPtr(T* p = nullptr) : ptr(p), ref_count(p ? new int(1) : nullptr) {}
    
    SharedPtr(const SharedPtr& other)
        : ptr(other.ptr), ref_count(other.ref_count) { ++(*ref_count); }
    SharedPtr& operator=(const SharedPtr& other) {
        if (this != &other) {
            if (ref_count && --(*ref_count) == 0) {
                delete[] ptr;
                delete ref_count;
            }
            ptr = other.ptr;
            ref_count = other.ref_count;
            ++(*ref_count);
        }
        return *this;
    }

    SharedPtr(SharedPtr&& other) noexcept
        : ptr(other.ptr), ref_count(other.ref_count) {
            other.ptr = nullptr;
            other.ref_count = nullptr;
    }
    SharedPtr& operator=(SharedPtr&& other) noexcept {
        if (this != &other) {
            if (ref_count && --(*ref_count) == 0) {
                delete[] ptr;
                delete ref_count;
            }
            ptr = other.ptr;
            ref_count = other.ref_count;
            other.ptr = nullptr;
            other.ref_count = nullptr;
        }
        return *this;
    }

    ~SharedPtr() {
        if (ref_count && --(*ref_count) == 0) {
            delete[] ptr;
            delete ref_count;
        }
    }

    T& operator[](int index) const { return ptr[index]; }

    T* get() const { return ptr; }
    void reset(T* p = nullptr) {
        if (ptr != p) {
            if (--(*ref_count) == 0) {
                delete[] ptr;
                delete ref_count;
            }
            ptr = p;
            ref_count = p ? new int(1) : nullptr;            
        }
    }

    int share_count() { return ref_count ? *ref_count : 0; }
 };
