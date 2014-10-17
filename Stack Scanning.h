#include<iostream>
#include<algorithm>
#include<stack>
using namespace std;

struct Node{ int l, r, h; Node( int x=0, int y=0, int z=0){ l=x; r=y; h=z; } };
inline bool cmp( Node s, Node t ){ return ( s.l<t.l || (s.l==t.l&&s.r<t.r)); }

class LinearScanning{
public:
	// find the left and right element lower than a[i], ( -1 ... n )
	static void Get( int *a, int n, int *l, int *r ){
		int *q = new int[n], tail = -1;
		for(int i=0; i<n; i++){
			while( tail>-1 && a[q[tail]]>a[i] ){ r[q[tail]] = i; tail--; }
			if( tail == -1 || a[q[tail]]<=a[i] ){ q[++tail] = i; continue; }
		}
		while( tail>-1 ){ r[q[tail]] = n; tail--; }

		for(int i=n-1; i>=0; i--){
			while( tail>-1 && a[q[tail]]>a[i] ){ l[q[tail]] = i; tail--; }
			if( tail == -1 || a[q[tail]]<=a[i] ){ q[++tail] = i; continue; }
		}
		while( tail>-1 ){ l[q[tail]] = -1; tail--; }
		delete []q;
	}
	static void ans(){ // poj 2796 Feel Good
		const int N = 10008;
		int n, a[N], l[N], r[N];
		__int64 sum[N];
		while( scanf("%d",&n)==1 ){
			for(int i=1; i<=n; i++) scanf("%d",&a[i]);
			a[n+1] = 0;
			sum[0] = 0;
			for(int i=1; i<=n+1; i++) sum[i] = sum[i-1]+a[i];

			Get( a+1, n, l, r );
			__int64 res = -1; int rl = 1, rr = 1;
			for(int i=0; i<n; i++){
				__int64 val = (__int64)(sum[r[i]]-sum[l[i]+1])*a[i+1];
				if( res < val ){ res = val; rl = l[i]+2; rr = r[i]; }
			}
			/*stack<Node> s;
			for(int i=1; i<=n+1; i++){
				int l = i;
				while( !s.empty() && s.top().h>a[i] ){
					l = s.top().l;
					__int64 val = (__int64)(sum[s.top().r]-sum[s.top().l-1])*s.top().h;
					if( val > res ){
						res = val; rl = s.top().l; rr = s.top().r;
					}
					int r = s.top().r;
					s.pop();
					if( !s.empty() ) s.top().r = max( r, s.top().r );
				}
				if( s.empty() || s.top().h<a[i] ){ s.push( Node(l,i,a[i]) ); continue; }
				if( s.top().h == a[i] ){ s.top().r = i; continue; }
			}
			if( res == -1 ) res = 0;*/
			printf("%I64d\n%d %d\n",res, rl, rr);
		}
	}
	/*static void ans(){ // poj 3494 Largest Submatrix of All 1¡¯s
		int n, m, b[2008];
		bool a[2001][2001];
		while( scanf("%d%d",&n,&m)==2 ){
			for(int i=0; i<n; i++){
				for(int j=0; j<m; j++) scanf("%d",&a[i][j]);
			}
			for(int i=0; i<=m; i++) b[i] = 0;
			
			stack<Node> s;
			int res = 0;
			for(int i=0; i<n; i++){
				while( !s.empty() ) s.pop();
				for(int j=0; j<m; j++){
					if( a[i][j] ) b[j]++;
					else b[j] = 0;
				}
				for(int j=0; j<=m; j++){
					int l = j;
					while( !s.empty() && s.top().h>b[j] ){
						l = s.top().l;
						res = max( res, (s.top().r-s.top().l+1)*s.top().h);
						int r = s.top().r;
						s.pop();
						if( !s.empty() ) s.top().r = max( r, s.top().r );
					}
					if( s.empty() || s.top().h<b[j] ){ s.push( Node(l,j,b[j]) ); continue; }
					if( s.top().h == b[j] ){ s.top().r = j; continue; }
				}
			}
			printf("%d\n", res);
		}
	}*
	/*static void ans(){ // poj 2559 Largest Rectangle in a Histogram
		int n, a[100008];
		while( scanf("%d",&n)!=EOF && n!=0 ){
			for(int i=0; i<n; i++) scanf("%d",&a[i]);
			a[n] = 0;
			stack<Node> s;
			__int64 res = 0;
			for(int i=0; i<=n; i++){
				int l = i;
				while( !s.empty() && s.top().h>a[i] ){
					l = s.top().l;
					res = max( res, (__int64)(s.top().r-s.top().l+1)*s.top().h);
					int r = s.top().r;
					s.pop();
					if( !s.empty() ) s.top().r = max( r, s.top().r );
				}
				if( s.empty() || s.top().h<a[i] ){ s.push( Node(l,i,a[i]) ); continue; }
				if( s.top().h == a[i] ){ s.top().r = i; continue; }
			}
			printf("%I64d\n", res);
		}
	}*/
};

int main(){
	LinearScanning::ans();
    return 0;
}