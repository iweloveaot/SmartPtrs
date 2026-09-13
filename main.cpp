#include <iostream>
#include <utility>
#include "unq_ptr.h"
#include "shrd_ptr.h"

int main() {

    std::cout << " --------- working with Unique ---------- \n";
    int n[5] = {1, 2, 3, 4, 5};
    UniquePtr<int[5]> up1(&n);
    UniquePtr<int[5]> up2 = std::move(up1);

    double n2[3] = {6.0, 7.0, 8.0};
    double n3[3] = {16.0, 17.0, 18.0};
    UniquePtr<double[3]> up3(&n2);
    UniquePtr<double[3]> up4(&n3);
    up3 = std::move(up4);

    std::cout << "up2 first is " << up2[0] << "\n up3 second is " << up3[1] << "\n";
    std::cout << "thats all - END";

}