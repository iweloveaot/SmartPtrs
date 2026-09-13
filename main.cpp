#include <iostream>
#include <utility>
#include "shrd_ptr.h" 

// Тестовый класс для отслеживания вызовов конструктора и деструктора
struct TestObject {
    int value;
    TestObject(int v) : value(v) { 
        std::cout << "  [TestObject] Created: " << value << "\n"; 
    }
    ~TestObject() { 
        std::cout << "  [TestObject] Destroyed: " << value << "\n"; 
    }
};

int main() {
    std::cout << "=== 1. Testing Single Object (Copy & Move) ===\n";
    {
        // Создаем указатель. Счетчик = 1
        SharedPtr<TestObject> sp1(new TestObject(42));
        std::cout << "sp1 count: " << sp1.share_count() << "\n\n"; 
        
        {
            // Копирование. Счетчик должен стать 2
            SharedPtr<TestObject> sp2 = sp1; 
            std::cout << "After copy -> sp1 count: " << sp1.share_count() << "\n\n";
            
            // Перемещение. Счетчик не меняется (остается 2), но sp1 обнуляется
            SharedPtr<TestObject> sp3 = std::move(sp1); 
            std::cout << "After move -> sp1 count (should be 0): " << sp1.share_count() << "\n";
            std::cout << "After move -> sp3 count: " << sp3.share_count() << "\n\n";
            
        } // Здесь sp2 и sp3 уничтожаются. Счетчик падает до 0. 
          // Должно напечататься: "[TestObject] Destroyed: 42"
        
        std::cout << "End of scope 1.\n\n";
    }

    std::cout << "=== 2. Testing Array ===\n";
    {
        // Создаем массив. Используем int[]!
        SharedPtr<int[]> arr1(new int[3]{10, 20, 30});
        std::cout << "arr1 count: " << arr1.share_count() << "\n";
        
        // Копируем
        SharedPtr<int[]> arr2 = arr1; 
        std::cout << "arr1[1] via arr2: " << arr2[1] << "\n"; // Должно быть 20
        std::cout << "After copy -> arr1 count: " << arr1.share_count() << "\n";
        
    } // Здесь arr1 и arr2 уничтожаются. Память должна освободиться через delete[]
      // Если запустить под Valgrind, утечек не будет.

    std::cout << "\n=== 3. Testing reset() ===\n";
    {
        SharedPtr<TestObject> sp(new TestObject(100));
        
        // Сбрасываем на новый объект. Старый (100) должен уничтожиться!
        std::cout << "Calling reset()...\n";
        sp.reset(new TestObject(200)); 
        std::cout << "sp count after reset: " << sp.share_count() << "\n\n";
        
        // Сбрасываем в nullptr. Объект (200) должен уничтожиться, счетчик стать 0.
        std::cout << "Calling reset(nullptr)...\n";
        sp.reset(); 
        std::cout << "sp count after reset to null: " << sp.share_count() << "\n";
    }

    std::cout << "\n=== All tests completed successfully ===\n";
    return 0;
}