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

    
    double findMedian(int A[], int m, int B[], int n, int k){
        if(m>n) return findMedian(B,n,A,m,k);
        if(m==0) return B[k-1];
        if(k==1) return min(A[0],B[0]);
        
        int md = min(m,k/2), nd = k-md;
        if(A[md-1]<B[nd-1]) return findMedian(A+md,m-md,B,nd,k-md);
        else return findMedian(A,md,B+nd,n-nd,md);
    }
    double findMedianSortedArrays(int A[], int m, int B[], int n) {
        int mn = m+n;
        if(mn&1) return findMedian(A,m,B,n,mn/2+1);
        else return (findMedian(A,m,B,n,mn/2)+findMedian(A,m,B,n,mn/2+1))/2;
    }
};


int main(){
    return 0;
}
