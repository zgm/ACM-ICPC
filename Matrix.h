#include<stdlib.h>
#include<stdio.h>
#include<cstring>
#include<math.h>
#include<cmath>
#include<algorithm>
using namespace std;

int Mod = 9973;

class Matrix{
public:
	static void Add( int n, int *a, int *b ){ // a = a+b
		for(int i=0; i<n; i++) a[i] = (a[i]+b[i])%Mod;
	}
	static void Mul( int n, int *A, int *B, int *tmp ){ // a = a*b
		memset( tmp, 0, sizeof(int)*n*n );
		for(int i=0, *a=A, *t=tmp; i<n; i++, a+=n, t+=n){
			for(int j=0, *b=B; j<n; j++, b+=n){
				if( a[j] == 0 ) continue;
				for(int k=0; k<n; k++) t[k] = (t[k]+a[j]*b[k])%Mod;
			}
		}
		memcpy( A, tmp, sizeof(int)*n*n );
	}
	static void Power( int n, int *a, int p, int *t, int *tmp ){ // t = a^p
		if( p == 1 ) memcpy( t, a, sizeof(int)*n*n );
		else{
			Power( n, a, p>>1, t, tmp );
			Mul( n, t, t, tmp );
			if( p&1 ) Mul( n, t, a, tmp );
		}
	}
	// get t = a + a^2 + a^3 + ... + a^k
	/*static void Sequence( int n, int *a, int k, int *t, int *t1, int *t2, int *t3 ){
		if( k == 1 ) memcpy( t, a, sizeof(int)*n*n );
		else{
			Sequence( n, a, k>>1, t, t1, t2, t3 );
			if( k&1 ){
				Power( n, a, k/2+1, t2, t1 );
				memcpy( t3, t2, sizeof(int)*n*n );
				for(int i=0; i<n; i++) t3[i*n+i]++;
				Mul( n, t, t3, t1 );
				Add( n*n, t, t2 );
			}
			else{
				Power( n, a, k/2, t2, t1 );
				for(int i=0; i<n; i++) t2[i*n+i]++;
				Mul( n, t, t2, t1 );
			}
		}
	}*/
	// get t = I + a + a^2 + a^3 + ...+a^k
	// b = [ [A I]; [0 I] ], b^(k+1) = [ [A^(k+1)  A^k+...+A^2+A+I]; [0 I] ]
	static void Sequence( int n, int *a, int k, int *t ){
		int *b = new int[12*n*n], *s = b+4*n*n, *tmp = b+8*n*n;
		memset( b, 0, sizeof(int)*4*n*n );
		for(int i=0; i<n; i++) memcpy( b+2*n*i, a+n*i, sizeof(int)*n );
		for(int i=0; i<n; i++) b[2*n*i+n+i] = b[2*n*(n+i)+n+i] = 1;
		Power( 2*n, b, k+1, s, tmp );
		for(int i=0; i<n; i++) memcpy( t+n*i, s+2*n*i+n, sizeof(int)*n );
		delete []b;
	}
	/*static void ans(){ // poj 3233 Matrix Power Series
		const int N = 32;
		int n, k, m, a[N*N], t[N*N];
		while( scanf("%d%d%d",&n,&k,&m)==3 ){
			Mod = m;
			for(int i=0; i<n*n; i++) scanf("%d",&a[i]);
			Sequence( n, a, k, t );
			for(int i=0; i<n; i++){
				t[i*n+i] = (t[i*n+i]+Mod-1)%Mod;
				for(int j=0; j<n; j++){
					printf("%d",t[i*n+j]);
					if( j == n-1 ) printf("\n");
					else printf(" ");
				}
			}
		}
	}*/
	/*static void Init( int n, int *a, int k ){
		memset( a, 0, sizeof(int)*n*n );
		for(int i=0; i<=k; i++) a[i*n+i] = a[i*n+i+k+1] = 1;
		for(int i=k+1; i<2*k+1; i++){ a[i*n+i-k-1] = a[i*n+i+1] = 1; }
		a[(2*k+1)*n+k] = 1;
	}
	static void ans(){ // hust 1217
		int T, k, a[82*82], t[82*82], tmp[82*82];
		unsigned int p;
		scanf("%d",&T);
		while( T-- ){
			scanf("%d%d",&p,&k);
			int n = 2*k+2;
			Init( n, a, k );
			if( p == 2 ){
				if( k == 0 ) printf("3\n");
				else if( k == 1 ) printf("1\n");
				else printf("0\n");
				continue;
			}
			Power( n, a, p-2, t, tmp );
			int res = 2*t[k]+t[2*k+1]+2*t[(k+1)*n+k]+t[(k+1)*n+2*k+1];
			if( k >= 1 ) res += t[2*k]+t[(k+1)*n+2*k];
			printf("%d\n",(res)%9973);
		}
	}*/
	static bool LUP( int n, double *a, int *p ){
		for(int i=0; i<n; i++) p[i] = i;
		for(int k=0, r; k<n; k++){
			double q = 0;
			for(int i=k; i<n; i++)
				if( fabs( a[i*n+k] ) > q ){ q = fabs( a[i*n+k] ); r = i; }
			if( q < 1e-8 ) return false;
			swap( p[k], p[r] );
			for(int i=0; i<n; i++) swap( a[k*n+i], a[r*n+i] );
			for(int i=k+1; i<n; i++){
				a[i*n+k] /= a[k*n+k];
				for(int j=k+1; j<n; j++) a[i*n+j] -= a[i*n+k]*a[k*n+j];
			}
		}
		return true;
	}
	// get the answer A * x = b while A is n*n, b is n*1
	static bool LUPSolve( int n, double *A, double *b, double *x ){
		double *a = new double[n*n+n];
		memcpy( a, A, sizeof(double)*n*n );
		int *p = new int[n];
		if( !LUP( n, a, p ) ){ delete []a; delete []p; return false; }
		double *y = a+n*n;
		for(int i=0; i<n; i++){
			y[i] = b[p[i]];
			for(int j=0; j<i; j++) y[i] -= a[i*n+j]*y[j];
		}
		for(int i=n-1; i>=0; i--){
			x[i] = y[i];
			for(int j=i+1; j<n; j++) x[i] -= a[i*n+j]*x[j];
			x[i] /= a[i*n+i];
		}
		delete []a;
		delete []p;
		return true;
	}
	// get a answer A * x = B while A is n*m, B is n*1
	static bool Gauss( int n, int m, double *A, double *B, double *x, double eps=1e-8 ){
		double *a = new double[n*m+n], *b = a+n*m;
		memcpy( a, A, sizeof(double)*n*m );
		memcpy( b, B, sizeof(double)*n );
		int rank = 0;
		for(int i=0, r; i<m; i++){
			double q = 0;
			for(int j=rank; j<n; j++)
				if( fabs(a[j*m+i]) > q ){ q = fabs(a[j*m+i]); r = j; }
			if( fabs(q)<eps ) continue;
			
			for(int j=i; j<m; j++) swap( a[rank*m+j], a[r*m+j] );
			swap( b[rank], b[r] );
			//q = a[rank*m+i];
			//for(int j=i; j<m; j++) a[rank*m+j] /= q;
			//b[rank] /= q;
			
			for(int j=rank+1; j<n; j++){
				q = a[j*m+i]/a[rank*m+i];
				if( fabs(q)<eps ) continue;
				for(int k=i; k<m; k++) a[j*m+k] -= q*a[rank*m+k];
				b[j] -= q*b[rank];
			}
			rank++;
		}
		for(int i=rank; i<n; i++)
			if( fabs(b[i])>eps ){ delete []a; return false; }
		memset( x, 0, sizeof(double)*m );
		for(int i=rank-1; i>=0; i--){
			int j = i;
			for( ; j<m; j++) if( fabs(a[i*m+j])>eps ) break;
			x[j] = b[i]/a[i*m+j];
			for(int k=0; k<i; k++) b[k] -= a[k*m+j]*x[j];
		}
		delete []a;
		return true;
	}
	static void show( int n, int m, double *a ){
		for(int i=0; i<n; i++){
			for(int j=0; j<m; j++) printf("%.2f ",a[i*m+j]); printf("\n");
		}
	}


