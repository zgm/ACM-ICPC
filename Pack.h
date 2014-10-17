#include<iostream>
#include<cmath>
using namespace std;

class Pack{
public:
	// n: the number of goods, V: the capacity of pack, c[]: the cost, w[]: the value
	static int ZeroOne( int n, int V, int *c, int *w){
		int *p = new int[V+1], res = 0;
		memset( p, 0, sizeof(int)*(V+1) );
		for(int i=0; i<n; i++){
			for(int j=V; j>=c[i]; j--) p[j] = max( p[j], p[j-c[i]]+w[i] );
		}
		for(int i=0; i<=V; i++) res = max( res, p[i] );
		delete []p;
		return res;
	}
	static int FullPack( int n, int V, int *c, int *w){
		int *p = new int[V+1], res = 0;
		memset( p, 0, sizeof(int)*(V+1) );
		for(int i=0; i<n; i++){
			for(int j=c[i]; j<=V; j++) p[j] = max( p[j], p[j-c[i]]+w[i] );
		}
		for(int i=0; i<=V; i++) res = max( res, p[i] );
		delete []p;
		return res;
	}
	static int MultiPack1( int n, int V, int *nm, int *c, int *w){
		int *p = new int[V+1], res = 0;
		memset( p, 0, sizeof(int)*(V+1) );
		for(int i=0; i<n; i++){
			int k;
			for(k=0; (1<<k)-1<=nm[i]; k++); k--;
			for(int j=0; j<k; j++){
				for(int r=V, t=(1<<j); r>=t*c[i]; r--) p[r] = max( p[r], p[r-t*c[i]]+t*w[i] );
			}
			k = nm[i]-(1<<k)+1;
			if( k>0 ) for(int j=V; j>=k*c[i]; j--) p[j] = max( p[j], p[j-k*c[i]]+k*w[i] );
		}
		for(int i=0; i<=V; i++) res = max( res, p[i] );
		delete []p;
		return res;
	}
	// 单调队列优化
	static int MultiPack( int n, int V, int *nm, int *c, int *w ){ // no test
		int *p = new int[V+1], *pp = new int[V+1], res = 0;
		int *q = new int[V+1], *qq = new int[V+1];
		memset( p, 0, sizeof(int)*(V+1) );
		for(int i=0; i<n; i++){
			for(int j=0; j<c[i]; j++){
				int L = 0, R = 0;
				for(int k=0; k<=(V-j)/c[i]; k++){
					int b = p[k*c[i]+j]-k*w[i];
					while( R>L && qq[R-1]<b ) R--;
					q[R] = k; qq[R++] = b;
					if( q[L] < k-nm[i] ) L++;
					pp[k*c[i]+j] = qq[L]+k*w[i];
				}
			}
			memcpy( p, pp, sizeof(int)*(V+1) );
		}
		for(int i=0; i<=V; i++) res = max( res, p[i] );
		delete []p;
		delete []pp;
		delete []q;
		return res;
	}
	static void ans(){
		freopen("C:\\Documents and Settings\\zgmcn\\桌面\\acm.in", "r", stdin );
		freopen("C:\\Documents and Settings\\zgmcn\\桌面\\acm.out", "w", stdout );
		const int N = 10008;
		int n, V, num[N], c[N], w[N];
		while( scanf("%d%d",&n,&V)==2 ){
			for(int i=0; i<n; i++) scanf("%d %d %d",&num[i],&c[i],&w[i]);
			printf("%d\n",MultiPack1( n, V, num, c, w ) );
			printf("%d\n",MultiPack( n, V, num, c, w ) );
		}
	}
	//
	static int MultiPack_EqualZero( int n, int V, int *nm, int *c, int *w, int *p, bool *vis ){
		memset( p, 0, sizeof(int)*(V+1) );
		memset( vis, false, sizeof(bool)*(V+1) );
		vis[0] = true;
		for(int i=0, k; i<n; i++){
			for(k=0; (1<<k)-1<=nm[i]; k++); k--;
			for(int j=0; j<k; j++){
				for(int r=V, t=(1<<j); r>=t*c[i]; r--){
					if( !vis[r-t*c[i]] ) continue;
					p[r] = max( p[r], p[r-t*c[i]]+t*w[i] );
					vis[r] = true;
				}
			}
			k = nm[i]-(1<<k)+1;
			if( k <= 0 ) continue;
			for(int j=V; j>=k*c[i]; j--){
				if( !vis[j-k*c[i]] ) continue;
				p[j] = max( p[j], p[j-k*c[i]]+k*w[i] );
				vis[j] = true;
			}
		}
		return p[V];
	}
	/*static void ans(){ // poj 2754 Similarity of necklaces 2
		const int N = 208, M = 100008 ;
		int n, p[N], m[N], l[N], u[N], nm[N], P[M];
		bool vis[M];
		while( scanf("%d",&n)==1 ){
			for(int i=0; i<n; i++) scanf("%d%d%d%d",&p[i],&m[i],&l[i],&u[i]);
			int V = 0, res = 0;
			for(int i=0; i<n; i++){ V += m[i]*l[i]; res += p[i]*l[i]; }
			V = 0-V;
			for(int i=0; i<n; i++) nm[i] = u[i]-l[i];
			printf("%d\n",res+MultiPack_EqualZero( n, V, nm, m, p, P, vis ));
		}
	}*/
};

int main(){
	Pack::ans();
	return 0;
}