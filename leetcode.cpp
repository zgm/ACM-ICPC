#include<iostream>
#include<cstdio>
#include<cstdlib>
using namespace std;


class Solution {
public:
    int findPeakElement(const vector<int> &num) {
        int n = num.size();
        int left = 0, right = n-1, mid;
        while(left!=right){
            mid = (left+right)/2;
            if(mid==0 || num[mid-1]<num[mid]){
                if(mid==n-1 || num[mid]>num[mid+1]) return mid;
                else left = mid+1;
            }else right = mid-1;
        }
        return left;
    }
};


int main(){
    return 0;
}
