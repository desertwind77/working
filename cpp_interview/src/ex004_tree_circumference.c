/*
 * ex004_tree_circumference.c
 *
 * http://www.careercup.com/question?id=14087880
 *
 * Calculate circumference of a tree going clockwise and anticlockwise?
 *
 *           1
 *     2           3
 *  4     5     6     7
 * 8 9   0 1   2 4   
 * will have circumference -> 1,2,4,8,9,0,1,2,4,7,3.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct node_ {
	int value;
	struct node_ *left, *right;
} node;

void preOrder( node *r, bool printMe, bool printLeft, bool printRight ) {
    if ( !r->left && !r->right ) {
        printf( "%d ", r->value );
    } else {
        if ( printMe ) printf( "%d ", r->value );
        if ( r->left ) {
            preOrder( r->left, printLeft, printLeft, printRight );
		}
        if( r->right ) {
            preOrder( r->right, printRight, printRight, printRight );
		}
	}
}

void preOrderReverse( node *r, bool printMe, bool printLeft, bool printRight) {
    if ( !r->left && !r->right ) {
        printf( "%d ", r->value );
    } else {
        if ( r->right ) {
            preOrderReverse( r->right, printRight, printRight, printRight );
        }
        if ( r->left ) {
            preOrderReverse( r->left, printLeft, printLeft, printRight );
		}
        if ( printMe ) printf( "%d ", r->value );
    }
}

void postOrder( node *r, bool printMe, bool printLeft, bool printRight ) {
    if ( !r->left && !r->right ) {
        printf( "%d ", r->value );
    } else {
        if ( r->left ) {
            postOrder( r->left, printLeft, printLeft, printLeft );
        }
        if ( r->right ) {
            postOrder( r->right, printRight, printLeft, printRight );
        }
        if ( printMe ) printf( "%d ", r->value );
    }
}

void postOrderReverse( node *r, bool printMe, bool printLeft, bool printRight) {
    if( !r->left && !r->right ) {
        printf( "%d ", r->value );
    } else {
        if ( printMe ) printf( "%d ", r->value ); 
		if ( r->right ) {
            postOrderReverse( r->right, printRight, printLeft, printRight );
        }
        if ( r->left ) {
            postOrderReverse( r->left, printLeft, printLeft, printLeft );
        }
    }
}

void counterClockwise( node *t ) {
    printf( "%d ", t->value );
    if ( t->left ) preOrder( t->left, true, true, false );
	if ( t->right ) postOrder( t->right, true, false, true );
	printf( "\n" );
}

void clockWise( node *t ) {
	printf( "%d ", t->value );
	if ( t->right ) postOrderReverse( t->right, true, false, true );
	if ( t->left ) preOrderReverse( t->left, true, true, false );
	printf( "\n" );
}

node * newNode( int v, node *l, node *r ) {
    node *n = (node *) malloc( sizeof( node ) );
    n->value = v;
    n->left  = l;
    n->right = r;
	return n;
}

// create the following tree
//                              1
//              2               |               3 
//       4       |       5       |       6       |       7
//   8   |   9   |  10   |  11   |  12   |  13   |  14   |  15
// 16 17 | 18 19 | 20 21 | 22 23 | 24 25 | 26 27 | 28 29 | 30 31
node * createTestTree() {
	node *n1 = newNode( 16, NULL, NULL );
	node *n2 = newNode( 17, NULL, NULL );
	node *n3 = newNode( 18, NULL, NULL );
	node *n4 = newNode( 19, NULL, NULL );
	node *n5 = newNode( 20, NULL, NULL );
	node *n6 = newNode( 21, NULL, NULL );
	node *n7 = newNode( 22, NULL, NULL );
	node *n8 = newNode( 23, NULL, NULL );
	node *n9 = newNode( 24, NULL, NULL );
	node *n10 = newNode( 25, NULL, NULL );
	node *n11 = newNode( 26, NULL, NULL );
	node *n12 = newNode( 27, NULL, NULL );
	node *n13 = newNode( 28, NULL, NULL );
	node *n14 = newNode( 29, NULL, NULL );
	node *n15 = newNode( 30, NULL, NULL );
	node *n16 = newNode( 31, NULL, NULL );

	node *n17 = newNode( 8, n1, n2 );
	node *n18 = newNode( 9, n3, n4 );
	node *n19 = newNode( 10, n5, n6 );
	node *n20 = newNode( 11, n7, n8 );
	node *n21 = newNode( 12, n9, n10 );
	node *n22 = newNode( 13, n11, n12 );
	node *n23 = newNode( 14, n13, n14 );
	node *n24 = newNode( 15, n15, n16 );

	node *n25 = newNode( 4, n17, n18 );
	node *n26 = newNode( 5, n19, n20 );
	node *n27 = newNode( 6, n21, n22 );
	node *n28 = newNode( 7, n23, n24 );

	node *n29 = newNode( 2, n25, n26 );
	node *n30 = newNode( 3, n27, n28 );

	node *n31 = newNode( 1, n29, n30 );

	return n31;
}

int main() {
	node *tree = createTestTree();

    printf( "Counter clockwise : " );
	counterClockwise( tree ); 

    printf( "Clockwise         : " );
	clockWise( tree ); 

    return 0;
}
