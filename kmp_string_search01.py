#!/usr/bin/env python3

def isSubstringBruteForce( str1, str2 ):
    for i in range( 0, len( str1 ) - len( str2 ) + 1 ):
        match = True
        for j in range( 0, len( str2 ) ):
            if str1[ i + j ] != str2[ j ]:
                match = False
                break
        if match:
            return True

    return False

def isSubstringKMP( str1, str2 ):
    pass

def isSubstring( _str1, _str2 ):
    str1 = _str1
    str2 = _str2
    if len( _str1 ) < len( _str2 ):
        str1 = _str2
        str2 = _str1
    return isSubstringBruteForce( str1, str2 )

def main():
    testVector = [
        ( 'abcdabcabcdf', 'abcdf', True ),
        ( 'abcdabcabcdf', 'xyz', False ),
        ( 'abcdabcabcdf', '', True ),
    ]

    for ( str1, str2, result ) in testVector: 
        assert isSubstring( str1, str2 ) == result

main()
