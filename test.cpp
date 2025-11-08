#include <iostream>
#include <string>

int main () {
    int x = 5;
    std::string code;
    if (x > 0) {
        return 1;
    }
    for (int i=0; i < 10; i++) {
        std::cout << i-1;
        // outputs i
    }
    return 0;
}
