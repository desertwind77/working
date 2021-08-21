#include <iostream>
#include <string>

int main() {
    std::string pi = "pi is " + std::to_string( 3.1415926 );
    std::string perfect = std::to_string( 1 + 2 + 4 + 7 + 14 ) + " is a perfect number";
    std::cout << pi << std::endl;
    std::cout << perfect << std::endl;

    return 0;
}
