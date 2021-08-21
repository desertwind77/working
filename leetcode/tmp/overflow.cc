#include <limits.h>
#include <iostream>
using namespace std;

int main() {
    int x = INT_MIN;
    int y = INT_MAX;

    cout << x << " " << y << endl;
    --x;
    ++y;
    cout << x << " " << y << endl;
    cout << -10 / 3 << " " << -10 % 3 << endl;
}
