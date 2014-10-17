#include<iostream>
#include<cmath>
using namespace std;

class Graph{
public:
	static int StoerWagner( int N, int *w ){
		int n = N, mn = (1<<28), *v = new int[N], *d = new int[N];
		bool *vis = new bool[N];
		for(int i=0; i<n; i++) v[i] = i;
		while( n > 1 ){
			memset( vis, 0, sizeof(bool)*n );
			memset( d, -1, sizeof(int)*n );
			for(int i=0; i<n; i++) d[i] = w[v[0]*N+v[i]];
			vis[0] = true;
			for(int i=1, cur=0; i<=n-1; i++){
				int mx = -1, pre = cur;
				for(int j=0; j<n; j++){
					if( !vis[j] && d[j]>mx ){ mx = d[j]; cur = j; }
				}
				if( mx == -1 ) return 0;
				vis[cur] = true;
				if( i == n-1 ){
					mn = min( mn, d[cur] );
					for(int j=0; j<n; j++){
						w[v[j]*N+v[pre]] = (w[v[pre]*N+v[j]] += w[v[cur]*N+v[j]]);
					}
					v[cur] = v[--n];
				}
				else{
					for(int j=0; j<n; j++){
						if( !vis[j] ) d[j] += w[v[cur]*N+v[j]];
					}
				}
			}
		}
		delete []vis;
		delete []d;
		delete []v;
		return mn;
	}
	static void ans(){ // poj 2914 Minimum Cut
		int n, m, w[500*500];
		while( scanf("%d%d",&n,&m)==2 ){
			int u, v, c;
			memset( w, 0, sizeof(int)*n*n );
			for(int i=0; i<m; i++){
				scanf("%d%d%d",&u,&v,&c);
				w[u*n+v] += c;
				w[v*n+u] += c;
			}
			printf("%d\n", StoerWagner( n, w ) );
		}
	}
};

int main(){
	Graph::ans();
	return 0;
}