	/*static bool ok( int x, int y, int n, int m ){
		return 0<=x && x<n && 0<=y && y<m;
	}
	static void dfs( int x, int y, int n, int m, char *s, int *p, bool *vis, int k, double *A, double *B ){
		char c = s[x*m+y];
		int w = k*n*m+x*m+y, num = 0;
		if( vis[w] || c=='#' ) return;
		vis[w] = true;
		if( c>='a' && c<='z' ){
			A[w*3*n*m+k*n*m+p[c-'a']] = 1.0;
			dfs( p[c-'a']/m, p[c-'a']%m, n, m, s, p, vis, k, A, B );
			return;
		}
		if( c>'0' && c<='9' ) B[w] -= c-'0';
		int d[8] = {-1,0,0,1,1,0,0,-1};
		for(int i=0; i<8; i+=2){
			int u = x+d[i], v = y+d[i+1];
			if( ok(u,v,n,m) && s[u*m+v]!='#' ) num++;
		}
		if( num==0 ) return;
		for(int i=0; i<8; i+=2){
			int u = x+d[i], v = y+d[i+1];
			if( !ok(u,v,n,m) || s[u*m+v]=='#' ) continue;
			if( s[u*m+v]!='!' ){
				A[w*3*n*m+(k*n*m+u*m+v)] += 1.0/num;
				dfs( u, v, n, m, s, p, vis, k, A, B );
			}
			else if( k<2 ){
				A[w*3*n*m+(k+1)*n*m+u*m+v] += 1.0/num;
				dfs( u, v, n, m, s, p, vis, k+1, A, B );
			}
		}
	}
	static bool ok( int x, int y, int n, int m, char *s, int *p, bool *vis ){
		char c = s[x*m+y];
		if( c>'0' && c<='9' ) return true;
		vis[x*m+y] = true;
		if( c>='a' && c<='z' )
			return ok( p[c-'a']/m, p[c-'a']%m, n, m, s, p, vis );
		int d[8] = {-1,0,0,1,1,0,0,-1};
		for(int i=0; i<8; i+=2){
			int u = x+d[i], v = y+d[i+1];
			if( !ok(u,v,n,m) || s[u*m+v]=='#' || vis[u*m+v] ) continue;
			if( ok(u,v,n,m,s,p,vis) ) return true;
		}
		return false;
	}
	static void ans(){ // spoj 2324  Mario
		const int N = 15;
		int n, m, p[26], d[8] = {-1,0,0,1,1,0,0,-1};
		char s[N*N+N]; bool vis[3*N*N];
		double A[3*N*N*3*N*N], B[3*N*N], X[3*N*N];
		while( scanf("%d%d",&n,&m)==2 ){
			memset( p, -1, sizeof(int)*26 );
			int st = -1;
			for(int i=0; i<n; i++) scanf("%s",s+i*m);
			for(int i=0; i<n*m; i++){
				if( s[i]>='A' && s[i]<='Z' ) p[s[i]-'A'] = i;
				else if( s[i]=='$' ) st = i;
			}
			memset( A, 0, sizeof(double)*3*n*m*3*n*m );
			memset( B, 0, sizeof(double)*3*n*m );
			memset( vis, 0, sizeof(bool)*3*n*m );
			dfs( st/m, st%m, n, m, s, p, vis, 0, A, B );
			
			for(int i=0; i<n*m; i++){
				if( s[i]=='#' || (s[i]>'0' && s[i]<='9') ) continue;
				memset( vis, 0, sizeof(bool)*n*m );
				if( !ok(i/m,i%m,n,m,s,p,vis) ){
					memset( A+i*3*n*m, 0, sizeof(double)*3*n*m );
					memset( A+(i+n*m)*3*n*m, 0, sizeof(double)*3*n*m );
					memset( A+(i+2*n*m)*3*n*m, 0, sizeof(double)*3*n*m );
					B[i] = B[i+n*m] = B[i+2*n*m] = 0;
				}
			}
			for(int i=0; i<3*n*m; i++) A[i*3*n*m+i] -= 1.0;
			// need adding ( rank < n )
			if( Gauss( 3*n*m, 3*n*m, A, B, X, 1e-12 ) ) printf("%.9f\n",X[st]);
			else printf("-1\n");
		}
	}*/

