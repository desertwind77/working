#include <algorithm>
#include <iostream>
#include <vector>

bool isOdd( int i ) {
    return ( i % 2 ) == 1;
}

int main() {
    std::vector< int > myVector;
    myVector.push_back( 10 );
    myVector.push_back( 25 );
    myVector.push_back( 40 );
    myVector.push_back( 55 );

    std::vector< int >::iterator it = std::find_if( myVector.begin(), myVector.end(), isOdd );
    std::cout << "The first odd value is " << *it << std::endl;

    return 0;
}
