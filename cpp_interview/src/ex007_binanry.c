#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <strings.h>

#define BITS_IN_BYTE 8

int64_t firstOneFromLsb( int64_t input ); 
void convertByteToBinary( uint8_t input, char *buf ); 
void convertToBinary( uint64_t input, size_t inputSize, char *buf ); 
void printBinary( uint64_t input, size_t inputSize );

int main() {
    uint8_t a = 0x3b, aa;
    uint16_t b = 0x7582, bb;
    uint32_t c = 0x37572693, cc;
    uint64_t d = 0x3757269637572696, dd;

    printBinary( a, sizeof( a ) );
    printf( "\n" );
    aa = firstOneFromLsb( a );
    printBinary( aa, sizeof( aa ) );
    printf( "\n" );

    printBinary( b, sizeof( b ) );
    printf( "\n" );
    b = 0x7500;
    bb = firstOneFromLsb( b );
    printBinary( bb, sizeof( bb ) );
    printf( "\n" );

    printBinary( c, sizeof( c ) );
    printf( "\n" );
    c = 0x37570000;
    cc = firstOneFromLsb( c );
    printBinary( cc, sizeof( cc ) );
    printf( "\n" );

    printBinary( d, sizeof( d ) );
    printf( "\n" );
    d = 0x0757000000000000;
    dd = firstOneFromLsb( d ); 
    printBinary( dd, sizeof( dd ) );
    printf( "\n" );

    return 0;
}

// notation : A is complement of a. complemnt of 1 is 0 and vice versa.
// input = a1b where a can be anything and b is all-0.
// -input = complement of input + 1
//        = complement( a1b ) + 1
//        = A0B + 1 where B is all-1
//        = A1b
// input & -input = 010 which is the 1st 1 from LSB
int64_t firstOneFromLsb( int64_t input ) {
    return input & ( -1 * input );
}

// x             = 0101 1000
// x - 1         = 0101 0111
// x & ( x - 1 ) = 0101 0000
uint64_t turnOffRightMostOne( uint64_t x ) {
    return x & ( x - 1);
}

bool isPowerOfTwo( uint64_t x ) {
    return turnOffRightMostOne( x ) == 0;
}

// x             = 0001 1111
// x + 1         = 0010 0000
// x & ( x + 1 ) = 0000 0000
bool isPowerOfTwoMinusOne( uint64_t x ) {
    return ( x & ( x + 1 ) == 0 );
}

void convertByteToBinary( uint8_t input, char *buf ) {
    int i, cur;

    for( i = 0 ; i < BITS_IN_BYTE ; i++ ) {
        cur = input & ( 1 << i );
        buf[ BITS_IN_BYTE - i - 1 ] = ( cur ? 1 : 0 );
    }
}

void convertToBinary( uint64_t input, size_t inputSize, char *buf ) {
    uint64_t mask, cur, allFs = 0xFF;
    int i, shift;

    for( i = 0 ; i < inputSize ; i++ ) {
        shift = BITS_IN_BYTE * ( inputSize - i - 1 );
        mask = allFs << shift; 
        cur = input & mask; 
        cur >>= shift;
        convertByteToBinary( cur, buf + ( i * BITS_IN_BYTE ) );
    }
}

void printBinary( uint64_t input, size_t inputSize ) {
    char buf[ BITS_IN_BYTE * inputSize ];
    int i;

    bzero( buf, BITS_IN_BYTE * inputSize );
    convertToBinary( input, inputSize, buf );
    for( i = 0 ; i < inputSize * BITS_IN_BYTE ; i++ ) {
        if( i && ( i % 8 == 0 ) ) printf( " " );
        printf( "%d", buf[ i ] );
    }
}
