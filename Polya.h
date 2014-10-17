#include<stdlib.h>
#include<stdio.h>
#include<cstring>
#include<iostream>
using namespace std;


class Polya{
public:
	// 计算置换（1...n ，a ）的循环因子个数
	static int Num( int n, int *a ){
		bool *vis = new bool[n+1];
		memset( vis, 0, sizeof(bool)*(n+1) );
		int k = 0;
		for(int i=0; i<n; i++){
			if( vis[i] ) continue;
			for(int j=i; !vis[j]; j=a[j]) vis[j] = 1;
			k++; 
		}
		delete []vis;
		return k;
	}
	static int Gcd( int a, int b ){
		if( a < b ) swap( a, b );
		for( ; b!=0; swap(a,b),b%=a);
		return a;
	}
	static long long Power( int n, int p ){ // n^p
		if( p==0 ) return 1;
		long long s = 1, t = n;
		for( ; p!=1; t*=t,p>>=1) if( p&1 ) s *= t;
		return s*t;
	}
	// 计算长度为n的环，用p种颜色着色，不等价的着色数，可以旋转和翻转
	/*static long long Burnsid( int n, int p ){
		if( n*p == 0 ) return 0;
		long long s = Power( p, n ), t = n*Power( p, n/2 );
		s += ((n&1) ? t*p:t/2*(p+1));
		for(int i=1; i<n; i++) s += Power( p, Gcd(i,n) );
		return s/(2*n);
	}*/
	// 计算长度为n的环，用p种颜色着色，不等价的着色数，可以旋转但不可以翻转
	/*static long long Burnsid( int n, int p ){
		if( n*p == 0 ) return 0;
		long long s = 0, t;
		for(int i=1; i<=n; i++) s += Power( p, Gcd(i,n) );
		return s/n;
	}*/
	static void GetPrim( int &pn, int *prim, int n, bool *vis ){
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
	static int Euler( int n, int pn, int *prim ){
		int s = 1;
		for(int i=0; i<pn && prim[i]*prim[i]<=n && n>1; i++){
			if( n%prim[i] ) continue;
			for(s*=(prim[i]-1),n/=prim[i]; n%prim[i]==0; s*=prim[i],n/=prim[i]);
		}
		return ( n==1 ? s:s*(n-1) );
	}
	static int Euler( int n ){
		int s = 1;
		for(int i=2; i*i<=n; i++){
			if( n%i ) continue;
			for(s*=(i-1),n/=i; n%i==0; s*=i,n/=i);
		}
		return ( n==1 ? s:s*(n-1) );
	}
	static int Power( int n, int p, int Mod ){ // n^p % Mod
		if( p==0 ) return 1;
		int s = 1, t = n%Mod;
		for( ; p!=1; p>>=1){
			if( p&1 ) s = (s*t)%Mod;
			t = (t*t)%Mod;
		}
		return (s*t)%Mod;
	}
	static void Factor_DFS( int v, int k, int qn, int *q, int *num, int &fn, int *f ){
		for(int i=0; i<=num[k]; i++){
			if( k == qn-1 ) f[fn++] = v;
			else Factor_DFS( v, k+1, qn, q, num, fn, f );
			v *= q[k];
		}
	}
	static void Factor( int value, int pn, int *prim, int &fn, int *factor ){
		if( value==1 ){ factor[0] = 1; fn = 1; return; }
		int n = 0, a[60], num[60];
		for(int i=0; i<pn && value>1; i++){
			if( value%prim[i] != 0 ) continue;
			for(num[n]=0; value%prim[i]==0; value/=prim[i],num[n]++);
			a[n++] = prim[i];
		}
		if( value>1 ){ num[n] = 1; a[n++] = value; }

		fn = 0;
		Factor_DFS( 1, 0, n, a, num, fn, factor );
	}
	// 计算长度为n的环，用n种颜色着色，不等价的着色数，可以旋转但不可以翻转
	static int Burnsid( int n, int Mod, int pn, int *prim, int *factor ){
		int s = 0, t, fn = 0;
		Factor( n, pn, prim, fn, factor );
		for(int i=0; i<fn; i++){
			t = (Euler( n/factor[i], pn, prim )%Mod*Power( n, factor[i]-1, Mod ))%Mod;
			s = (s+t)%Mod;
		}
		return s;
	}
	/*static void ans(){ // poj 2154  Color
		int t, n, mod;
		int pn, prim[35000], factor[70000]; bool vis[35000];
		GetPrim( pn, prim, 35000, vis );
		scanf("%d",&t);
		while( t-- ){
			scanf("%d%d",&n,&mod);
			printf("%d\n",Burnsid(n,mod,pn,prim,factor));
		}
	}*/
	static void Mul( int n, int *A, int *B, int *tmp, int Mod ){ // a = a*b % Mod
		memset( tmp, 0, sizeof(int)*n*n );
		for(int i=0, *a=A, *t=tmp; i<n; i++, a+=n, t+=n){
			for(int j=0, *b=B; j<n; j++, b+=n){
				if( a[j] == 0 ) continue;
				for(int k=0; k<n; k++) t[k] = (t[k]+a[j]*b[k])%Mod;
			}
		}
		memcpy( A, tmp, sizeof(int)*n*n );
	}
	static void Power( int n, int *a, int p, int *t, int *tmp, int Mod ){ // t = a^p % Mod
		if( p == 1 ) memcpy( t, a, sizeof(int)*n*n );
		else{
			Power( n, a, p>>1, t, tmp, Mod );
			Mul( n, t, t, tmp, Mod );
			if( p&1 ) Mul( n, t, a, tmp, Mod );
		}
	}
	static int GetTr( int m, int *a, int p, int Mod, int *t ){
		int num = 0;
		memset( t, 0, sizeof(int)*m*m );
		for(int i=0; i<m; i++) t[i*m+i] = 1;
		for(int i=0; (1<<i)<=p; i++) if( p&(1<<i) ) Mul( m, t, a+i*m*m, t+m*m, Mod );
		//Power( m, a, p, t, t+m*m, Mod );
		for(int i=0; i<m; i++) num += t[i*m+i];
		return num%Mod;
	}
	// 求x的逆元，一种方法为 x^(p-2) % p
	static int Inv( int x, int Mod ){ return Power( x, Mod-2, Mod ); }
	static int Burnsid( int n, int m, int *a, int Mod, int pn, int *prim, int *factor, int *buf ){
		int s = 0, t, fn = 0;
		Factor( n, pn, prim, fn, factor );
		for(int i=0; i<fn; i++){
			t = Euler( n/factor[i], pn, prim )%Mod * GetTr( m, a, factor[i], Mod, buf );
			s = (s+t)%Mod;
		}
		return (s*Inv(n,Mod))%Mod;
	}
	static void ans(){ // poj 2888  Magic Bracelet
		int T, n, m, k, a[100*31];
		int pn, prim[35000], factor[70000], buf[200]; bool vis[35000];
		GetPrim( pn, prim, 35000, vis );
		scanf("%d",&T);
		while( T-- ){
			scanf("%d%d%d",&n,&m,&k);
			for(int i=0; i<m*m; i++) a[i] = 1;
			for(int i=0,u,v; i<k; i++){
				scanf("%d%d",&u,&v);
				u--; v--;
				a[u*m+v] = a[v*m+u] = 0;
			}
			for(int i=1; i<=30; i++){
				memcpy( a+i*m*m, a+(i-1)*m*m, sizeof(int)*m*m );
				Mul( m, a+i*m*m, a+i*m*m, buf, 9973 );
			}
			printf("%d\n",Burnsid(n,m,a,9973,pn,prim,factor,buf));
		}
	}
};

int main(){
	// sgu 294
	Polya::ans();
	return 0;
}
