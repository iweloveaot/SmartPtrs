#include <iostream>
#include <string>
#include "functional_tests.h"
#include "load_tests.h"
#include "unq_ptr.h"
#include "shrd_ptr.h"


int safe_read_int() {
    int value;

    while (!(std::cin >> value)) {
        std::cout << "Invalid input. Please enter a number: ";
        std::cin.clear();             
        std::cin.ignore(10000, '\n'); 
    }
    std::cin.ignore(10000, '\n'); 
    return value;
}

void demo_unique_single() {
    std::cout << "\n[UniquePtr<int> Interactive Demo]\n";
    UniquePtr<int> up1;
    UniquePtr<int> up2;
    
    int cmd = -1;
    while (cmd != 0) {
        std::cout << "\n--- Current State ---\n";
        std::cout << "up1 ptr: " << up1.get() << (up1.get() ? " (value: " + std::to_string(*up1) + ")" : "") << "\n";
        std::cout << "up2 ptr: " << up2.get() << (up2.get() ? " (value: " + std::to_string(*up2) + ")" : "") << "\n";
        
        std::cout << "\nActions:\n";
        std::cout << "1. Create/Reset up1 (enter value)\n";
        std::cout << "2. Release up1 (manual delete)\n";
        std::cout << "3. Move up1 -> up2\n";
        std::cout << "0. Back to main demo menu\n";
        std::cout << ">> ";
        
        cmd = safe_read_int();
        
        if (cmd == 1) { 
            std::cout << "Enter value: ";
            int v = safe_read_int(); 
            up1.reset(new int(v)); 
        }
        else if (cmd == 2) { 
            int* raw = up1.release(); 
            std::cout << "Released raw ptr: " << raw << " (Value: " << (raw ? *raw : -1) << ")\n"; 
            delete raw; 
        }
        else if (cmd == 3) { 
            up2 = std::move(up1); 
            std::cout << "Moved! up1 is now null.\n";
        }
        else if (cmd != 0) {
            std::cout << "Unknown command.\n";
        }
    }
}

void demo_unique_array() {
    std::cout << "\n[UniquePtr<int[]> Interactive Demo]\n";
    UniquePtr<int[]> up1;
    
    int cmd = -1;
    while (cmd != 0) {
        std::cout << "\n--- Current State ---\n";
        std::cout << "up1 ptr: " << up1.get() << "\n";
        
        std::cout << "\nActions:\n";
        std::cout << "1. Create/Reset array (enter size)\n";
        std::cout << "2. Get element by index\n";
        std::cout << "3. Release array (manual delete[])\n";
        std::cout << "0. Back to main demo menu\n";
        std::cout << ">> ";
        
        cmd = safe_read_int();
        
        if (cmd == 1) { 
            std::cout << "Enter size: ";
            int sz = safe_read_int(); 
            if (sz <= 0) { std::cout << "Size must be > 0\n"; continue; }
            int* raw = new int[sz];
            for(int i = 0; i < sz; ++i) raw[i] = i * 10;
            up1.reset(raw); 
            std::cout << "Array created and filled with i*10.\n";
        }
        else if (cmd == 2) { 
            std::cout << "Enter index: ";
            int idx = safe_read_int(); 
            std::cout << "up1[" << idx << "] = " << up1[idx] << "\n"; 
        }
        else if (cmd == 3) { 
            int* raw = up1.release(); 
            std::cout << "Released raw ptr: " << raw << "\n"; 
            delete[] raw; 
        }
        else if (cmd != 0) {
            std::cout << "Unknown command.\n";
        }
    }
}

void demo_shared_single() {
    std::cout << "\n[SharedPtr<int> Interactive Demo]\n";
    SharedPtr<int> sp1;
    SharedPtr<int> sp2;
    
    int cmd = -1;
    while (cmd != 0) {
        std::cout << "\n--- Current State ---\n";
        std::cout << "sp1 ptr: " << sp1.get() << ", count: " << sp1.share_count();
        if (sp1.get()) std::cout << ", *sp1 = " << *sp1;
        std::cout << "\n";
        
        std::cout << "sp2 ptr: " << sp2.get() << ", count: " << sp2.share_count();
        if (sp2.get()) std::cout << ", *sp2 = " << *sp2;
        std::cout << "\n";
        
        std::cout << "\nActions:\n";
        std::cout << "1. Create/Reset sp1 (enter value)\n";
        std::cout << "2. Copy sp1 -> sp2\n";
        std::cout << "3. Move sp1 -> sp2\n";
        std::cout << "4. Reset sp1 to null\n";
        std::cout << "0. Back to main demo menu\n";
        std::cout << ">> ";
        
        cmd = safe_read_int();
        
        if (cmd == 1) { 
            std::cout << "Enter value: ";
            int v = safe_read_int(); 
            sp1.reset(new int(v)); 
        }
        else if (cmd == 2) { 
            sp2 = sp1; 
            std::cout << "Copied! Watch the reference count.\n";
        }
        else if (cmd == 3) { 
            sp2 = std::move(sp1); 
            std::cout << "Moved! sp1 is now null, count unchanged.\n";
        }
        else if (cmd == 4) { 
            sp1.reset(); 
        }
        else if (cmd != 0) {
            std::cout << "Unknown command.\n";
        }
    }
}

