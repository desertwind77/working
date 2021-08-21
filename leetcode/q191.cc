// Write a function that takes an unsigned integer and return the number of '1' bits it has 
// (also known as the Hamming weight).
#include <assert.h>
#include <iostream>
using namespace std;

class Solution {
public:
    int hammingWeight( uint32_t n ) {
        int count = 0;

        while( n != 0 ) {
            n = n & ( n - 1 );
            count++;
        }

        return count;
    }
};

struct TestCase {
    uint32_t input;
    uint32_t output;
} testCases[] = {
    { 0x0B, 3 },
    { 0x80, 1 },
    { 0xFFFFFFFD, 31 },
};

int main() {
    uint32_t size = sizeof( testCases ) / sizeof( struct TestCase );

    for( uint8_t i = 0 ; i < size ; ++i ) {
        uint32_t input = testCases[ i ].input;
        uint32_t output = testCases[ i ].output;
        assert( Solution().hammingWeight( input ) == output );
    }

    cout << "Passed" << endl;

    return 0;
}
