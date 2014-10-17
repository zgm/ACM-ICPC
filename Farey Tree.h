#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<algorithm>
#include<cmath>
using namespace std;

//short a[8000001][2];

class FareyTree{
public:
	/*static void FareyDown( long long p1, long long q1, long long p2, long long q2, long long n, long long &a1, long long &b1, long long a, long long b ){
		while( q1+q2 <= n ){
			if( p1*b == q1*a ){ a1 = p1; b1 = q1; return; }
			if( a*q2 == b*p2 ){ a1 = p2; b1 = q2; return; }
			long long t = (p1+p2)*b-(q1+q2)*a;
			if( t == 0 ){ a1 = p1+p2; b1 = q1+q2; return; }
			if( t < 0 ){
				long long k = (a*q1-p1*b)/(p2*b-a*q2);
				k = min( k, (n-q1)/q2 );
				p1 += k*p2;
				q1 += k*q2;
				a1 = p1; b1 = q1;
			}
			else{
				long long k = (b*p2-a*q2)/(a*q1-p1*b);
				k = min( k, (n-q2)/q1 );
				p2 += k*p1;
				q2 += k*q1;
			}
		}
	}
	static void FareyUp( long long p1, long long q1, long long p2, long long q2, long long n, long long &a2, long long &b2, long long a, long long b ){
		while( q1+q2 <= n ){
			if( p1*b == q1*a ){ a2 = p1; b2 = q1; return; }
			if( a*q2 == b*p2 ){ a2 = p2; b2 = q2; return; }
			long long t = (p1+p2)*b-(q1+q2)*a;
			if( t == 0 ){ a2 = p1+p2; b2 = q1+q2; return; }
			if( t > 0 ){
				long long k = (b*p2-a*q2)/(a*q1-b*p1);
				k = min( k, (n-q2)/q1 );
				p2 += k*p1;
				q2 += k*q1;
				a2 = p2; b2 = q2;
			}
			else{
				long long k = (a*q1-p1*b)/(p2*b-a*q2);
				k = min( k, (n-q1)/q2 );
				p1 += k*p2;
				q1 += k*q2;
			}
		}
	}
	static void ans(){ // 2008 hefei: Rational Number Approximation
		long long a, b, n, t = 0;
		while( scanf("%lld%lld%lld",&a,&b,&n)==3 && a+b+n!=0 ){
			//clock_t st = clock();
			printf("Case %lld: ",++t);
			if( a == 0 ){ printf("0/1 0/1\n"); continue; }
			long long a1 = 0, b1 = 1, a2 = 1, b2 = 1;
			FareyDown( 0, 1, 1, 1, n, a1, b1, a, b );
			FareyUp( 0, 1, 1, 1, n, a2, b2, a, b );
			printf("%lld/%lld %lld/%lld\n",a1,b1,a2,b2);
			//clock_t ed = clock();
			//printf("%.6f\n", (double)(ed-st)/CLOCKS_PER_SEC);
		}
	}*/
	/*static void Farey( int p1, int q1, int p2, int q2, int n, int &k ){
		if( q1+q2 > n ) return;
		Farey( p1, q1, p1+p2, q1+q2, n, k );
		//printf("%d/%d ",p1+p2, q1+q2 );
		a[++k][0] = p1+p2;
		a[k][1] = q1+q2;
		Farey( p1+p2, q1+q2, p2, q2, n, k );
	}
	static void ans(){ // poj 3374 Cake Share
		int t = 0, n, m, c, k = 1;
		scanf("%d%d",&n,&m);
		Farey( 0, 1, 1, 1, n, k );
		while( m-- ){
			scanf("%d",&c);
			if( c == 1 ) printf("0/1\n");
			if( c == k+1 ) printf("1/1\n");
			if( c > k+1 ) printf("No Solution\n");
			if( c > 1 && c<=k ) printf("%d/%d\n",a[c][0], a[c][1] );
		}
	}*/
	static void GetPrim( int *prim, int &pn, int n, bool *vis ){
		pn = 0;
		memset( vis, 0, sizeof(bool)*n );
		for(int i=2; i<n; i++){
			if( !vis[i] ) prim[pn++] = i;
			for(int j=0; j<pn && prim[j]*i<n; j++){
				vis[prim[j]*i] = 1;
				if( i%prim[j] == 0 ) break;
			}
		}
	}
	// Euler Function, calculate the total number of w(w<n) prim to n
	static void Euler( int *prim, int pn, bool *IsPrim, int n, long long *phy ){
		phy[2] = 1;
		for(int i=3; i<=n; i++){
			if( IsPrim[i] ){ phy[i] = i-1; continue; }
			int k = -1;
			for(int j=0; j<pn; j++) if( i%prim[j] == 0 ){ k = prim[j]; break; }
			int s = i/k;
			if( s%k == 0 ) phy[i] = phy[s]*k;
			else phy[i] = phy[s]*(k-1);
		}
	}
	static void ans(){ // poj 2478 Farey Sequence
		const int N = 1000001;
		int n, prim[N/10], pn = 0; bool IsPrim[N]; long long phy[N];
		GetPrim( prim, pn, N, IsPrim );
		memset( IsPrim, 0, sizeof(bool)*N );
		for(int i=0; i<pn; i++) IsPrim[prim[i]] = true;
		for(int i=0; i<pn; i++) if( prim[i]>1000 ){ pn = i; break; }
		Euler( prim, pn, IsPrim, N-1, phy );
		for(int i=3; i<N; i++) phy[i] += phy[i-1];
		while( scanf("%d",&n)==1 && n!=0 ){
			//int k = 1;
			//Farey( 0, 1, 1, 1, n, k );
			printf("%I64d\n", phy[n]);
		}
	}
};

int main(){
	FareyTree::ans();
	return 0;
}