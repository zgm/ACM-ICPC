#include<stdlib.h>
#include<stdio.h>
#include<cstring>
#include<string.h>
#include<vector>
#include<iostream>
using namespace std;

struct Link{
	int v, next;
	void set( int V=-1, int Next=-1){ v=V; next=Next; }
};
class List{
public:
	int end; Link *V;
	List( int size = 1024 ){ V = new Link[size]; }
	~List(){ delete []V; }	
	void init( int n ){ end = n; for(int i=0; i<n; i++) V[i].next = -1; }
	void insert( int u, int v ){
		V[end].set( v, V[u].next );
		V[u].next = end++;
	}
};

class Match{
public:
    static bool find( int u, Link *V, bool *vis, int *y ){
        for(int i=V[u].next; i!=-1; i=V[i].next){
            int v = V[i].v;
            if( vis[v] ) continue;
            vis[v] = true;
            if( y[v]==-1 || find( y[v],V,vis,y ) ){ y[v] = u; return true; }
        }
        return false;
    }
    static int MaxMatch( int n, int m, Link *V, int *y ){
        int i, res = 0;
        for(i=0; i<m; i++) y[i] = -1;
        bool *vis = new bool[m];
        for(i=0; i<n; i++){
            memset( vis, 0, sizeof(bool)*m );
            if( find( i, V, vis, y ) ) res ++;
        }
        delete []vis;
        return res;
    }
	static bool bfs( int n, int m, Link *V, int *xy, int *d, int *q ){
		bool ok = false; int head = 0, tail = 0;
		memset( d, 0, sizeof(int)*(n+m) );
		for(int i=0; i<n; i++) if( xy[i]==-1 ) q[tail++] = i;
		for( ; head!=tail; head=(head+1)%(n+m) ){
			for(int u=q[head],i=V[u].next; i!=-1; i=V[i].next){
				int v = V[i].v;
				if( d[n+v] ) continue;
				d[n+v] = d[u]+1;
				if( xy[n+v]==-1 ) ok = true;
				else{
					d[xy[n+v]] = d[n+v]+1;
					q[tail] = xy[n+v];
					tail = (tail+1)%(n+m);
				}
			}
		}
		return ok;
	}
	static bool dfs( int u, int n, Link *V, int *xy, int *d ){
		for(int i=V[u].next; i!=-1; i=V[i].next){
			int v = V[i].v;
			if( d[u]+1==d[n+v] ){
				d[n+v] = 0;
				if( xy[n+v]==-1 || dfs( xy[n+v], n, V, xy, d ) ){
					xy[u] = v; xy[n+v] = u; return true;
				}
			}
		}
		return false;
	}
	static int HopcroftKarp( int n, int m, Link *V ){ // must n,m>0
		int res = 0, *xy = new int[3*(n+m)], *d = xy+n+m, *q = d+n+m;
		memset( xy, -1, sizeof(int)*(n+m) );
		while( bfs(n,m,V,xy,d,q) ){
			for(int i=0; i<n; i++){
				if( xy[i]==-1 && dfs(i,n,V,xy,d) ) res++;
			}
		}
		delete []xy;
		return res;
	}
	/*static void ans(){ // spoj 4206  Fast Maximum Matching
		int n, m, k;
		List s( 300000 );
		while( scanf("%d%d%d",&n,&m,&k)==3 ){
			s.init( n );
			for(int u,v; k--; ){
				scanf("%d%d",&u,&v);
				s.insert( u-1, v-1 );
			}
			printf("%d\n",HopcroftKarp(n,m,s.V));
		}
	}*/
	// hdu 2389  Rain on your Parade
    /*static void ans( ){ // poj 3692 Kindergarten
        int t = 0, g, b, m, u, v, y[201];
        bool vis[201][201];
		List s( 80000 );
        while( scanf("%d%d%d",&g,&b,&m) != EOF && g+b+m != 0 ){
            s.init( g );
            memset( vis, false, sizeof(vis) );
            for(int i=0; i<m; i++){
                scanf("%d%d",&u,&v);
                vis[u-1][v-1] = true;
            }
            for(int i=0; i<g; i++){
                for(int j=0; j<b; j++){
                    if( vis[i][j] ) continue;
                    s.insert( i, j );
                }
            }
            printf("Case %d: %d\n",++t,g+b-MaxMatch( g, b, s.V, y ) );
        }
    }*/
	/*static bool find( int u, Link *V, bool *vis, int *xy, bool *del=NULL ){
        for(int i=V[u].next; i!=-1; i=V[i].next){
            int v = V[i].v;
			if( del!=NULL && del[v] ) continue;
            if( vis[v] ) continue;
            vis[v] = true;
            if( xy[v]==-1 || find( xy[v], V, vis, xy, del ) ){
                xy[v] = u; xy[u] = v; return true;
            }
        }
        return false;
    }
    static int MaxMatch( int n, Link *V, int *id, int *xy ){
        int i, j, res = 0;
		memset( xy, -1, sizeof(int)*n );
        bool *vis = new bool[n];
        for(i=0; i<n; i++){
			if( id[i] == 1 ) continue;
            memset( vis, 0, sizeof(bool)*n );
            if( find( i, V, vis, xy ) ) res ++;
        }
        delete []vis;
        return res;
    }
	static void ans(){ // poj 3715 Blue and Red
		int t, n, m, id[208], xy[208];
		bool vis[208], del[208];
		scanf("%d",&t);
		List s( 50000 );
		while( t-- ){
			scanf("%d%d",&n,&m);
			s.init( n );
			for(int i=0; i<n; i++) scanf("%d",&id[i] );
			int u, v;
			for(int i=0; i<m; i++){
				scanf("%d%d",&u,&v);
				if( id[u] == id[v] ) continue;
				s.insert( u, v );
				s.insert( v, u );
			}
			int k = MaxMatch( n, s.V, id, xy );
			printf("%d", k );
			memset( del, 0, sizeof(bool)*n );
			for(int i=0; i<n; i++){
				if( xy[i] == -1 ) continue;
				memset( vis, 0, sizeof(bool)*n );
				del[i] = true;
				if( !find( xy[i], s.V, vis, xy, del ) ){
					printf(" %d",i);
					k--;
					xy[xy[i]] = -1;
				}
				else del[i] = false;
				xy[i] = -1;
			}
			printf("\n");
		}
	}*/
	static int root( int u, int v, int n, int *x, int *pre, int *id, bool *in ){
		memset( in, 0, sizeof(bool)*n );
		while( true ){
			in[u] = true;
			if( x[u]==-1 ) break;
			u = id[pre[x[u]]];
		}
		while( !in[v] ) v = id[pre[x[v]]];
		return v;
	}
	static void change( int b, int u, int *x, int *pre, int *id, bool *in ){
		for(int v; id[u]!=b; ){
			v = x[u];
			in[id[v]] = in[id[u]] = true;
			u = pre[v];
			if( id[u]!=b ) pre[u] = v;
		}
	}
	static int contract( int u, int v, int n, int *x, int *pre, int *id, bool *in ){
		int b = root( id[u], id[v], n, x, pre, id, in );
		memset( in, 0, sizeof(bool)*n );
		change( b, u, x, pre, id, in );
		change( b, v, x, pre, id, in );
		if( id[u]!=b ) pre[u] = v;
		if( id[v]!=b ) pre[v] = u;
		return b;
	}
	static void argument( int u, int *x, int *pre ){
		for(int v,k; u!=-1; u=k){
			v = pre[u]; k = x[v];
			x[u] = v; x[v] = u;
		}
	}
	static int bfs( int s, int n, Link *V, int *x, int *q, int *pre, int *id, bool *hash, bool *in ){
		memset( pre, -1, sizeof(int)*n );
		memset( hash, 0, sizeof(bool)*n );
		for(int i=0; i<n; i++) id[i] = i;
		q[0] = s; hash[s] = 1;
		for(int head=0,tail=1; head<tail; ){
			int u = q[head++];
			for(int i=V[u].next; i!=-1; i=V[i].next){
				int v = V[i].v;
				if( id[u]!=id[v] && v!=x[u] ){
					if( v==s || (x[v]!=-1 && pre[x[v]]!=-1) ){
						int b = contract( u, v, n, x, pre, id, in );
						for(int j=0; j<n; j++){
							if( in[id[j]] ){
								id[j] = b;
								if( hash[j]==0 ){ hash[j] = 1; q[tail++] = j; }
							}
						}
					}
					else if( pre[v]==-1 ){
						pre[v] = u;
						if( x[v]==-1 ){ argument( v, x, pre ); return 1; }
						else{ q[tail++] = x[v]; hash[x[v]] = 1; }
					}
				}
			}
		}
		return 0;
	}
	static int MaxMatch( int n, Link *V, int *x ){ // 一般图匹配（带花树算法）
		memset( x, -1, sizeof(int)*n );
		int num = 0, *q = new int[3*n];
		bool *hash = new bool[2*n];
		for(int i=0; i<n; i++){
			if( x[i]==-1 ) num += bfs( i, n, V, x, q, q+n, q+2*n, hash, hash+n );
		}
		delete []q; delete []hash;
		return num;
	}
	/*static void ans(){ // ural 1099   Work scheduling
		List s( 1000000 );
		int n, u, v, x[408];
		scanf("%d",&n);
		s.init( n );
		while( scanf("%d%d",&u,&v)==2 ){
			s.insert( u-1, v-1 );
			s.insert( v-1, u-1 );
		}
		int num = MaxMatch( n, s.V, x );
		printf("%d\n",2*num);
		for(int i=0; i<n; i++){
			if( x[i]!=-1 && i<x[i] ) printf("%d %d\n",i+1,x[i]+1);
		}
	}*/
	static bool ok( int x, int y, int w, int h ){ return 0<=x && x<w && 0<=y && y<h; }
	static void ans(){ // poj 3020  Antenna Placement
		int T, w, h, id[40][12], x[408]; char t[40][12];
		List s( 1000000 );
		scanf("%d",&T);
		while( T-- ){
			scanf("%d%d",&w,&h);
			int num = 0;
			for(int i=0; i<w; i++){
				scanf("%s",t[i]);
				for(int j=0; j<h; j++) if( t[i][j]=='*' ) id[i][j] = num++;
			}
			s.init( num );
			for(int i=0; i<w; i++){
				for(int j=0; j<h; j++){
					if( t[i][j]!='*' ) continue;
					if( ok(i+1,j,w,h) && t[i+1][j]=='*' ){
						s.insert( id[i][j], id[i+1][j] );
						s.insert( id[i+1][j], id[i][j] );
					}
					if( ok(i,j+1,w,h) && t[i][j+1]=='*' ){
						s.insert( id[i][j], id[i][j+1] );
						s.insert( id[i][j+1], id[i][j] );
					}
				}
			}
			printf("%d\n",num-MaxMatch(num,s.V,x));
		}
	}
};

class CompanyRestructuring{
public:
	static int fewestDivisions(vector<string> hm){ // srm 435 DIV 1 Level 3  CompanyRestructuring
		int n = hm.size();
		const int N = 58;
		bool mag[N][N], sup[N][N];
		for(int i=0; i<n; i++){
			for(int j=0; j<n; j++){
				mag[i][j] = sup[i][j] = (hm[i][j]=='Y');
			}
		}
		for(int k=0; k<n; k++){
			for(int i=0; i<n; i++){
				for(int j=0; j<n; j++){
					sup[i][j] |= sup[i][k]&sup[k][j];
				}
			}
		}
		bool vis[N];
		memset( vis, 0, sizeof(bool)*n );
		List s( 4*N*N );
		int res = n, y[N];
		for(int i=0; i<n; i++){
			if( vis[i] ) continue;
			s.init( n );
			for(int j=0; j<n; j++){
				if( j==i || (sup[i][j]&&sup[j][i]) ){
					vis[j] = true;
					for(int k=0; k<n; k++){
						if( mag[k][j] && !sup[j][k] ) s.insert( k, j );
					}
				}
			}
			res -= Match::MaxMatch( n, n, s.V, y );
		}
		return res;
	}
};

int main(){
    Match::ans();
    return 0;
}