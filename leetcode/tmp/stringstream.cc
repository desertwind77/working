#include <iostream>
#include <sstream>

int main() {
    std::stringstream ss;
    ss << 100 << " " << 200;

    int foo, bar;
    ss >> foo >> bar;

    std::cout << "foo:" << foo << std::endl;
    std::cout << "bar:" << bar << std::endl;

    return 0;
}
