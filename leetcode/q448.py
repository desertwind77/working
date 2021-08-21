#!/usr/bin/env python3

def missingNumber( array ):
   for i in range( 0, len( array ) ):
      while array[ i ] != i + 1 and array[ array[ i ] - 1 ] != array[ i ]:
         tmp = array[ array[ i ] - 1 ]
         array[ array[ i ] - 1 ] = array[ i ]
         array[ i ] = tmp

   answer = [ i for i in range( 0, len( array ) ) if array[ i ] != i + 1 ]
   return answer

if __name__ == '__main__':
   array = [ 4, 3, 2, 7, 8, 2, 3, 1]
   print( missingNumber( array ) )

