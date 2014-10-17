#include<stdio.h>
#include<stdlib.h>
#include<cstring>
#include<algorithm>
using namespace std;

void Get_Prim( int *prim, int &pn, int n ){
	bool *vis = new bool[n];
	memset( vis, 0, sizeof(bool)*n );
	pn = 0;
	for(int i=2; i<n; i++){
		if( !vis[i] ) prim[pn++] = i;
		for(int j=0; j<pn && prim[j]*i<n; j++){
			vis[prim[j]*i] = 1;
			if( i%prim[j] == 0 ) break;
		}
	}
	delete []vis;
}
void Factor_DFS( long long v, int k, int qn, int *q, int *num, int &fn, long long *f ){
	for(int i=0; i<=num[k]; i++){
		if( k == qn-1 ) f[fn++] = v;
		else Factor_DFS( v, k+1, qn, q, num, fn, f );
		v *= q[k];
	}
}
void Factor( long long value, int pn, int *prim, int &fn, long long *factor ){
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

int Gcd( int a, int b ){
	if( a < b ) swap( a, b );
	while( b!= 0 ){ swap( a, b ); b %= a; }
	return a;
}
// calculate gcd(a,b) and the integer x and y where gcd(a,b)=a*x+b*y
int Ext_Gcd( int a, int b, int &x, int &y ){
	if( b == 0 ){ x = 1; y = 0; return a; }
	int d = Ext_Gcd( b, a%b, x, y );
	int t = x;
	x = y;
	y = t-a/b*y;
	return d;
}
// get the smallest positive answer x where x % m1 = a1 and x % m2 = a2
long long Mod_L_EQ( int a1, int m1, int a2, int m2 ){
	int x, y, c = Gcd( m1, m2 );
	if( abs(a1-a2)%c != 0 ) return -1;
	Ext_Gcd( m2/c, m1/c, x, y );
	long long xx = (long long)a2+(long long)m2*x*(a1-a2)/c;
	long long yy = (long long)m1*m2/c;
	xx %= yy;
	return ( xx<0 ? xx+yy:xx );
}

int main(){ // 2008 hefei: Discrete Square Roots
	int t = 0, x, n, r, a[100000], an;
	int prim[1<<16], pn, fn; long long f[1<<16];
	Get_Prim( prim, pn, (1<<16) );
	while( scanf("%d%d%d",&x,&n,&r)==3 && x+n+r ){
		Factor( (long long)n, pn, prim, fn, f );
		an = 0;
		for(int i=0; i<fn; i++){
			int n1 = f[i], n2 = n/f[i];
			long long R = Mod_L_EQ( r%n1, n1, (n2-r%n2)%n2, n2 );
			if( R == -1 || R>=n ) continue;
			int K = n/Gcd( n1, n2 );
			while( R < n ){ a[an++] = R; R += K; }
		}
		a[an++] = r;
		sort( a, a+an );
		printf("Case %d: %d",++t,a[0]);
		for(int i=1; i<an; i++) if( a[i]!=a[i-1] ) printf(" %d",a[i]);
		printf("\n");
	}
	return 0;
}

/*bool solve( int n, long long *a, long long x ){
	if( x==0 ) return a[n]!=0;
	for(int i=n; i>=1; i--){
		if( Abs(a[i])%Abs(x)!=0 ) return false;
		a[i-1] = a[i-1]+a[i]/x;
	}
	return a[0]==0;
}

int main(){ // poj 3471 Integral Roots
	const int N = 108;
	int n, prim[1<<16], pn;
	long long f[1<<16], x[N], a[N], b[N];
	int fn, xn;
	Get_Prim( prim, pn, (1<<16) );
	while( scanf("%d",&n)==1 ){
		a[0] = 1;
		for(int i=1; i<=n; i++) scanf("%I64d",&a[i]);
		xn = 0;
		while( a[n]==0 && n>=0 ){ x[xn++] = 0; n--; }
		Factor( Abs(a[n]), pn, prim, fn, f );
		
		for(int i=0; i<fn && n>0; i++){
			if( Abs(a[n])%f[i] != 0 ) continue;
			memcpy( b, a, sizeof(long long)*(n+1) );
			if( solve( n, b, f[i] ) ){
				x[xn++] = f[i];
				for(int j=0; j<=n-1; j++) a[j+1] -= a[j]*(-f[i]);
				n--; i--; continue;
			}
			memcpy( b, a, sizeof(long long)*(n+1) );
			if( solve( n, b, -f[i] ) ){
				x[xn++] = -f[i];
				for(int j=0; j<=n-1; j++) a[j+1] -= a[j]*f[i];
				n--; i--; continue;
			}
		}
		sort( x, x+xn );
		printf("%d\n", xn );
		for(int i=0; i<xn; i++) printf("%I64d\n",x[i]);
	}
	return 0;
}*/