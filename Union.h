#include<iostream>
#include<algorithm>
#include<time.h>
#include<vector>
#include<cstdio>
using namespace std;

struct Point{
	int x, y, w;
	Point(int X=0, int Y=0, int W=0){ x=X; y=Y; w=W; }
};
inline bool cmp( Point s, Point t){ return s.w < t.w; }

class UF{
public:
	static int Find( int u, int *p ){
		if( p[u] < 0 ) return u;
		else{ p[u] = Find(p[u], p); return p[u]; }
	}
	static void Union( int u, int v, int *p ){
		int pu = Find( u, p ), pv = Find( v, p );
		if( pu != pv ){
			int tmp = p[pu]+p[pv];
			if( p[pu] > p[pv] ){ p[pu] = pv; p[pv] = tmp; }
			else{ p[pv] = pu; p[pu] = tmp; }
		}
	}
	static int Kruscal( int n, int m, Point *P ){
		sort( P, P+m, cmp );
		int *p = new int[n+1];
		memset( p, -1, sizeof(int)*(n+1) );
		int k = 0, u, v, res = 0;
		for(int i=1; i<n; i++){
			for( ; k<m; k++){
				u = P[k].x; v = P[k].y;
				if( Find(u, p) == Find(v, p) ) continue;
				res += P[k].w; // k++ may be better
				Union( u, v, p );
				break;
			}
		}
		delete []p;
		printf("%d\n", P[k].w );
		return res;
	}
	static void ans(){ // poj 2912  Rochambeau
		const int N = 508, M = 2008;
		int n, m, p[N], pre[N], next[N];
		int u[M], v[M]; char c[M];
		while( scanf("%d%d",&n,&m)==2 ){
			for(int i=0; i<m; i++){
				scanf("%d%c%d",&u[i],&c[i],&v[i]);
				if( c[i]=='<' ){ c[i]='>'; swap( u[i], v[i] ); }
			}
			int id = -1, result = -1, num = 0;
			for(int judge=0; judge<n; judge++){
				memset( p, -1, sizeof(int)*n );
				memset( pre, -1, sizeof(int)*n );
				memset( next, -1, sizeof(int)*n );
				bool ok = true;
				for(int i=0; i<m; i++){
					if( u[i]==judge || v[i]==judge ) continue;
					int x = Find( u[i], p ), y = Find( v[i], p );
					int pre_root = -1, cur_root = -1, next_root = -1;
					if( c[i]=='=' ){
						if( pre[x]!=-1 && pre[y]!=-1 ) Union( pre[x], pre[y], p );
						if( pre[x]!=-1 ) pre_root = Find( pre[x], p );
						else if( pre[y]!=-1 ) pre_root = Find( pre[y], p );

						Union( x, y, p );
						cur_root = Find( x, p );

						if( next[x]!=-1 && next[y]!=-1 ) Union( next[x], next[y], p );
						if( next[x]!=-1 ) next_root = Find( next[x], p );
						else if( next[y]!=-1 ) next_root = Find( next[y], p );
					}
					else{
						if( pre[x]!=-1 && next[y]!=-1 ) Union( pre[x], next[y], p );
						if( pre[x]!=-1 ) pre_root = Find( pre[x], p );
						else if( next[y]!=-1 ) pre_root = Find( next[y], p );

						if( pre[y]!=-1 ) Union( x, pre[y], p );
						cur_root = Find( x, p );

						if( next[x]!=-1 ) Union( next[x], y, p );
						next_root = Find( y, p );
					}
					if( pre_root==cur_root || cur_root==next_root
						|| (pre_root==next_root && pre_root!=-1) ){
							ok = false; id = max( id, i ); break;
					}
					
					pre[cur_root] = pre_root;
					next[cur_root] = next_root;

					if( pre_root!=-1 ){
						pre[pre_root] = next_root;
						next[pre_root] = cur_root;
					}
					if( next_root!=-1 ){
						pre[next_root] = cur_root;
						next[next_root] = pre_root;
					}
				}
				if( ok ){ result = judge; num++; }
			}
			if( num==1 ) printf("Player %d can be determined to be the judge after %d lines\n",result,id+1);
			else if( num>1 ) printf("Can not determine\n");
			else printf("Impossible\n");
		}
	}
	/*static void ans(){ // poj 1703  Find them, Catch them
		const int N = 100008;
		int t, n, m, p[N], opt[N]; char s[10];
		scanf("%d",&t);
		while( t-- ){
			scanf("%d%d",&n,&m);
			memset( p, -1, sizeof(int)*n );
			memset( opt, -1, sizeof(int)*n );
			int x, y;
			while( m-- ){
				scanf("%s%d%d",s,&x,&y);
				x = Find( x-1, p );
				y = Find( y-1, p );
				if( s[0]=='A' ){
					if( x==y ) printf("In the same gang.\n");
					else if( x==opt[y] || opt[x]==y ) printf("In different gangs.\n");
					else printf("Not sure yet.\n");
				}
				else{
					if( opt[x]!=-1 ) Union( opt[x], y, p );
					if( opt[y]!=-1 ) Union( x, opt[y], p );
					x = Find( x, p );
					y = Find( y, p );
					opt[x] = y;
					opt[y] = x;
				}
			}
		}
	}*/
	/*static void ans(){ // poj 2395 Out of Hay
		int n, m; Point P[15008];
		while( 2==scanf("%d%d",&n,&m) ){
			for(int i=0; i<m; i++) scanf("%d%d%d",&P[i].x,&P[i].y,&P[i].w);
			Kruscal( n, m, P );
		}
	}*/

	/*struct Point{ int x, y; Point(int a=0, int b=0){ x=a; y=b; } };
	static bool Ok( int x, int y, int d ){
		return x*x+y*y<=d*d;
	}
	static void ans(){ // poj 2236 Wireless Network
		Point P[1008];
		int n, d, p[1008];
		bool vis[1008];
		vector<int> vt[1008];
		while( scanf("%d%d",&n,&d)==2 ){
			for(int i=0; i<n; i++) scanf("%d%d",&P[i].x,&P[i].y);
			memset( vis, 0, sizeof(bool)*n );
			Init( n, p );
			for(int i=0; i<n; i++){
				for(int j=0; j<n; j++){
					if( i == j ) continue;
					if( Ok( P[i].x-P[j].x, P[i].y-P[j].y, d ) ) vt[i].push_back( j );
				}
			}

			char c; int u, v; getchar();
			while( (c=getchar())!=EOF ){
				if( c == 'O' ){
					scanf("%d",&u); u--;
					vector<int> tmp;
					for(int i=0; i<vt[u].size(); i++){
						if( vis[vt[u][i]] ){
							Union( u, vt[u][i], p );
							tmp.push_back( i );
						}
					}
					for(int i=tmp.size()-1; i>=0; i--) vt[u].erase( vt[u].begin()+tmp[i] );
					vis[u] = 1;
				}
				else{
					scanf("%d %d",&u,&v); u--; v--;
					if( Find(u,p) == Find(v,p) ) printf("SUCCESS\n");
					else printf("FAIL\n");
				}
				getchar();
			}
		}
	}*/
};

int main(){
	UF::ans();
	return 0;
}
