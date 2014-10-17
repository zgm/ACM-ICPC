#include<iostream>
#include<algorithm>
#include<stdlib.h>
#include<stdio.h>
#include<string>
#include<cstring>
#include<time.h>
using namespace std;

const int INF = (1<<28);

struct Link{
	int v, next, w;
	void set( int V=-1, int Next=-1, int W=-1){ v=V; next=Next; w=W; }
};
class List{
public:
	int end; Link *V;
	List( int size = 1024 ){ V = new Link[size]; }
	~List(){ delete []V; }	
	void init( int n ){ end = n; for(int i=0; i<n; i++) V[i].next = -1; }
	void insert( int u, int v, int w = -1 ){
		V[end].set( v, V[u].next, w );
		V[u].next = end++;
	}
};

class Tree{
public:
	// bfs以及dfs求树的中心点，使得删除该点后所得所有树的最大节点数最小
	/*static void Root( int u, Link *V, int n, int &root, int &mxson ){
		int *size = new int[3*n], qn = 0, *q = size+n, *next = size+2*n;
		bool *vis = new bool[n];
		memset( vis, 0, sizeof(bool)*n );
		for(int i=0; i<n; i++) next[i] = V[i].next;
		for(q[++qn]=u,vis[u]=true; qn>0; ){
			u = q[qn];
			int i, mx = -1;
			for(i=next[u]; i!=-1; i=V[i].next){
				int v = V[i].v;
				if( !vis[v] ){ vis[v] = true; q[++qn] = v; break; }
			}
			next[u] = (i==-1 ? -1:V[i].next);
			if( i!=-1 ) continue;
			size[u] = 1;
			for(i=V[u].next; i!=-1; i=V[i].next){
				int v = V[i].v;
				if( !vis[v] ){ mx = max( mx, size[v] ); size[u] += size[v]; }
			}
			mx = max( mx, n-size[u] );
			if( mx<mxson ){ mxson = mx; root = u; }
			vis[u] = false;
			--qn;
		}
		delete []size;
		delete []vis;
	}
	static int Root( int u, int p, int n, Link *V, bool *vis, int &root, int &mxson ){
		int size = 1, mx = -1;
		for(int i=V[u].next; i!=-1; i=V[i].next){
			int v = V[i].v;
			if( v==p || vis[v] ) continue;
			int num = Root( v, u, n, V, vis, root, mxson );
			mx = max( mx, num );
			size += num;
		}
		mx = max( mx, n-size );
		if( mx<mxson ){ mxson = mx; root = u; }
		return size;
	}
	static void ans(){ // poj 1655 Balancing Act
		const int N = 20008;
		int t, n;
		List s( 3*N );
		scanf("%d",&t);
		while( t-- ){
			scanf("%d",&n);
			s.init( n );
			for(int i=1,u,v; i<n; i++){
				scanf("%d%d",&u,&v);
				s.insert( u-1, v-1 );
				s.insert( v-1, u-1 );
			}
			int root, mxson = n;
			//memset( vis, 0, sizeof(bool)*n );
			//Root( 0, -1, n, s.V, vis, root, mxson );
			Root( 0, s.V, n, root, mxson );
			printf("%d %d\n",root+1,mxson);
		}
	}*/


	static void Union( int n, int *s, int m, int *t, int *tmp ){
		for(int l=0,r=0,k=0; l<n||r<m; k++){
			if( r>=m || (l<n&&s[l]<t[r]) ) tmp[k] = s[l++];
			else tmp[k] = t[r++];
		}
		memcpy( s, tmp, sizeof(int)*n );
		memcpy( t, tmp+n, sizeof(int)*m );
	}
	// s,t升序,统计个数{(i,j) | s[i]+t[j]<=k}
	static int Sum( int n, int *s, int m, int *t, int k ){
		int num = 0;
		for(int i=0,j=m-1; i<n; i++){
			while( j>=0 && s[i]+t[j]>k ) j--;
			if( j<0 ) break;
			num += j+1;
		}
		return num;
	}
	static int Size( int u, int p, Link *V, bool *vis ){
		int size = 1;
		for(int i=V[u].next; i!=-1; i=V[i].next){
			int v = V[i].v;
			if( v!=p && !vis[v] ) size += Size( v, u, V, vis );
		}
		return size;
	}
	static int Root( int u, int p, int n, Link *V, bool *vis, int &root, int &mxson ){
		int size = 1, mx = -1;
		for(int i=V[u].next; i!=-1; i=V[i].next){
			int v = V[i].v;
			if( v==p || vis[v] ) continue;
			int num = Root( v, u, n, V, vis, root, mxson );
			mx = max( mx, num );
			size += num;
		}
		mx = max( mx, n-size );
		if( mx<mxson ){ mxson = mx; root = u; }
		return size;
	}
	static int GetRoot( int u, int n, Link *V, bool *vis ){
		int root, mxson = n;
		Root( u, -1, n, V, vis, root, mxson );
		return root;
	}
	static void dfs( int u, int p, Link *V, int K, bool *vis, int dis, int &dn, int *d ){
		if( dis>K ) return;
		d[dn++] = dis;
		for(int i=V[u].next; i!=-1; i=V[i].next){
			int v = V[i].v;
			if( v==p || vis[v] ) continue;
			dfs( v, u, V, K, vis, dis+V[i].w, dn, d );
		}
	}
	struct Node{
		int v, w, key;
		void set( int V, int W, int Key ){ v = V; w = W; key = Key; }
	};
	static bool cmp( Node s, Node t ){ return s.key<t.key; }
	static int dfs( int u, int n, Link *V, int K, bool *vis, int *d, int *tmp, Node *P ){
		int num = 0, root = GetRoot( u, n, V, vis ), pn = 0;
		vis[root] = true;
		for(int i=V[root].next; i!=-1; i=V[i].next){
			int v = V[i].v;
			if( vis[v] ) continue;
			int size = Size( v, root, V, vis );
			P[pn++].set( v, V[i].w, size );
			if( size>1 ) num += dfs( v, size, V, K, vis, d, tmp, P+pn );
		}
		if( pn>1 ) sort( P, P+pn, cmp );
		for(int i=0,len=0; i<pn; i++){
			int dn = 0;
			dfs( P[i].v, root, V, K, vis, P[i].w, dn, d+len );
			if( dn>1 ) sort( d+len, d+len+dn );
			num += dn+Sum( len, d, dn, d+len, K );
			Union( len, d, dn, d+len, tmp );
			len += dn;
		}
		vis[root] = false;
		return num;
	}
	/*static void ans(){ // poj 1741 Tree
		const int N = 10008;
		List s( 40000 );
		int n, k, d[N], tmp[N]; bool vis[N];
		Node P[N];
		while( scanf("%d%d",&n,&k)==2 && n+k ){
			s.init( n );
			for(int i=1,u,v,w; i<n; i++){
				scanf("%d%d%d",&u,&v,&w);
				s.insert( u-1, v-1, w );
				s.insert( v-1, u-1, w );
			}
			memset( vis, 0, sizeof(bool)*n );
			printf("%d\n", dfs( 0, n, s.V, k, vis, d, tmp, P ) );
		}
	}*/


