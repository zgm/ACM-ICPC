#include<cstdio>
#include<algorithm>
#include<iostream>
#include<map>
#include<cstring>
using namespace std;


class Tree{
public:
    static void BuildTree( int l, int r, int deep, int n, int *v, int *s, int *toLeft ){
        if( l==r ) return;
        int mid = (l+r)/2, left = mid-l+1, st = deep*n;
        for(int i=l; i<=r; i++) if( v[st+i]<s[mid] ) left--;
        for(int i=l, ls=l, rs=mid+1; i<=r; i++){
            if( i==l )  toLeft[st+i] = 0;
            else toLeft[st+i] = toLeft[st+i-1];
            if( v[st+i]<s[mid] ){
                toLeft[st+i]++;
                v[st+n+ls++] = v[st+i];
            }else if( v[st+i]>s[mid] ) v[st+n+rs++] = v[st+i];
            else{
                if( left ){
                    left--;
                    toLeft[st+i]++;
                    v[st+n+ls++] = v[st+i];
                }
                else v[st+n+rs++] = v[st+i];
            }
        }
        BuildTree( l, mid, deep+1, n, v, s, toLeft );
        BuildTree( mid+1, r, deep+1, n, v, s, toLeft );
    }
    static int Query( int l, int r, int deep, int n, int *v, int *toLeft, int L, int R, int K ){
        if( l==r ) return v[deep*n+r];
        int st = deep*n, mid = (l+r)/2;
        int slr = 0, sl = 0; // [L,R]，[l,L-1]分到左边的个数
        if( l==L ) slr = toLeft[st+R];
        else{ slr = toLeft[st+R]-toLeft[st+L-1]; sl = toLeft[st+L-1]; }
        if( slr>=K ) return Query( l, mid, deep+1, n, v, toLeft, l+sl, l+sl+slr-1, K );
        else{
            int sr = L-l-sl, srl = R-L+1-slr; // [l,L-1],[L,R]分到右边的个数
            return Query( mid+1, r, deep+1, n, v, toLeft, mid+sr+1, mid+sr+srl, K-slr );
        }
    }
    /*static void ans(){ // poj 2104  K-th Number
        const int N = 100008;
        int n, m, *v = new int[20*N], s[N], *toLeft = new int[20*N];
        while( scanf("%d%d",&n,&m)==2 ){
            for(int i=0; i<n; i++){
                scanf("%d",&v[i]);
                s[i] = v[i];
            }
            sort( s, s+n );
            BuildTree( 0, n-1, 0, n, v, s, toLeft );
            for(int i=0,L,R,K; i<m; i++){
                scanf("%d%d%d",&L,&R,&K);
                printf("%d\n",Query(0,n-1,0,n,v,toLeft,L-1,R-1,K));
            }
        }
    }*/
    static int Lowbit( int t ){ return ( t&(t^(t-1)) ); }
	static void Plus( int ps, int ad, int *a, int n ){
		while( ps < n ){
			a[ps] += ad;
			ps += Lowbit( ps );
		}
	}
	static int Sum( int ps, int *a ){
		int s = 0;
		while( ps > 0 ){
			s += a[ps];
			ps -= Lowbit( ps );
		}
		return s;
	}
	static int Find( int k, int *a, int n ){
		int i, cnt = 0, res = 0;
		for(i=0; (1<<i)<n; i++);
		for(i-=1; i>=0; i--){
			res += (1<<i);
			if( res>=n || cnt+a[res]>=k ) res -= (1<<i);
			else cnt += a[res];
		}
		return res+1;
	}
    static void ans(){ // toj 3758  Jewel
        const int N = 100008;
        int cs=0, n, *p = new int[2*N*4]; char c[20];
        int *v = new int[20*N], s[N], *toLeft = new int[20*N];
        int a[N]; map<int,int> mp;
        while( scanf("%d",&n)==1 ){
            int m = 0;
            for(int i=0,r; i<n; i++){
                scanf("%s",c);
                r = strlen(c);
                if( c[r-1]=='t' ){
                    p[i*4+0] = 0;
                    scanf("%d",&p[i*4+1]);
                    v[m++] = p[i*4+1];
                }else if( c[r-1]=='1' ){
                    p[i*4+0] = 1;
                    scanf("%d%d%d",&p[i*4+1],&p[i*4+2],&p[i*4+3]);
                }else if( c[r-1]=='2' ){
                    p[i*4+0] = 2;
                    scanf("%d",&p[i*4+1]);
                }else{
                    p[i*4+0] = 3;
                    scanf("%d",&p[i*4+1]);
                }
            }
            memcpy( s, v, sizeof(int)*m );
            sort( s, s+m );
            BuildTree( 0, m-1, 0, m, v, s, toLeft );
            
            mp.clear();
            for(int i=0; i<m; i++) mp[s[i]] = i+1;
            memset( a, 0, sizeof(int)*(m+1) );

            long long r1 = 0, r2 = 0, r3 = 0;
            for(int i=0; i<n; i++){
                if( p[i*4+0]==0 ){
                    Plus( mp[p[i*4+1]], 1, a, m+1 );
                }else if( p[i*4+0]==1 ){
                    r1 += Query(0,m-1,0,m,v,toLeft,p[i*4+1]-1,p[i*4+2]-1,p[i*4+3]);
                }else if( p[i*4+0]==2 ){
                    r2 += Sum( mp[p[i*4+1]], a );
                }else if( p[i*4+0]==3 ){
                    r3 += s[Find( p[i*4+1], a, m+1 )-1];
                }
            }
            printf("Case %d:\n%lld\n%lld\n%lld\n",++cs,r1,r2,r3);
        }
    }
};

int main(){
    Tree::ans();
    return 0;
}

