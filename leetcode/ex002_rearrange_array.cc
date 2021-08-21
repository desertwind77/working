#include <assert.h>
#include <stdint.h>
#include <iostream>
#include <vector>
using namespace std;

// Move non-zero elements of an array to the left
struct TestCase {
    vector<int> input;
    vector<int> output;
} testCases[] = {
    { { 0 }, { 0 } },
    { { 1, 0, 0, 2, 5, 0 },
      { 1, 2, 5, 0, 0, 0 } },
    { { 1, 0, 0, 2, 5, 0, 3, 4, 0, 0, 9, 9, 0, 1 },
      { 1, 2, 5, 3, 4, 9, 9, 1, 0, 0, 0, 0, 0, 0 } }
};

vector<int> compact_array( vector<int> array );
bool compare_array( vector<int> input, vector<int> output );
void print_array( vector<int> array );

int main() {
    uint8_t size = sizeof( testCases ) / sizeof( struct TestCase );

    for( int i = 0 ; i < size ; ++i ) {
        vector<int> input = testCases[ i ].input;
        vector<int> expected = testCases[ i ].output;
        vector<int> output = compact_array( input );
        if( !compare_array( output, expected ) ) {
            print_array( output );
            print_array( expected );
        }
    }
}

vector<int>
compact_array( vector<int> array ) {
    uint32_t size = array.size();
    uint32_t indexForNextNonZero = -1;
    uint32_t curIndex = 0;

    while( curIndex < size ) {
        if( array[ curIndex ] != 0 ) {
            ++curIndex;
        } else {
            uint32_t indexForFirstZero = curIndex;

            if( indexForNextNonZero == -1 ) indexForNextNonZero = curIndex;
            while( ( indexForNextNonZero < size ) && ( array[ indexForNextNonZero ] == 0 ) ) {
                // Search for the next non-zero element
                ++indexForNextNonZero;
            }
            if( ( indexForNextNonZero < size ) && ( array[ indexForNextNonZero ] != 0 ) ) {
                // Found the next non-zero element
                int tmp = array[ indexForFirstZero ];
                array[ indexForFirstZero ] = array[ indexForNextNonZero ];
                array[ indexForNextNonZero ] = tmp;
                ++indexForNextNonZero;
                ++curIndex;
            } else {
                // No more non-zero element
                curIndex = size;
            }
        }
    }

    return array;
}

bool
compare_array( vector<int> input, vector<int> output ) {
    if( input.size() != output.size() ) return false;

    for( int i = 0 ; i < input.size() ; ++i ) {
        if( input[ i ] != output[ i ] ) return false;
    }

    return true;
}

void
print_array( vector<int> array ) {
    for( int i = 0 ; i < array.size() ; ++i ) {
        cout << array[ i ] << " ";
    }
    cout << endl;
}