	static int GetSize( int u, int p, Link *V, bool *color, bool *vis, int &black ){
		int size = 1;
		for(int i=V[u].next; i!=-1; i=V[i].next){
			int v = V[i].v, b = 0;
			if( v==p || vis[v] ) continue;
			size += GetSize( v, u, V, color, vis, b );
			if( black<b ) black = b;
		}
		black += color[u];
		return size;
	}
	static void GetD( int u, int p, Link *V, int k, int K, bool *color, bool *vis, int *d, int w ){
		k += color[u];
		if( k>K ) return;
		d[k] = max( d[k], w );
		for(int i=V[u].next; i!=-1; i=V[i].next){
			int v = V[i].v;
			if( v==p || vis[v] ) continue;
			GetD( v, u, V, k, K, color, vis, d, w+V[i].w );
		}
	}
	static int dfs( int u, int p, Link *V, int K, bool *color, bool *vis, int *d, int *tmp, Node *P ){
		int mx = 0, pn = 0;
		for(int i=V[u].next; i!=-1; i=V[i].next){
			int v = V[i].v, black = 0;
			if( v==p || vis[v] ) continue;
			int n = GetSize( v, u, V, color, vis, black );
			int root = GetRoot( v, n, V, vis );
			P[pn++].set( v, V[i].w, black );

			vis[root] = true;
			mx = max( mx, dfs( root, u, V, K, color, vis, d, tmp, P+pn ) );
			vis[root] = false;
		}
		if( pn<=0 || K-color[u]<0 ) return mx;
		
		sort( P, P+pn, cmp );
		for(int i=0; i<=P[pn-1].key+1; i++) d[i] = -INF;
		for(int i=0,len=0; i<pn; i++){
			int v = P[i].v, n = min( P[i].key+color[u], K );
			for(int j=0; j<=n; j++) tmp[j] = -INF;
			tmp[color[u]] = 0;
			GetD( v, u, V, color[u], K, color, vis, tmp, P[i].w );
			for(int j=1; j<=n; j++) tmp[j] = max( tmp[j], tmp[j-1] );
			mx = max( mx, tmp[n] );
			for(int j=0; j<len; j++){
				int t = min( n, K+color[u]-j );
				if( t>=0 && t<=n ) mx = max( mx, d[j]+tmp[t] );
			}
			for(int j=0; j<=n; j++) d[j] = max( d[j], tmp[j] );
			len = min( n, K )+1;
		}
		return mx;
	}
	static void ans(){ // spoj 1825  Free tour II
		const int N = 200008;
		int n, k, m, d[2*N];
		bool color[N], vis[N];
		List s( 3*N ); Node P[N];
		clock_t st = clock();
		while( scanf("%d%d%d",&n,&k,&m)==3 ){
			memset( color, 0, sizeof(bool)*n );
			for(int i=0,j; i<m; i++){
				scanf("%d",&j);
				color[j-1] = true;
			}
			s.init( n );
			for(int i=1,u,v,w; i<n; i++){
				scanf("%d%d%d",&u,&v,&w);
				s.insert( u-1, v-1, w );
				s.insert( v-1, u-1, w );
			}
			memset( vis, 0, sizeof(bool)*n );
			int root = GetRoot( 0, n, s.V, vis );
			vis[root] = true;
			printf("%d\n",dfs( root, -1, s.V, k, color, vis, d, d+n, P ));
		}
	}
};

int main(){
	Tree::ans();
	return 0;
}
