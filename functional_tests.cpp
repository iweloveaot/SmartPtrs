#include "functional_tests.h"
#include "test_object.h"
#include "unq_ptr.h"
#include "shrd_ptr.h"
#include <iostream>
#include <cassert>

#define TEST_ASSERT(condition, message) \
    if (!(condition)) { \
        std::cerr << "[FAIL] " << message << "\n"; \
        return; \
    } else { \
        std::cout << "[PASS] " << message << "\n"; \
    }


void test_unique_single() {
    std::cout << "\n--- UniquePtr (Single) ---\n";
    TestObject::reset_stats();

    UniquePtr<TestObject> up1(new TestObject(10));
    TEST_ASSERT(TestObject::alive_count == 1, "Created 1 object");
    TEST_ASSERT(up1->value == 10, "operator-> works");
    TEST_ASSERT((*up1).value == 10, "operator* works");

    up1.reset(new TestObject(20));
    TEST_ASSERT(TestObject::alive_count == 1, "reset: old destroyed, new alive");
    TEST_ASSERT(up1->value == 20, "reset: new value correct");

    TestObject* raw_ptr = up1.release();
    TEST_ASSERT(up1.get() == nullptr, "release: internal ptr is null");
    TEST_ASSERT(TestObject::alive_count == 1, "release: object NOT destroyed");
    TEST_ASSERT(raw_ptr->value == 20, "release: returned ptr is valid");
    
    delete raw_ptr; 
    TEST_ASSERT(TestObject::alive_count == 0, "Manual delete: object destroyed");

    UniquePtr<TestObject> up2(new TestObject(30));
    up2.reset();
    TEST_ASSERT(TestObject::alive_count == 0, "reset(nullptr): object destroyed");
    TEST_ASSERT(up2.get() == nullptr, "reset(nullptr): internal ptr is null");
}


void test_unique_array() {
    std::cout << "\n--- UniquePtr (Array) ---\n";
    TestObject::reset_stats();

    UniquePtr<TestObject[]> up1(new TestObject[3]);
    TEST_ASSERT(TestObject::alive_count == 3, "Created array of 3 objects");
    
    up1[0].value = 100;
    up1[1].value = 200;
    TEST_ASSERT(up1[0].value == 100, "operator[] read/write works");

    up1.reset(new TestObject[2]);
    TEST_ASSERT(TestObject::alive_count == 2, "reset array: old 3 destroyed, new 2 created");

    TestObject* raw_arr = up1.release();
    TEST_ASSERT(up1.get() == nullptr, "release array: internal ptr is null");
    TEST_ASSERT(TestObject::alive_count == 2, "release array: objects NOT destroyed");
    
    delete[] raw_arr;
    TEST_ASSERT(TestObject::alive_count == 0, "Manual delete[]: array destroyed");
}


void test_shared_single() {
    std::cout << "\n--- SharedPtr (Single) ---\n";
    TestObject::reset_stats();

    SharedPtr<TestObject> sp1(new TestObject(42));
    TEST_ASSERT(sp1.share_count() == 1, "Initial count is 1");
    TEST_ASSERT(sp1->value == 42, "operator-> works");

    {
        SharedPtr<TestObject> sp2 = sp1;
        TEST_ASSERT(sp1.share_count() == 2, "Copy: count is 2");
        
        sp2.reset(new TestObject(99));
        TEST_ASSERT(sp1.share_count() == 1, "reset: old object count dropped to 1");
        TEST_ASSERT(sp2.share_count() == 1, "reset: new object count is 1");
        TEST_ASSERT(TestObject::alive_count == 2, "reset: both objects alive");
        TEST_ASSERT(sp2->value == 99, "reset: new value correct");
    } 
    TEST_ASSERT(TestObject::alive_count == 1, "Scope exit: second object destroyed");
    TEST_ASSERT(sp1.share_count() == 1, "sp1 count remains 1");

    sp1.reset();
    TEST_ASSERT(TestObject::alive_count == 0, "reset(nullptr): final object destroyed");
    TEST_ASSERT(sp1.get() == nullptr, "reset(nullptr): internal ptr is null");
    TEST_ASSERT(sp1.share_count() == 0, "share_count is 0 for null ptr");
}

void test_shared_array() {
    std::cout << "\n--- SharedPtr (Array) ---\n";
    TestObject::reset_stats();

    SharedPtr<TestObject[]> sp1(new TestObject[2]);
    TEST_ASSERT(sp1.share_count() == 1, "Array: initial count is 1");
    TEST_ASSERT(TestObject::alive_count == 2, "Array: 2 objects alive");

    sp1.reset(new TestObject[3]);
    TEST_ASSERT(TestObject::alive_count == 3, "Array reset: old 2 destroyed, new 3 alive");
    TEST_ASSERT(sp1.share_count() == 1, "Array reset: count is 1");

    sp1[0].value = 100;
    TEST_ASSERT(sp1[0].value == 100, "Array: operator[] works after reset");

    sp1.reset();
    TEST_ASSERT(TestObject::alive_count == 0, "Array reset(nullptr): all destroyed");
    TEST_ASSERT(sp1.share_count() == 0, "Array: count is 0");
}

void run_functional_tests() {
    std::cout << "\n========================================\n";
    std::cout << "=== STARTING FUNCTIONAL TESTS ===\n";
    std::cout << "========================================\n";

    test_unique_single();
    test_unique_array();
    test_shared_single();
    test_shared_array(); 
    
    std::cout << "\n========================================\n";
    std::cout << "=== FUNCTIONAL TESTS COMPLETED ===\n";
    std::cout << "========================================\n\n";
}