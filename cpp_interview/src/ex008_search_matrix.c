/*
 * Given a sorted matrix where the number below and right of you will
 * always be bigger, write an algorithm to find if a particular number
 * exist in the matrix. What is the running time of your algorithm.
 */
bool doSearch( int **matrix, int startRow, int startCol,
             int endRow, int endCol, int key ) {
    int midRow = ( endRow - startRow ) / 2;
    int midCol = ( endCol - startCol ) / 2;

    1, 2, 3
    non-square

    if( key == matrix[ midRow ][ midCol ] ) {
        return true;
    } else if( key < matrix[ midRow ][ midCol ] ) {
        return doSearch( matrix, startRow, startCol, midRow, midCol, key );
    } else {
        return doSearch( matrix, midRow + 1, startCol,
                         endRow, midCol, key ) ||
               doSearch( matrix, startRow, midCol + 1, 
                         midRow, endCol, key ) ||
               doSearch( matrix, midRow + 1, midCol + 1,
                         endRow, endCol, key );
    }
}

bool search( int **matrix, int row, int col, int key ) {
    return doSearch( matrix, 0, 0, row - 1, col - 1, key );
}

int main() {
}
