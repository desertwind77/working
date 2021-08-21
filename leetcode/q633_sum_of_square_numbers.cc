#include <iostream>
#include <stdlib.h>

using namespace std;

bool judgeSquareSum( int c ) {
    bool *isSqrtable = (bool *) malloc( sizeof( bool ) * ( c + 1 ) );

    int i = 0;
    while( i < c + 1 ) {
        isSqrtable[ i ] = false;
        i++;
    }

    long j = 0;
    while( j * j <= c ) {
        isSqrtable[ j * j ] = true;
        j++;
    }

    int k = c;
    while( k >= 0 ) {
        int l = c - k; 
        if( isSqrtable[ k ] && isSqrtable[ l ] ) return true;
        k--;
    }

    return false;
}

int main() {
    cout << judgeSquareSum( 10000000 ) << endl;
    return 0;
}
