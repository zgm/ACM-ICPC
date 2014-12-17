#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<vector>
#include<string>
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
    
    
    string longestPalindrome(string s){
        int n = s.length();
        string t = "", result = "";
        for(int i=0; i<n; i++){ t += '$'; t += s[i]; }
        t += '$';
        int m = 2*n+1;
        
        vector<int> len(m,0);
        int x = 0, xlen = 0, mx = 0, mlen = 0;
        for(int i=1; i<m; i++){
            if(x+xlen>=i) len[i] = min(len[2*x-i],x+xlen-i);
            while(0<i-len[i] && i+len[i]+1<m && t[i-len[i]-1]==t[i+len[i]+1]) len[i]++;
            if( i+len[i]>x+xlen ){ x = i; xlen = len[i]; }
            if(len[i]>mlen){ mx = i; mlen = len[i]; }
        }
        for(int i=(mx-mlen)/2; i<(mx+mlen)/2; i++) result += s[i];
        return result;
    }
    
    vector<vector<int> > threeSum(vector<int> &num){
        vector<vector<int> > result;
        vector<int> tmp(3,0);
        unordered_set<vector<int> > st;
        sort(num.begin(),num.end());
        for(int i=0; i<num.size(); i++){
            int j = i+1, k = num.size()-1;
            while(j<k){
                if(j>=i+2 && num[j-1]==num[j]){ j++; continue; }
                int sum = num[i]+num[j]+num[k];
                if(sum==0){
                    tmp[0] = num[i]; tmp[1] = num[j]; tmp[2] = num[k];
                    if(st.find(tmp)!=st.end()){
                        result.push_back(tmp);
                        st.insert(tmp);
                    }
                    j++; k--;
                }else if(sum<0){
                    j++;
                }else{
                    k--;
                }
            }
        }
        return result;
    }
};



int main(){
    //string s = "abcdedcfe";
    string s = "a";
    Solution r;
    cout<<r.longestPalindrome(s)<<endl;
    return 0;
}
