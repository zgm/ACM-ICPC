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
    
    /*vector<vector<int> > threeSum(vector<int> &num){
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
    }*/
    
    bool canJump(int A[], int n){
        int maxD = 0;
        for(int i=0; i<n; i++){
            if(maxD>=i) maxD = max( maxD, i+A[i] );
        }
        return maxD>=n-1;
    }
    int jump(int A[], int n){
        int step = 0, maxD = 0, nextD = 0;
        for(int i=0; i<n; i++){
            if(maxD<i){ step++; maxD = nextD; }
            nextD = max( nextD, i+A[i] );
        }
        return step;
    }
    
    void MaxDiffK(int n, int A[], int K, int B[]){ // change A[] to B[], so that B[i]-B[i+1]<=K
        int maxA = A[0];
        for(int i=0; i<n; i++) maxA = max( maxA, A[i] );
        int dp[n][maxA+1];
        for(int i=0; i<n; i++){
            for(int j=0; j<=maxA; j++){
                if(i==0){
                    dp[i][j] = abs(A[i]-j);
                }else{
                    dp[i][j] = -1;
                    for(int d=-K; d<=K; d++){
                        if(0>j+d || j+d>maxA) continue;
                        if( dp[i][j]==-1 || dp[i][j]>dp[i-1][j+d])
                            dp[i][j] = dp[i-1][j+d];
                    }
                    dp[i][j] += abs(A[i]-j);
                }
            }
        }
        
        int minDiff = dp[n-1][0];
        for(int i=0; i<=maxA; i++){
            if( minDiff>dp[n-1][i] ){
                minDiff = dp[n-1][i];
                B[n-1] = i;
            }
        }
        printf("the min diff is %d\n",minDiff);
        for(int i=n-2; i>=0; i--){
            for(int d=-K; d<=K; d++){
                if( dp[i][B[i+1]+d]+abs(A[i+1]-B[i+1])==dp[i+1][B[i+1]] ){
                    B[i] = B[i+1]+d;
                    break;
                }
            }
        }
        for(int i=0; i<n; i++) printf("%d ",A[i]); printf("\n");
        for(int i=0; i<n; i++) printf("%d ",B[i]); printf("\n");
    }
    
    struct Node{
        int key, count;
        Node(int value=0){ key=value; count=1; }
    }p[10000];
    void build_tree(int s, int t, int x[], int id){
        if(s==t){ p[id].key = x[s]; p[id].count = 1; return; }
        
        int mid = (s+t)/2;
        build_tree(s,mid,x,id<<1);
        build_tree(mid+1,t,x,(id<<1)+1);
        p[id].count = p[id<<1].count + p[(id<<1)+1].count;
    }
    int getKth(int s, int t, int id, int K){
        p[id].count--;
        if(s==t) return p[id].key;
        
        int mid = (s+t)/2;
        if(p[id<<1].count>=K) return getKth(s,mid,id<<1,K);
        else return getKth(mid+1,t,(id<<1)+1,K-p[id<<1].count);
    }
};



int main(){
    Solution r;
    
    int N = 9, x[] = {1,2,3,4,5,6,7,8,9}, count[] = {8,4,4,4,1,2,1,0,0}; // 9,5,6,7,2,4,3,1,8
    r.build_tree(0,N-1,x,1);
    for(int i=0; i<N; i++){
        printf("%d ",r.getKth(0,N-1,1,count[i]+1));
    }
    printf("\n");
    
    
    string s = "abcdedcfe";
    cout<<r.longestPalindrome(s)<<endl;
    
    int n = 10, A[] = {1,2,1,2,1,8,9,8,9,8}, K = 1, B[10] = {0};
    r.MaxDiffK(n,A,K,B);
    return 0;
}
