// calculate a ^ n
#include <stdio.h>

// O( n )
int power_iterative( int a, int n ) {
    int result = 1;
    int i;

    if( n ) {
        for( i = 0 ; i < n ; i ++ ) {
            result *= a;
        }
    }

    return result;
}

// O( n ) 
int power_recursive1( int a, int n ) {
    int result = 1;

    if( n ) {
        result = a * power_recursive1( a, n - 1 );
    }

    return result;
}

// O( n )
int power_recursive2( int a, int n ) {
    if( !n ) {
        return 1;
    } else if( n % 2 == 0 ) {
        return power_recursive2( a, n / 2 ) * power_recursive2( a, n / 2 );
    } else {
        return a * power_recursive2( a, n / 2 ) * power_recursive2( a, n / 2 );
    }
}

// O( log n )
int power_recursive3( int a, int n ) {
    int tmp;

    if( !n ) {
        return 1;
    } else {
        tmp = power_recursive3( a, n / 2 );
        if( n % 2 == 0 ) {
            return tmp * tmp;
        } else {
            return a * tmp * tmp;
        }
    }
}

int main() {
    int a = 2, n = 8;

    printf( "%d ^ %d = %d\n", a, n, power_iterative( a, n ) ); 
    printf( "%d ^ %d = %d\n", a, n, power_recursive1( a, n ) ); 
    printf( "%d ^ %d = %d\n", a, n, power_recursive2( a, n ) ); 
    printf( "%d ^ %d = %d\n", a, n, power_recursive3( a, n ) ); 

    return 0;
}
