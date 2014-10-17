#include<iostream>
#include<cstdio>
#include<cmath>
#include<algorithm>
#include<cstring>
#include<time.h>
using namespace std;

bool choose( int n, int m, int *a, int *L, int *R, int &x, int &y ){
    int num = 0, mn = -1, mx = -1;
    for(int i=0; i<n; i++){
        if( L[i]==R[i] ) continue;
        x = i; y = L[i]; num += R[i]-L[i];
        if( mn==-1 || mn>a[i*m+L[i]] ) mn = a[i*m+L[i]];
        mx = max( mx, a[i*m+R[i]-1] );
    }
    if( mn==mx ) return false;
    int k = rand()%num+1;
    for(int i=0; i<n; i++){
        if( k<=R[i]-L[i] ){ x = i; y = L[i]+k-1; break; }
        else k -= R[i]-L[i];
    }
    return true;
}
// select Kth element from a[n][m]
int Select( int n, int m, int *a, int k ){
    int *W = new int[3*n], *L = W, *R = L+n, *M = R+n, x, y;
    memset( L, 0, sizeof(int)*n );
    for(int i=0; i<n; i++) R[i] = m;
    srand((unsigned)time(NULL));
    for( ;choose( n, m, a, L, R, x, y ); ){
        int mid = a[x*m+y], num = 0;
        for(int i=0,r=R[0]; i<n; i++){
            if( r>=R[i] ) r = R[i]-1;
            while( r>=L[i] && a[i*m+r]>mid ) r--;
            num += r-L[i]+1; 
            M[i] = r+1;
        }
        if( num>=k ) swap( R, M );
        else{ k -= num; swap( L, M ); }
    }
    delete []W;
    return a[x*m+y];
}
int Select2( int n, int m, int *a, int k ){
    int L = a[0], R = a[(n-1)*m+m-1], res = -1;
    while( L<=R ){
        int mid = (L+R)/2, num = 0;
        for(int i=0,r=m-1; i<n; i++){
            while( r>=0 && a[i*m+r]>mid ) r--;
            num += r+1;
            if( num>=k ) break;
        }
        if( num>=k ){ res = mid; R = mid-1; }
        else L = mid+1;
    }
    return res;
}

int main(){
    int n, m, k, *a = new int[640000];
    while( scanf("%d%d%d",&n,&m,&k)==3 ){
        srand((unsigned)time(NULL));
        for(int i=0; i<n; i++){
            for(int j=0; j<m; j++){
                int u = rand()%10000, v = u;
                if( j>0 ) v = max( v, a[i*m+j-1]+u );
                if( i>0 ) v = max( v, a[(i-1)*m+j]+u );
                a[i*m+j] = v;
                //printf("%d ",v);
            }//printf("\n");
        }//printf("\n");
        printf("%d %d\n",a[0],a[(n-1)*m+m-1]);

        clock_t st = clock();
        printf("%d\n",Select2(n,m,a,k));
        clock_t end = clock();
        printf("%.3f\n",(double)(end-st)/CLOCKS_PER_SEC);


        st = clock();
        printf("%d\n",Select(n,m,a,k));
        end = clock();
        printf("%.3f\n",(double)(end-st)/CLOCKS_PER_SEC);


        st = clock();
        sort( a, a+n*m );
        printf("%d\n",a[k-1]);
        end = clock();
        printf("%.3f\n",(double)(end-st)/CLOCKS_PER_SEC);
    }
    delete []a;
    return 0;
}

