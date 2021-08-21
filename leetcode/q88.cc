/*
Q88:
Given two sorted integer arrays nums1 and nums2, merge nums2 into nums1 as one sorted array.

Note:

The number of elements initialized in nums1 and nums2 are m and n respectively.
You may assume that nums1 has enough space (size that is greater or equal to m + n) to hold additional elements from nums2.
Example:

Input:
nums1 = [1,2,3,0,0,0], m = 3
nums2 = [2,5,6],       n = 3

Output: [1,2,2,3,5,6]
*/
#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
    void merge(vector<int>& nums1, int m, vector<int>& nums2, int n) {
        int index1=0, index2=0;
        
        int curNum1 = m;
        while( ( index1 < m + n - 1 ) && ( index2 < n ) ) {
            if( ( nums1[ index1 ] <= nums2[ index2 ] ) && ( index1 < curNum1 ) ) {
                index1++;
            } else {
                for( int i = curNum1 ; i > index1 ; --i ) {
                    nums1[ i ] = nums1[ i - 1 ];
                }
                
                nums1[ index1 ] = nums2[ index2 ];
                index2++;
                curNum1++;
            }
        }
        
        if( curNum1 < m + n ) {
            if( index2 < n ) {
                for( int j = index2 ; j < n ; ++j ) {
                    nums1[ index1 ] = nums2[ index2 ];
                    index1++;
                    index2++;
                }
            }
        }
    }
};

int main() {
    vector<int> nums1 = { 1, 2, 3, 0, 0, 0 };
    vector<int> nums2 = { 2, 5, 6 };

    Solution().merge( nums1, 3, nums2, 3 );

    return 0;
}
