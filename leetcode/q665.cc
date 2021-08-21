// Given an array with n integers, your task is to check if it could become non-decreasing by modifying at most 1 element.
// We define an array is non-decreasing if array[i] <= array[i + 1] holds for every i (1 <= i < n).
//
// Testcases
// [4,2,3] = True
// [2,3,3,2,4] = True
// [1,2,4,5,3] = True
// [4,2,1] = False
//
#include <algorithm>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string>
#include <vector>

using namespace std;

class Solution {
public:
    bool checkPossibility(vector<int>& nums) {
        int size = nums.size();
        bool isSwapped = false;
        bool result = false;

        for( int i=size - 1 ; i >= 0 ; i-- ) {
            if( i ==  0 ) {
                result = true;
                break;
            } else if( i == 1 ) {
                if( nums[ i ] >= nums[ i - 1 ] ) {
                    // no-opt; move on
                } else if( !isSwapped ) {
                    nums[ i - 1 ] = nums[ i ];
                    isSwapped = true;
                } else {
                    break;
                }
            } else if( nums[ i ] >= nums[ i - 1 ] ) {
              if( nums[ i - 1 ] >= nums[ i - 2 ] ) {
                  // no-opt; move on
              } else if( !isSwapped ) {
                  if( nums[ i ] >= nums[ i - 2 ] ) {
                      nums[ i - 1 ] = nums[ i ];
                  } else {
                      nums[ i - 2 ] = nums[ i - 1 ];
                  }
                  isSwapped = true;
              } else {
                  break;
              }
            } else if( !isSwapped ) {
                nums[ i ] = nums[ i - 1];
                isSwapped = true;
            } else {
                break;
            }
        }
    
        return result;
    }
};

void trimLeftTrailingSpaces(string &input) {
    input.erase(input.begin(), find_if(input.begin(), input.end(), [](int ch) {
        return !isspace(ch);
    }));
}

void trimRightTrailingSpaces(string &input) {
    input.erase(find_if(input.rbegin(), input.rend(), [](int ch) {
        return !isspace(ch);
    }).base(), input.end());
}

vector<int> stringToIntegerVector(string input) {
    vector<int> output;
    trimLeftTrailingSpaces(input);
    trimRightTrailingSpaces(input);
    input = input.substr(1, input.length() - 2);
    stringstream ss;
    ss.str(input);
    string item;
    char delim = ',';
    while (getline(ss, item, delim)) {
        output.push_back(stoi(item));
    }
    return output;
}

string boolToString(bool input) {
    return input ? "True" : "False";
}

int main() {
    string line;
    while (getline(cin, line)) {
        vector<int> nums = stringToIntegerVector(line);
        
        bool ret = Solution().checkPossibility(nums);

        string out = boolToString(ret);
        cout << out << endl;
    }
    return 0;
}
