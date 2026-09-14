#ifndef _TEST_OBJECT_H_
#define _TEST_OBJECT_H_

#include <iostream>

struct TestObject {
    
    inline static int alive_count = 0; 
    inline static int total_created = 0;

    int value;
    
    TestObject(int v = 0) : value(v) { 
        ++alive_count; 
        ++total_created;
    }
    
    ~TestObject() { 
        --alive_count; 
    }
    
    static void reset_stats() {
        alive_count = 0;
        total_created = 0;
    }
};

#endif