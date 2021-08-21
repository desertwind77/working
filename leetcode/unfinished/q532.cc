#include <vector>
#include <iostream>
using namespace std;

struct Pair {
    int a, b;
};

class Solution {
public:
    int findPairs(vector<int>& nums, int k) {
        vector<Pair> found;
        
        for( int i = 0 ; i < nums.size() ; ++i ) {
            for( int j = 0 ; j < nums.size() ; ++j ) {
                int first = nums[ i ];
                int second = nums[ j ];

                if( i == j ) continue;
                cout << i << " " << j << " " << first << " " << second << endl;
                
                int diff = first - second;
                if( diff < 0 ) diff *= -1;
                
                if( first > second ) {
                    int tmp = first;
                    first = second;
                    second = tmp;
                }
                

                if( diff == k ) {
                    bool isExist = false;
                    for( vector<Pair>::iterator it = found.begin() ; 
                        it != found.end() ; ++it ) {
                        Pair item = *it;
                        if( item.a == first && item.b == second ) {
                            isExist = true;
                        }
                    }

                   cout << "---" << i << " " << j << " (" << first << ", " << second << ") " << isExist << endl;
                
                    if( !isExist ) {
                        Pair newItem;
                        newItem.a = first;
                        newItem.b = second;
                        found.push_back( newItem );
                    }
                }
            }

            cout << endl;
        }
        
        return found.size();
    }
};

int main() {
    vector<int> nums = { 1, 2, 3, 4, 5 };
    cout << Solution().findPairs( nums, 1 ) << endl;
    return 0;
};