	/*static int Next( int k, int n ){
		while( k<0 || k>n ){
			if( k<0 ) k = -k;
			else k = 2*n-k;
		}
		return k;
	}
	static void dfs( int u, bool *vis, int n, int *p ){
		vis[u] = true;
		if( u==n ) return;
		for(int i=1; i<=6; i++){
			int v = Next(u+i,n);
			if( p[v]>=0 ) v = p[v];
			if( !vis[v] ) dfs( v, vis, n, p );
		}
	}
	static void ans(){ // poj 3756  Chess Game
		const int N = 108;
		double A[N*N], B[N], X[N];
		int n, nf, nb, ns, u, v, p[N];
		bool vis[N];
		while( scanf("%d",&n)==1 ){
			for(int i=0; i<=n; i++) p[i] = -2;
			scanf("%d",&nf);
			while( nf-- ){ scanf("%d%d",&u,&v); p[u] = Next(u+v,n); }
			scanf("%d",&nb);
			while( nb-- ){ scanf("%d%d",&u,&v); p[u] = Next(u-v,n); }
			scanf("%d",&ns);
			while( ns-- ){ scanf("%d",&u); p[u] = -1; }

			memset( vis, 0, sizeof(bool)*(n+1) );
			dfs( 0, vis, n, p );
			if( !vis[n] ){ printf("Impossible\n"); continue; }
			
			memset( A, 0, sizeof(double)*(n+1)*(n+1) );
			memset( B, 0, sizeof(double)*(n+1) );
			for(int i=0; i<=n-1; i++){
				if( !vis[i] ) continue;
				for(int j=1; j<=6; j++){
					int u = Next(i+j,n), stop = 0;
					if( p[u]!=-2 && p[u]!=-1 ) u = p[u];
					else if( p[u]==-1 ) stop = 1;
					A[i*(n+1)+u] += 1.0/6; B[i] += (1.0+stop)/6;
				}
			}
			for(int i=0; i<=n; i++) A[i*(n+1)+i] -= 1;
			if( Gauss( n+1, n+1, A, B, X ) && X[0]<-1e-4 ){
				printf("%.2f\n",-X[0]);
			}
			else printf("Impossible\n");
		}
	}*/

