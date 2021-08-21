#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Fibonacci series
// f( 0 ) = 0
// f( 1 ) = 1
// f( n ) = f( n - 1) + f( n - 2 )
// A simple, but non-efficient recursive implementation
int fibonacci_recursive( int n ) {
    if ( n == 0 ) {
        return 0;
    } else if ( n == 1 ) {
        return 1;
    } else {
        return fibonacci_recursive( n - 1 ) + fibonacci_recursive( n - 2 );
    }
}

// An efficient recursive implementation for finding Fibonacci series using
// backtracing to avoid finding the terms which are already been calculated
int do_fibonacci_lookup( int * fib, int n ) {
    if ( ( n != 0 ) && !fib[ n ] ) {
        fib[ n ] = do_fibonacci_lookup( fib, n - 1 ) +
                   do_fibonacci_lookup( fib, n - 2 );
    }

    return fib[ n ];
}

int fibonacci_dynamic( int n ) {
    int * fib = (int *) calloc( n + 1, sizeof( int ) );
    if ( !fib ) return -1;

    bzero( fib, ( n + 1 ) * sizeof( int ) );
    fib[ 0 ] = 0;
    fib[ 1 ] = 1;

    if ( n > 1 ) {
        fib[ n ] = do_fibonacci_lookup( fib, n );
    }

    return fib[ n ];
}

// An iterative implementation which is more efficient than its recursive
// counterpart
int fibonacci_nonrecursive( int n ) {
    int arr[ n + 1 ];
    int res;
    int i;

    arr[ 0 ] = 0;
    arr[ 1 ] = 1;
    for ( i = 2 ; i <= n ; i++ ) {
        arr[ i ] = arr[ i - 1 ] + arr[ i - 2 ];
    }

    res = arr[ n ];

    return res;
}

// An more memory-efficient iterative implementation using only 3 variables
int fibonacci_three_var( int n ) {
    int Fn, Fn1, Fn2, i;

    if ( n == 0 ) {
        return 0;
    } else if ( n == 1 ) {
        return 1;
    } else {
        Fn1 = 1;        // Fn-1
        Fn2 = 0;        // Fn-2
        for ( i = 2 ; i <= n ; i++ ) {
            Fn = Fn1 + Fn2;
            Fn2 = Fn1;
            Fn1 = Fn;
        }
    }

    return Fn;
}

void matrix_multiply( int a[ 2 ][ 2 ], int b[ 2 ][ 2 ], int c[ 2 ][ 2 ] ) {
    int p = a[ 0 ][ 0 ] * b[ 0 ][ 0 ] + a[ 0 ][ 1 ] * b[ 1 ][ 0 ];
    int q = a[ 0 ][ 0 ] * b[ 1 ][ 0 ] + a[ 0 ][ 1 ] * b[ 1 ][ 1 ];
    int r = a[ 1 ][ 0 ] * b[ 0 ][ 0 ] + a[ 1 ][ 1 ] * b[ 1 ][ 0 ];
    int s = a[ 1 ][ 0 ] * b[ 1 ][ 0 ] + a[ 1 ][ 1 ] * b[ 1 ][ 1 ];

    c[ 0 ][ 0 ] = p;
    c[ 0 ][ 1 ] = q;
    c[ 1 ][ 0 ] = r;
    c[ 1 ][ 1 ] = s;
}

void matrix_copy( int src[ 2 ][ 2 ], int dst[ 2 ][ 2 ] ) {
    int i, j;

    for( i = 0 ; i < 2 ; i++ ) {
        for( j = 0 ; j < 2 ; j++ ) {
            dst[ i ][ j ] = src[ i ][ j ];
        }
    }
}

void matrix_power( int m[ 2 ][ 2 ], int n, int r[ 2 ][ 2] ) {
    int p[ 2 ][ 2 ];
    int i;

    assert( n >= 2 );
    matrix_copy( m, p );
    for( i = 2 ; i <= n ; i++ ) {
        matrix_multiply( p, m , r );
        matrix_copy( r, p );
    }
}

void matrix_power_log_n( int m[ 2 ][ 2 ], int n, int r[ 2 ][ 2 ] ) {
    int t[ 2 ][ 2 ];

    assert( n >= 2 );
    if( n == 2 ) {
        matrix_multiply( m, m, r );
    } else if ( n % 2 == 0 ) {
        matrix_power_log_n( m, n / 2, t );
        matrix_multiply( t, t, r );
    } else { 
        matrix_power_log_n( m, n - 1, r );
        matrix_multiply( m, r, r );
    }
}

// | 1 1 | ^ n = | Fn+1 Fn   |
// | 1 0 |       | Fn   Fn-1 |
// O( n )
int fibonacci_matrix( int n ) {
    int m[ 2 ][ 2 ] = { { 1, 1 }, { 1, 0 } };
    int f[ 2 ][ 2 ];

    if( n == 0 ) {
        return 0;
    } else if( n == 1 ) {
        return 1;
    } else {
        matrix_power_log_n( m, n - 1, f );
        return f[ 0 ][ 0 ];
    }
}

// F( n ) = ( 2 * F( k - 1 ) + F( k ) ) * F( k )        ; n is even and k = n /2
// F( n ) = F( k ) * F( k ) + F( k - 1 ) * F( k - 1 )   ; n is odd and k = (n + 1 )/2
int do_fib( int *fib, int n ) {
    int k;

    if( n == 0 ) {
        return 0;
    } else if( n == 1 || n == 2 ) {
        fib[ n ] = 1;
        return fib[ n ];
    } else {
        if( fib[ n ] ) return fib[ n ];

        if( n % 2 == 0 ) {
            k = n / 2;
            fib[ n ] = ( 2 * do_fib( fib,  k - 1 ) + do_fib( fib,  k ) ) *
                do_fib( fib, k );
        } else {
            k = ( n + 1 ) / 2;
            fib[ n ] = do_fib( fib, k ) * do_fib( fib, k ) + 
                do_fib( fib, k - 1) * do_fib( fib, k - 1 );
        }

        return fib[ n ];
    }
}

int fibonacci_log_n( int n ) {
    int fib[ n + 1 ];
    // fib is in stack so we need to fill with 0 first.
    // Otherwise, the fib lookup may screw up. 
    bzero( fib, sizeof( fib ) );
    return do_fib( fib, n );
}

int main() {
    int input[] = { 10, 13 };
    int i, n;
    int size = sizeof( input ) / sizeof( int );

    for ( i = 0 ; i < size ; i++ ) {
        n = input[ i ];
        printf( "Fibonacci of %d = %d\n", n, fibonacci_recursive( n ) ); 
        printf( "Fibonacci of %d = %d\n", n, fibonacci_dynamic( n ) ); 
        printf( "Fibonacci of %d = %d\n", n, fibonacci_nonrecursive( n ) ); 
        printf( "Fibonacci of %d = %d\n", n, fibonacci_three_var( n ) ); 
        printf( "Fibonacci of %d = %d\n", n, fibonacci_matrix( n ) ); 
        printf( "Fibonacci of %d = %d\n", n, fibonacci_log_n( n ) ); 
        printf( "\n" );
    }

    return 0;
}