void demo_shared_array() {
    std::cout << "\n[SharedPtr<int[]> Interactive Demo]\n";
    SharedPtr<int[]> sp1;
    SharedPtr<int[]> sp2;
    
    int cmd = -1;
    while (cmd != 0) {
        std::cout << "\n--- Current State ---\n";
        std::cout << "sp1 ptr: " << sp1.get() << ", count: " << sp1.share_count() << "\n";
        std::cout << "sp2 ptr: " << sp2.get() << ", count: " << sp2.share_count() << "\n";
        
        std::cout << "\nActions:\n";
        std::cout << "1. Create array in sp1 (enter size)\n";
        std::cout << "2. Copy sp1 -> sp2\n";
        std::cout << "3. Move sp1 -> sp2\n";
        std::cout << "4. Get element from sp1 (enter index)\n";
        std::cout << "5. Reset sp1 to null\n";
        std::cout << "0. Back to main demo menu\n";
        std::cout << ">> ";
        
        cmd = safe_read_int();
        
        if (cmd == 1) { 
            std::cout << "Enter size: ";
            int sz = safe_read_int(); 
            if (sz <= 0) { std::cout << "Size must be > 0\n"; continue; }
            int* raw = new int[sz];
            for(int i = 0; i < sz; ++i) raw[i] = i * 100;
            sp1.reset(raw); 
        }
        else if (cmd == 2) { sp2 = sp1; }
        else if (cmd == 3) { sp2 = std::move(sp1); }
        else if (cmd == 4) { 
            std::cout << "Enter index: ";
            int idx = safe_read_int(); 
            std::cout << "sp1[" << idx << "] = " << sp1[idx] << "\n"; 
        }
        else if (cmd == 5) { sp1.reset(); }
        else if (cmd != 0) {
            std::cout << "Unknown command.\n";
        }
    }
}

void run_interactive_demo() {
    int choice = -1;
    while (choice != 0) {
        std::cout << "\n========================================\n";
        std::cout << "       INTERACTIVE DEMO MENU\n";
        std::cout << "========================================\n";
        std::cout << "1. UniquePtr<int> (Single Object)\n";
        std::cout << "2. UniquePtr<int[]> (Array)\n";
        std::cout << "3. SharedPtr<int> (Single Object)\n";
        std::cout << "4. SharedPtr<int[]> (Array)\n";
        std::cout << "0. Back to Main Menu\n";
        std::cout << "Choose class to demo: ";
        
        choice = safe_read_int();

        switch (choice) {
            case 1: demo_unique_single(); break;
            case 2: demo_unique_array(); break;
            case 3: demo_shared_single(); break;
            case 4: demo_shared_array(); break;
            case 0: break;
            default: std::cout << "Invalid choice.\n";
        }
    }
}

int main() {
    int choice = -1;
    while (choice != 0) {
        std::cout << "\n========================================\n";
        std::cout << "    SMART POINTERS LAB - MAIN MENU\n";
        std::cout << "========================================\n";
        std::cout << "1. Run Functional Tests (Coverage)\n";
        std::cout << "2. Run Load Tests (Time & Memory CSV)\n";
        std::cout << "3. Interactive Manual Demo\n";
        std::cout << "0. Exit\n";
        std::cout << "Choose an option: ";
        
        choice = safe_read_int();

        switch (choice) {
            case 1: run_functional_tests(); break;
            case 2: run_load_tests(); break;
            case 3: run_interactive_demo(); break;
            case 0: std::cout << "Exiting... Goodbye!\n"; break;
            default: std::cout << "Invalid choice.\n";
        }
    }
    return 0;
}