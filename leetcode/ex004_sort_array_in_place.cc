#include <iostream>
#include <vector>

using namespace std;

void
printArray( vector<char> &v ) {
    for( vector<char>::iterator it = v.begin() ; it != v.end() ; ++it ) {
        cout << *it << " ";
    }
    cout << endl;
}

void
swap( vector<char> &v, int i, int j ) {
    if( i >= v.size() || j >= v.size() ) return;

    char tmp = v[ i ];
    v[ i ] = v[ j ];
    v[ j ] = tmp;
}

void
sortArrayInplace( vector<char> &v ) {
    if( v.size() == 0 ) return;

    int s = 0;
    int l = v.size() - 1;
    int i = 0;

    while( l >= 0 && v[ l ] == 'L' ) {
        l--;
    }

    while( i < l ) {
        if( v[ i ] == 'S' ) {
            if( i == s ) {
                ++s;
                ++i;
            } else {
                // i must be greater than s
                swap( v, i, s );
                ++s;
                // not increment i on purpose
            }
        } else if( v[ i ] == 'M' ) {
            i++;
        } else {
            // v[ i ] must be 'L'
            swap( v, i, l );
            --l;
            // not increment i on purpose
        }
    }
}

int
main() {
    vector<char> input = { 'S', 'M', 'L', 'M', 'S', 'L' };
    sortArrayInplace( input );
    printArray( input );
    return 0;
}
