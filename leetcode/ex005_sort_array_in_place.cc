#include <iostream>
#include <vector>

using namespace std;

void
printArray( vector<int> &v ) {
    for( vector<int>::iterator it = v.begin() ; it != v.end() ; ++it ) {
        cout << *it << " ";
    }
    cout << endl;
}

void
swap( vector<int> &v, int i, int j ) {
    if( i >= v.size() || j >= v.size() ) return;

    char tmp = v[ i ];
    v[ i ] = v[ j ];
    v[ j ] = tmp;
}

void
sortArrayInplace( vector<int> &v ) {
    if( v.size() == 0 ) return;

    int i = 0;
    int first = 0;
    while( i < v.size() ) {
        if( v[ i ] != 0 ) {
            if( i == first ) {
                i++;
                first++;
            } else {
                swap( v, i, first );
                first++;
                // not do i++ on purpose
            }
        } else {
            i++;
        }
    }
}

int
main() {
    vector<int> input = { 1, 0, 0, 2, 5, 0, 3, 0, 9, 4, 0, 7 };
    sortArrayInplace( input );
    printArray( input );
    return 0;
}
