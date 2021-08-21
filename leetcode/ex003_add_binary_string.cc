#include <string>
#include <iostream>

using namespace std;

string
addBinaryString( string a, string b ) {
    if( a.size() == 0 ) {
        return b;
    } else if( b.size() == 0 ) {
        return a;
    }

    string result, adder;
    if( a.size() > b.size() ) {
        result = a;
        adder = b;
    } else {
        result = b;
        adder = a;
    }

    int i = result.size() - 1;
    int j = adder.size() - 1;
    int rem = 0;
    while( i >= 0 ) {
        int tmp = 0;
        if( j >= 0 ) {
            tmp = result[ i ] + adder[ j ] + rem;
        } else {
            tmp = result[ i ] + rem;
        }

        if( tmp == 0 || tmp == 1 ) {
            result[ i ] = '0' + tmp;
            rem = 0;
        } else {
            // tmp must be 2. we can assert here.
            result[ i ] = '0';
            rem = 1;
        }

        --i;
        --j;
    }

    if( rem > 0 ) {
        result = to_string( rem ) + result;
    }

    return result;
}

int main() {
    string a = "1010";
    string b = "111";
    string result = addBinaryString( a, b );
    cout << result << endl;

    return 0;
}
