/*
 * ex03_endianess.c
 * http://en.wikipedia.org/wiki/Endianness
 */
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <arpa/inet.h>

bool is_big_endian1() {
    union {
        uint32_t i;
        char c[ 4 ];
    } bint = { 0x01020304 };
    return bint.c[ 0 ] == 1;
}

bool is_big_endian2() {
    uint32_t i = 0x01020304;
    char *j = (char *)&i;
    return j[ 0 ] == 1;
}

bool is_big_endian3() {
    // htonl converts host byte order to network
    // byte order (big endian)
    return htonl( 47 ) == 47; 
}

uint32_t convert_endianess( uint32_t in ) {
    uint32_t result = 0;

    result |= ( in & 0x000000FF ) << 24;
    result |= ( in & 0x0000FF00 ) << 8; 
    result |= ( in & 0x00FF0000 ) >> 8; 
    result |= ( in & 0xFF000000 ) >> 24; 

    return result;
}

int main() {
    if( is_big_endian1() ) {
        printf("big endian\n");
    } else {
        printf("little endian\n");
    }

    return 0;
}
