// Given a 32-bit signed integer, reverse digits of an integer.
//
// Assume we are dealing with an environment which could only store integers within the 32-bit signed
// integer range: [−2^31,  2^31 − 1]. For the purpose of this problem, assume that your function returns 0
// when the reversed integer overflows.
#include <assert.h>
#include <limits.h>
#include <iostream>
using namespace std;

class Solution {
public:
    int reverse( int x ) {
        int reverse = 0;
        int rem = 0;

        do {
            rem = x % 10;
            x /= 10;

            if( ( reverse > INT_MAX / 10 ) || 
                    ( ( reverse == INT_MAX / 10 ) && ( rem > 7 ) ) ) {
                return 0;
            }
            if( ( reverse < INT_MIN / 10 ) ||
                    ( ( reverse == INT_MIN / 10 ) && ( rem < -8 ) ) ) {
                return 0;
            }
            reverse = reverse * 10 + rem;
        } while( x != 0 );

        return reverse;
    }
};

struct TestCase {
    long input;
    long output;
} testCases[] = {
    { 123, 321 },
    { -123, -321 },
    { 120, 21 },
    { 2147483647, 0 },
    { -2147483648, 0 },
};

int main() {
    uint32_t size = sizeof( testCases ) / sizeof( struct TestCase );

    for( uint8_t i = 0 ; i < size ; ++i ) {
        long input = testCases[ i ].input;
        long output = testCases[ i ].output;
        assert( Solution().reverse( input ) == output );
    }

    cout << "Passed" << endl;

    return 0;
}
