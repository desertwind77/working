/*
 * ex01_average_exclude_max.cc
 *
 * Find the average of numbers in an array of integer excluding
 * the first n maximum numbers
 *
 * Example : 3, 7, 12, 2, 25, 8, 9, 13, 10, 0
 * The first 3 maximum numbers : 25, 13, 12
 * Average excluding the first 3 maximum numbers = (3 + 7 + 2 + 8 + 9 + 10) / 6 = 6.5
 */
#include <assert.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>

float average_exclude_max( int *arr, int size, unsigned int excludeMax ) {
    int *maxArray = NULL;   // array contain maximum numbers
    float result = 0;
    int sum = 0;            // sum of the array arr
    int maxSum = 0;         // sum of maxArray
    int cur = 0;            // current index of arr 
    int maxCur;             // current index of maxArray 
    int i, a;

    // initialize maxArray to INT_MIN 
    if( excludeMax && ( excludeMax < size ) ) {
        maxArray = (int*)calloc( excludeMax, sizeof( int ) );
        for( a = 0; a < excludeMax; a++ ) {
            maxArray[ a ] = INT_MIN;
        }
    } else {
        return 0;
    }

    while ( cur < size ) {
        maxCur = excludeMax - 1;
        while ( maxArray && ( maxCur > 0 ) &&
                ( arr[ cur ] > maxArray[ maxCur ] ) &&
                ( arr[ cur ] >= maxArray[ maxCur - 1 ] ) ) {
            maxArray[ maxCur ] = maxArray[ maxCur - 1 ];
            maxCur--;
        }
        if ( arr[ cur ] > maxArray[ maxCur ] ) {
            maxArray[ maxCur ] = arr[ cur ];
        }

        sum += arr[ cur ];
        cur++;
    }

    for ( i = 0 ; i < excludeMax ; i++ ) {
        maxSum += maxArray[ i ];
    }

    result = ( (float)( sum - maxSum ) / ( cur - excludeMax ) );
    return result; 
}

void test1() {
    int testArray[] = { 3, 7, 12, 2, 25, 8, 9, 13, 10 };
    int size = sizeof( testArray ) / sizeof( int );
    float result = 6.5;

    assert( result == average_exclude_max( testArray, size, 3 ) );
    printf( "result = %.2f\n", average_exclude_max( testArray, size, 3 ) );
}

int main() {
    test1();
    return 0;
}
