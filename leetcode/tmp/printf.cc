#include <stdio.h>

int main() {
    int myInt = 3;
    long myLong = 4;
    long long myLlong = 5;
    unsigned myUnsigned = 6;
    unsigned long myULong = 7;
    unsigned long long myULlong = 8;
    float myFloat = 3.1415926;
    double myDouble = 3.1415926;
    long double myLDouble = 3.1415926;

    printf( "myInt = %d\n", myInt );
    printf( "myLong = %ld\n", myLong );
    printf( "myLlong = %lld\n", myLong );
    printf( "myUnsinged = %u\n", myUnsigned );
    printf( "myULong = %lu\n", myULong );
    printf( "myULlong = %lu\n", myULlong );
    printf( "myFloat = %f\n", myFloat );
    printf( "myDouble = %f\n", myDouble );
    printf( "myLDouble = %Lf\n", myLDouble );
}