	// ECNU  1994  Strange Couple

	// a 的最后n列为单位矩阵
	static bool Simplex( int n, int m, double *A, double *B, double *c, double *x, double eps=1e-8 ){
		int *p = new int[n];
		double *a = new double[n*m], *b = new double[n];
		memcpy( a, A, sizeof(double)*n*m );
		memcpy( b, B, sizeof(double)*n );
		for(int i=0; i<n; i++) p[i] = m-n+i;
		while( true ){
			//show( n, m, a );
			//show( n, 1, b );
			double mx = -1;
			int x = -1, y = -1;
			for(int j=0; j<m; j++){
				double cz = c[j];
				for(int i=0; i<n; i++) cz -= c[p[i]]*a[i*m+j];
				if( mx < cz ){ mx = cz; y = j; }
			}
			if( mx < eps ) break;
			mx = 1e20;
			for(int i=0; i<n; i++){
				if( a[i*m+y] < eps ) continue;
				if( mx > b[i]/a[i*m+y] ){ mx = b[i]/a[i*m+y]; x = i; }
			}
			if( mx > 1e18 ) return false;
			p[x] = y;
			double k = a[x*m+y];
			b[x] /= k;
			for(int i=0; i<m; i++) a[x*m+i] /= k;
			for(int i=0; i<n; i++){
				if( i == x ) continue;
				k = a[i*m+y];
				b[i] -= b[x]*k;
				for(int j=0; j<m; j++) a[i*m+j] -= a[x*m+j]*k;
			}
		}
		memset( x, 0, sizeof(double)*m );
		for(int i=0; i<n; i++) if( b[i] < -eps ) return false;
		for(int i=0; i<n; i++) x[p[i]] = b[i];
		//show( m, 1, x );
		delete []p;
		delete []a;
		delete []b;
		return true;
	}
	/*static void ans(){
		const int N = 108;
		int n, m;
		double a[N*N], b[N], c[N], x[N];
		while( scanf("%d%d",&n,&m)==2 ){
			for(int i=0; i<m; i++) scanf("%lf",&c[i]);
			for(int i=0; i<n; i++){
				for(int j=0; j<m; j++) scanf("%lf",&a[i*m+j]);
				scanf("%lf",&b[i]);
			}
			Simplex( n, m, a, b, c, x );
		}
	}*/
	//static void ans(){ // poj 3477 System of Linear Equations 没通过
	//	const int N = 208;
	//	int n, m;
	//	double a[N*N], b[N], c[N], x[N];
	//	const double eps = 1e-10;
	//	while( scanf("%d%d",&n,&m)==2 ){
	//		memset( c, 0, sizeof(double)*m );
	//		for(int i=m; i<m+n; i++) c[i] = -1;
	//		memset( a, 0, sizeof(double)*n*(n+m) );
	//		for(int i=0; i<n; i++){
	//			for(int j=0; j<m; j++) scanf("%lf",&a[i*(n+m)+j]);
	//			a[i*(n+m)+m+i] = 1.0;
	//			scanf("%lf",&b[i]);
	//			if( b[i] < -eps){
	//				for(int j=0; j<m; j++) a[i*(n+m)+j] = -a[i*(n+m)+j];
	//				b[i] = -b[i];
	//			}
	//		}
	//		if( !Simplex( n, m+n, a, b, c, x ) ){ printf("impossible\n"); continue; }

