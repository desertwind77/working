/*
 * ex002_print_column.c
 *
 * Given an array, print it in n column as shown below.
 *
 * Input : ['a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j'],
 * Output:
 * a e i
 * b f j
 * c g
 * d h
 */
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

void printColumn( char *arr, int size, int col ) {
    int step, cur, i, j;

    if ( !size || !col || ( col > size ) ) return;

    step = ceil( size * 1.0 / col );

    for ( i = 0 ; i < step ; i++ ) {
        for( j = 0 ; j < col ; j++ ) {
            cur = ( j * step ) + i;
            if ( cur < size ) {
                printf( "%c ", arr[ cur ] );
            }
        }
        printf( "\n" );
    }
}

int main() {
    char *charArray;
    unsigned int size = 'z' - 'a' + 1;
    char ch;

    // charArray is an array containing 'a' to 'z'
    charArray = malloc( size );
    for( ch = 'a' ; ch <= 'z' ; ch++ ) {
        charArray[ ch - 'a' ] = ch;
    }

    printColumn( charArray, size, 5 ); 

    return 0;
}
