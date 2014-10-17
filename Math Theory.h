#include<iostream>
#include<math.h>
#include<cmath>
#include<map>
#include<algorithm>
using namespace std;

double const PI = 3.1415926535897932384626433832795;

class Math{
public:
	// 1+3+5+......+(2n-1) = n*n
	// 1*1!+2*2!+3*3!+......+n*n! = (n+1)!-1
	// 1^2-2^2+3^2-......+(-1)^n*n^2 = (-1)^(n+1)*n*(n+1)/2
	// 2^2+4^2+6^2+......+(2n)^2 = 2n*(n+1)*(2n+1)/3
	// 1/2!+2/3!+......+n/(n+1)! = 1-1/(n+1)!

	// 1*2+2*3+3*4+......+n*(n+1) = n*(n+1)*(n+2)/3
	// 1*2*3+2*3*4+......+n(n+1)(n+2) = n*(n+1)*(n+2)*(n+3)/4
	// 1^2+3^2+5^2+......+(2n-1)^2 = n*(4n^2-1)/3
	// 1^3+3^3+5^3+......+(2n-1)^3 = n^2*(2n^2-1)
	
	// get the sum of sequence "1^k + 2^k + 3^k +...+ n^k"
	// 1^1+2^1+3^1+......+n^1 = n(n+1)/2
	// 1^2+2^2+3^2+......+n^2 = n(n+1)(2n+1)/6
	// 1^3+2^3+3^3+......+n^3 = ( 1+2+3+......+n)^2 = n^2*(n+1)^2/4
	// 1^4+2^4+3^4+......+n^4 = n(n+1)(2n+1)(3n^2+3n-1)/30
	// 1^5+2^5+3^5+......+n^5 = n^2 (n+1)^2 (2n^2+2n-1)/12
	static int Sum( int n, int k ){
		if( k == 1 ) return n*(n+1)/2;
		if( k == 2 ) return n*(n+1)*(2*n+1)/6;
		if( k == 3 ) return n*n*(n+1)*(n+1)/4;
		if( k == 4 ) return n*(n+1)*(2*n+1)*(3*n*n+3*n-1)/30;
		if( k == 5 ) return n*n*(n+1)*(n+1)*(2*n*n+2*n-1)/12;
	}
	static void GetPrim( int *prim, int &pn, int n ){
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
	/*static void ans(){ // poj 3604 Professor Ben
		int t, n, prim[2300], pn;
		GetPrim( prim, pn, 2300 );
		scanf("%d",&t);
		while( t-- ){
			scanf("%d",&n);
			int res = 1, num;
			for(int i=0; i<pn && prim[i]<n; i++){
				for(num = 0; n>1 && n%prim[i]==0; n/=prim[i], num++);
				if( num != 0 ) res *= Sum( num+1, 3 );
			}
			if( n != 1 ) res *= Sum( 2, 3 );
			printf("%d\n", res);
		}
	}*/
	/*static void ans(){ // poj 2480 Longge's problem
		unsigned int n; int prim[100008], pn;
		while( scanf("%d",&n)!=EOF ){
			int k = sqrt( (double)n );
			GetPrim( prim, pn, k+1 );
			__int64 res = 1;
			for(int i=0; i<pn; i++){
				int num = 0; unsigned int tmp = 1;
				for( ; n>1 && n%prim[i]==0; n/=prim[i], tmp*=prim[i], num++);
				if( tmp != 1 ) res *= ((__int64)num*(tmp-tmp/prim[i])+tmp);
				if( n == 1 ) break;
			}
			if( n != 1 ) res *= (2*n-1);
			printf("%I64d\n", res);
		}
	}*/
	static int Mobius( int t ){
		int s = 1;
		for(int i=2; i*i<=t; i++){
			if( t%i ) continue;
			t /= i;
			if( t%i ) s = -s;
			else return 0;
		}
		if( t!=1 ) s = -s;
		return s;
	}
	static void ans(){ // sgu 370  Rifleman
		const int N = 1000008;
		int n, m, pn = 0, prim[1008], mobius[N];
		GetPrim( prim, pn, 1000 );
		while( scanf("%d%d",&n,&m)==2 ){
			if( n==1 && m==1 ){ printf("0\n"); continue; }
			if( n==1 || m==1 ){ printf("1\n"); continue; }
			long long result = 2;
			n--; m--;
			if( n>m ) swap( n, m );
			mobius[0] = 0; mobius[1] = 1;
			for(int i=2; i<=n; i++){
				int t = i, j = 0;
				for( ;j<pn && t%prim[j] && prim[j]*prim[j]<=t; j++);
				if( j<pn && t%prim[j]==0 ){
					t /= prim[j];
					if( t%prim[j]==0 ) mobius[i] = 0;
					else mobius[i] = -mobius[t];
				}
				else mobius[i] = -1;
				/*int t = i, j = 2;
				for( ; j*j<=t && t%j; j++);
				if( t%j==0 ){
					t /= j;
					if( t%j==0 ) mobius[i] = 0;
					else mobius[i] = -mobius[t];
				}
				else mobius[i] = -1;*/
			}
			for(int i=1; i<=n; i++) mobius[i] += mobius[i-1];
			for(int i=1; i<=n; i++){
				int step = min( n/(n/i), m/(m/i) )-i;
				result += (long long)(mobius[i+step]-mobius[i-1])*(n/i)*(m/i);
				i += step;
				//result += (long long)mobius[i]*(n/i)*(m/i);
			}
			cout<<result<<endl;
		}
	}

	static int Gcd( int a, int b ){
		if( a < b ) swap( a, b );
		while( b!=0 ){ swap( a, b ); b %= a; }
		return a;
	}
	// calculate gcd(a,b) and the integer x and y where gcd(a,b)=a*x+b*y
	static int Ext_Gcd( int a, int b, int &x, int &y ){
		if( b == 0 ){ x = 1; y = 0; return a; }
		int d = Ext_Gcd( b, a%b, x, y );
		int t = x;  x = y;  y = t-a/b*y;
		return d;
	}
	// calculate x where a^x = b (mod n ) and n is not / is prime
	static int BabyStep( int a, int b, int n ){
		map<int,int> mp;
		int D = 1%n, d = 0;
		for(int i=0,t=1%n; i<=100; t=(long long)t*a%n,i++) if( t==b )  return i;
		for(int t; (t=Gcd(a,n))!=1; ){
			if( b%t ) return -1;
			d++; n /= t; b /= t; D = (long long)D*a/t%n;
		}
		int m = (int)ceil( sqrt((double)n) ), am = powmod( a, m, n );
		for(int t=1%n,i=0; i<=m; t=(long long)t*a%n,i++){
			if( mp.find(t)==mp.end() ) mp[t] = i;
		}
		for(int i=0,x,y; i<=m; D=(long long)D*am%n,i++){
			Ext_Gcd( D, n, x, y );
			x = (long long)b*x%n;
			if( x<0 ) x += n;
			if( x>0 && mp.find(x)!=mp.end() ) return i*m+mp[x]+d;
		}
		/*map<int,int> mp;
		int m = (int)ceil( sqrt((double)n) ), am = powmod( a, m, n );
		for(int i=1,t=1; i<m; t=(long long)t*a%n,i++){
			if( mp.find(t)==mp.end() ) mp[t] = i;
		}
		for(int i=0,t=1,x,y; i<=m; t=(long long)t*am%n,i++){
			Ext_Gcd( t, n, x, y );
			x = (long long)b*x%n;
			if( x<0 ) x += n;
			if( x>0 && mp.find(x)!=mp.end() ) return i*m+mp[x];
		}*/
		return -1;
	}
	/*static void ans(){ // hdu 2815  Mod Tree
		int a, b, n;
		while( scanf("%d%d%d",&a,&n,&b)!=EOF ){
			if( b>=n ){ printf("Orz,I can’t find D!\n"); continue; }
			int x = BabyStep( a, b, n );
			if( x<0 ) printf("Orz,I can’t find D!\n");
			else printf("%d\n",x);
		}
	}*/
	// calculate x where a*x = b (mod n )
	static bool Mod_L_EQ( int a, int b, int n ){
		int x, y, d = Ext_Gcd( a, n, x, y );
		if( b%d > 0 ) return false; // No Answer
		int e = (x*(b/d))%n;
		for(int i=0; i<d; i++) printf("%dth answer is: %d\n",i+1, (e+i*(n/d))%n);
		return true;
	}
	// get the smallest positive answer x where x % m1 = a1 and x % m2 = a2
	static int Mod_L_EQ( int a1, int m1, int a2, int m2 ){
		int x, y, c = Gcd( m1, m2 );
		if( abs(a1-a2)%c != 0 ) return -1;
		Ext_Gcd( m2/c, m1/c, x, y );
		x = a2+m2*(x*(a1-a2)/c);
		y = m1*m2/c;
		x %= y;
		return ( x<0 ? x+y:x );
	}
	// calculate r where r % w[0]=a[0], r % w[1]=a[1], ... , r % w[n-1]=a[n-1]
	static int China( int n, int *a, int *w ){
		int k = 1, r = 0;
		for(int i=0; i<n; i++) k *= w[i];
		for(int i=0; i<n; i++){
			int m = k/w[i], x, y;
			Ext_Gcd( w[i], m, x, y );
			r = (r+y*m*a[i])%k;
		}
		return ( r>0 ? r:r+k );
	}
	// 将w分解成连分数数组a[]
	static int Fraction( double w, int *a ){
		int n = 0;
		while( true ){
			a[n] = (int)w;
			w -= a[n++];
			if( fabs(w)<1e-8 || n>100 ) break;
			w = 1/w;
		}
		return n;
	}
	// 通过连分数数组a[]计算w
	static double Fraction( int n, int *a ){
		double w = a[n-1];
		for(int i=n-2; i>=0; i--) w = a[i]+1/w;
		return w;
	}
	// 求解Pell方程x^2-N*y^2=1的最小解(x,y)
	static void PellEquation( int N, int &x, int &y ){
		int p0 = 0, p1 = 1, p2, q0 = 1, q1 = 0, q2;
		int a0 = (int)sqrt((double)N), a1;
		int g0 = 0, g1, h0 = 1, h1;
		for(int i=0, a=a0; ;i++){
			g1= -g0+a0*h0;
			h1 = (N-g1*g1)/h0;
			a1 = (int)((g1+a)/h1);
			p2 = a0*p1+p0;
			q2 = a0*q1+q0;
			if( p2*p2-N*q2*q2==1 ){ x=p2; y=q2; return; }
			p0 = p1; p1 = p2;
			q0 = q1; q1 = q2;
			a0 = a1; g0 = g1; h0 = h1;
		}
	}

	static bool Witness( int a, int n ){
		int d = 1;
		for(int i=(int)ceil( log((double)n-1)/log(2.0))-1; i>=0; i--){
			int x = d;
			d = (d*d)%n;
			if( (d==1) && (x!=1) &&(x!=n-1) ) return true;
			if( ((n-1)&(1<<i)) > 0 ) d = (d*a)%n;
		}
		return ( d==1 ? 0:1 );
	}
	// judge the integer n is or not prime
	static bool Miller_Rabin( int n, int time ){
		for(int i=0; i<time; i++){
			int a = rand()%(n-2)+2;
			if( Witness( a, n ) ) return false;
		}
		return true;
	}
	// A^B % C  = A^( B%Phi(C) + Phi(C) ) % C   while B>=Phi(C)
	static long long powmod( long long x, long long n, long long Mod ){ // x^n % Mod
		long long s = 1, t = x;
		while( n!=0 ){
			if( n%2 ) s = (s*t)%Mod;
			n/=2; t = (t*t)%Mod;
		}
		return s%Mod;
	}
	// 求x的逆元，一种方法为 x^(p-2) % p
	static long long inv( long long x, long long Mod ){ return powmod( x, Mod-2, Mod ); }
	// calculate C(n,k)%Mod while Mod is prime
	static long long C( long long n, long long k, long long Mod ){
		long long s = 1;
		for(int i=0; i<k; i++) s = (s*(n-i))%Mod;
		for(int i=0; i<k; i++) s = (s*inv(i+1,Mod))%Mod;
		return s;
	}
	static int factmod( int n, int p ){ // n! % p
		long long res = 1;
		while( n>1 ){
			long long cur = 1;
			for(int i=2; i<p; i++) cur = (cur*i)%p;
			res = (res*powmod(cur,n/p,p))%p;
			for(int i=2; i<=n%p; i++) res = (res*i)%p;
			n /= p;
		}
		return int(res);
	}
	static void div( int n, int *num, int pn, int *prim ){ // 用素数来表示 n！
		memset( num, 0, sizeof(int)*pn );
		for(int i=0; i<pn && prim[i]<=n; i++){
			for(int s=n; s/prim[i]; s/=prim[i]) num[i] += s/prim[i];
		}
	}
	/*static void ans(){  // hdu  3398  String
		const int N = 1000001, Mod = 20100501;
		int pn = 0, *prim = new int[N];
		GetPrim( prim, pn, 2*N );

		int t, n, m, num[N], r[N];
		scanf("%d",&t);
		while( t-- ){
			scanf("%d%d",&n,&m);
			div( n+m, num, pn, prim );
			div( n, r, pn, prim );
			for(int i=0; i<pn; i++) num[i] -= r[i];
			div( m, r, pn, prim );
			for(int i=0; i<pn; i++) num[i] -= r[i];
			long long s = 1;
			for(int i=0; i<pn; i++){
				if( num[i] ) s = (s*powmod(prim[i],num[i],Mod))%Mod;
			}

			div( n+m, num, pn, prim );
			div( n+1, r, pn, prim );
			for(int i=0; i<pn; i++) num[i] -= r[i];
			div( m-1, r, pn, prim );
			for(int i=0; i<pn; i++) num[i] -= r[i];
			long long t = 1;
			for(int i=0; i<pn; i++){
				if( num[i] ) t = (t*powmod(prim[i],num[i],Mod))%Mod;
			}
			printf("%d\n",(s-t+Mod)%Mod);
		}
	}*/
};

int main(){
	Math::ans();
	return 0;
}
