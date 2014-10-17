#include<iostream>
#include<cstdio>
using namespace std;

//const int N = 1000008, M = 1008;
const int N = 10008, M = 10008;

int dp[2][M];

int Match( char *text, char *key, int K ){
    int old = K, result = K+1;
    dp[0][0] = 0;
    for(int i=1; key[i]; i++) dp[0][i] = i;
    for(int i=1,r=1,last=K+1; text[i]; i++,r^=1){
        dp[r][0] = 0;
        for(int j=1; j<last; j++){
            if( text[i]==key[j] ) dp[r][j] = dp[r^1][j-1];
            else dp[r][j] = 1+min(min(dp[r^1][j-1],dp[r^1][j]),dp[r][j-1]);
        }
        if( text[i]==key[last] ) dp[r][last] = dp[r^1][last-1];
        else dp[r][last] = 1+min(dp[r^1][last-1],dp[r][last-1]);
        while( dp[r][last]>K ) last--;
        if( !key[last+1] ){
            result = min( result, dp[r][last] );
            K = min( K, result );
        }
        else last++;
    }
    return result>old ? -1:result;
}

char text[N], key[M];

int main(){
    int k;
    /*while( scanf("%s",&text[1])==1 ){ // hdu 3540    EditingOperation
        scanf("%s%d",&key[1],&k);
        printf("%d\n",Match(text,key,k));
    }*/
    while( scanf("%d",&k)==1 ){ // hrbeu online contest  1005   Just Search 没通过
        scanf("%s%s",&key[1],&text[1]);
        if( Match(text,key,k)==-1 ) printf("NO!\n");
        else printf("YES!\n");
    }
    return 0;
}
