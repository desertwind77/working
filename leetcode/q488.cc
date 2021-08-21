#include <vector>
#include <iostream>
using namespace std;

void printVector( vector<int> v ) {
    for( int i = 0 ; i < v.size() ; ++i ) {
        cout << v[ i ] << " ";
    }
    cout << endl;
}

class Solution {
public:
    vector<int> findDisappearedNumbers(vector<int>& nums) {
        vector<int> missing;
        int cur;
        
        for( int i = 0 ; i < nums.size() ; ++i ) {
            cout << "i = " << i << " nums[i] = " << nums[ i ] << endl;
            printVector( nums);

            while( ( cur = nums[ i ] ) != nums[ cur - 1 ] ) {
                if( cur != nums[ cur - 1 ] ) {
                    int tmp = nums[ cur - 1 ];
                    nums[ cur - 1 ] = cur;
                    nums[ i ] = tmp;
                } else {
                    break;
                }
            }
        }
        
        for( int i = 0 ; i < nums.size() ; ++i ) {
            if( nums[ i ] != ( i + 1 ) ) {
                missing.push_back( i + 1 );
            }
        }
        
        return missing;
    }
};

int main() {
    vector<int> nums = {192,43,47,189,84,75,194,93,30,155,19,75,117,55,100,135,176,129,11,190,109,116,108,24,19,85,37,123,8,90,182,152,104,27,106,163,115,28,183,41,33,188,14,138,132,24,15,133,42,73,125,43,94,132,129,3,122,192,190,117,11,90,152,154,193,140,137,48,26,164,10,4,2,136,178,32,104,128,126,180,200,4,157,56,180,63,72,186,118,170,10,48,167,160,58,96,200,92,109,103,50,91,139,154,61,50,20,183,141,126,111,91,33,168,128,41,62,77,172,57,34,15,145,57,136,122,66,14,79,60,114,64,7,159,52,139,103,51,29,145,197,36,39,29,115,35,74,186,80,9,52,181,151,157,78,92,30,165,199,184,163,58,170,87,133,16,69,1,16,67,178,96,84,56,111,93,97,141,196,79,165,63,168,156,106,40,95,40,62,181,49,27,20,76,37,66,172,26,116,49};
    vector<int> result = Solution().findDisappearedNumbers( nums );	
    printVector( result );
}