	//		int k = 0;
	//		for(k=m; k<m+n; k++) if( fabs(x[k])>eps ) break;
	//		if( k < m+n ){ printf("impossible\n"); continue; }
	//		/*for(k=0; k<n; k++){
	//			double s = b[k];
	//			for(int i=0; i<m; i++) s -= a[k*(m+n)+i]*x[i];
	//			if( fabs(s)>eps ) break;
	//		}*/
	//		//if( k < n ){ printf("impossible\n"); continue; }
	//		for(int i=0; i<m; i++){
	//			if( fabs(x[i])<eps ) printf("0.0000000000\n");
	//			else printf("%.10f\n",x[i]);
	//		}
	//	}
	//}
	/*static void ans(){ // poj 3707 High-Dimensional Vector Correspondence
		int n, m, s=0;
		const int N = 612;
		double *p = new double[N*N], *q = new double[N*N], *h = new double[N*N];
		double b[N];
		while( scanf("%d%d",&m,&n)==2 ){
			for(int i=0; i<n*m; i++) scanf("%lf",&p[i]);
			for(int i=0; i<n*m; i++) scanf("%lf",&q[i]);
			int k = 0;
			for( ; k<m; k++){
				for(int j=0; j<n; j++) b[j] = q[j*m+k];
				if( !Gauss( n, m, p, b, h+k*m ) ) break;
			}
			if( k < m ){ printf("*\n"); }
			else{
				printf("%d\n", m);
				for(int i=0; i<m; i++){
					for(int j=0; j<m; j++){
						if( j!= 0 ) printf(" ");
						printf("%.18lf",h[i*m+j]);
					}
					printf("\n");
				}
			}
		}
		delete []p;
		delete []q;
		delete []h;
	}*/
	static long long Determinant( int n, long long *a, long long Mod ){
		//for(int i=n*n-1; i>=0; i--) a[i] %= Mod;
		long long res = 1;
		for(int i=0; i<n; i++){
			for(int j=i+1; j<n; j++){
				while( a[j*n+i]!=0 ){
					long long t = a[i*n+i]/a[j*n+i];
					for(int k=i; k<n; k++) a[i*n+k] -= a[j*n+k]*t;
					for(int k=i; k<n; k++) swap( a[i*n+k], a[j*n+k] );
					res = -res;
				}
			}
			if( a[i*n+i]==0 ) return 0;
			res = res*a[i*n+i];
			//res %= Mod;
		}
		return res%Mod;
	}
};

int main(){
	Matrix::ans();
	return 0;
}