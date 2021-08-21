#include <string>
#include <iostream>

using namespace std;

int main() {
    string a = "abadafdaf";

    for( int i = 0 ; i < a.size() ; ++i ) {
        cout << a[i];
    }
    cout << endl;

    return 0;
}
