#include<iostream>
#include<map>
#include<cmath>
#include<string>
#include<set>
#include<algorithm>
using namespace std;

/*struct Link{
	int u, v, next; double w;
	void set( int U=-1, int V=-1, int Next=-1, double W=-1){ u=U; v=V; next=Next; w=W; }
};
class List{
public:
	int end; Link *V;
	List( int size = 1024 ){ V = new Link[size]; }
	~List(){ delete []V; }	
	void init( int n ){ end = n; for(int i=0; i<n; i++) V[i].next = -1; }
	void insert( int u, int v, double w = -1 ){
		V[end].set( u, v, V[u].next, w );
		V[u].next = end++;
	}
};

class Graph{
	struct Point{
		int u, v; double w;
		void set( int a, int b, double c){ u=a; v=b; w=c; }
	};
public:
	static bool cmp(Point s, Point t){ return s.w<t.w; }
	static int opt( int v ){ return ( (v%2)?(v-1):(v+1) ); }
	static int Find( int u, int *p ){
		if( p[u] < 0 ) return u;
		else{ p[u] = Find(p[u], p); return p[u]; }
	}
	static void Union( int u, int v, int *p ){
		int pu = Find( u, p ), pv = Find( v, p );
		if( pu == pv ) return;
		int tmp = p[pu]+p[pv];
		if( p[pu] > p[pv] ){ p[pu] = pv; p[pv] = tmp; }
		else{ p[pv] = pu; p[pu] = tmp; }
	}
	static void DFS( int s, Link *V, double *d, int *p ){
		for(int i=V[s].next; i!=-1; i=V[i].next){
			if( d[V[i].v] != -1 ) continue;
			if( V[i].w == -1 || V[opt(i)].w == -1 ) continue;
			p[V[i].v] = i;
			if( d[s] == -2 ) d[V[i].v] = 0;
			else d[V[i].v] = max( d[s], V[i].w );
			DFS( V[i].v, V, d, p );
		}
	}
	static double MST( int m, Point *p, int n, Link *V, int s, int k ){
		int *P = new int[n];
		memset( P, -1, sizeof(int)*n );
		double res = 0;
		List t( 8*n );
		t.init( (n%2 ? (n+1):n) );
		for(int i=0; i<m; i++){
			int u = p[i].u, v = p[i].v;
			if( u == s || v == s ) continue;
			if( Find( u, P ) == Find( v, P ) ) continue;
			res += p[i].w;
			Union( u, v, P );
			t.insert( u, v, p[i].w );
			t.insert( v, u, p[i].w );
		}
		double *d = new double[n];
		int *id = new int[n];
		bool *vis = new bool[n];
		for(int i=0; i<n; i++) d[i] = -1;
		memset( vis, 0, sizeof(bool)*n );
		for(int i=V[s].next; i!=-1; i=V[i].next){
			int u = V[i].v;
			int v = Find( u, P );
			if( d[v]==-1 || d[v]>V[i].w ){
				if( d[v] != -1 ) vis[id[v]] = false;
				d[v] = V[i].w; id[v] = u; vis[u] = true;
			}
		}
		int block = 0;
		for(int i=0; i<n; i++){
			if( d[i]!=-1 ){
				res += d[i]; block++;
				t.insert( s, id[i], d[i] );
				t.insert( id[i], s, d[i] );
			}
		}
		for(int i=block+1; i<=k; i++){
			for(int j=0; j<n; j++) d[j] = -1;
			d[s] = -2;
			memset( P, -1, sizeof(int)*n );
			DFS( s, t.V, d, P );
			double mc = 0; int v;
			for(int j=V[s].next; j!=-1; j=V[j].next){
				if( vis[V[j].v] ) continue;
				if( mc > V[j].w-d[V[j].v] ){ mc = V[j].w-d[V[j].v]; v = V[j].v; }
			}
			if( mc >= 0 ) break;
			res += mc;
			vis[v] = true;
			for(int j=v, pj; ; j=t.V[pj].u){
				pj = P[j];
				if( t.V[pj].w == d[v] ){ t.V[pj].w = -1; break; }
			}
			t.insert( s, v, 0 );
			t.insert( v, s, 0 );
		}
		delete []P;
		delete []d;
		delete []id;
		delete []vis;
		return res;
	}
	static void ans(){ // poj 1639 度限制生成树
		int m, k;
		char s[20], t[20];
		Point p[20008];
		while( scanf("%d",&m)==1 ){
			List ss( 2*m );
			ss.init( m );
			map<string, int> mp;
			map<string, int>::iterator it;
			mp.insert( make_pair( "Park", 0 ) );
			int n = 1;
			for(int i=0; i<m; i++){
				int u, v, w;
				scanf("%s %s %d", s, t, &w );
				string st = string(s);
				if( (it=mp.find(st)) == mp.end() ){
					mp.insert( make_pair(st, n) ); u = n++;
				}
				else u = it->second;
				st = string(t);
				if( (it=mp.find(st)) == mp.end() ){
					mp.insert( make_pair(st, n) ); v = n++;
				}
				else v = it->second;
				p[i].set( u, v, (double)w );
				if( u == 0 ) ss.insert( 0, v, (double)w );
				if( v == 0 ) ss.insert( 0, u, (double)w );
			}
			sort( p, p+m, cmp );
			scanf("%d",&k);
			printf("Total miles driven: %.0f\n", MST( m, p, n, ss.V, 0, k ) );
		}
	}
};*/

class Graph{
public:
	static double prim( int n, double *val, double *len, double &s ){
		bool *vis = new bool[n];
		memset( vis, 0, sizeof(bool)*n );
		double res = 0; s = 0;
		double *d = new double[n];
		memcpy( d, len, sizeof(double)*n );
		for(int i=1; i<n; i++){
			double mn = 1e20; int k;
			for(int j=1; j<n; j++){
				if( !vis[j] && mn > val[j] ){ mn = val[j]; k = j; }
			}
			res += mn;
			s += d[k];
			vis[k] = 1;
			for(int j=1; j<n; j++){
				if( val[j] > val[k*n+j] ){
					val[j] = val[k*n+j];
					d[j] = len[k*n+j];
				}
			}
		}
		delete []vis;
		delete []d;
		return res;
	}
    static double dis( int x1, int y1, int x2, int y2){
		return sqrt((double)(x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
    }
	static void ans(){ // poj 2728 Desert King 最优比率生成树
		const int N = 1008;
		int n, p[N][3], *cost = new int[N*N];
		double *len = new double[N*N], *val = new double[N*N];
		while( scanf("%d",&n)!=EOF && n!=0 ){
			for(int i=0; i<n; i++) scanf("%d%d%d",&p[i][0],&p[i][1],&p[i][2]);
			for(int i=0; i<n; i++){
				for(int j=i; j<n; j++){
					len[i*n+j] = len[j*n+i] = dis( p[i][0], p[i][1], p[j][0], p[j][1]);
					cost[i*n+j] = cost[j*n+i] = abs( p[j][2]-p[i][2] );
				}
			}

			double r = 0, eps = 1e-4;
			while( true ){
				for(int i=0; i<n; i++){
					for(int j=i; j<n; j++){
						val[i*n+j] = val[j*n+i] = cost[i*n+j]-len[i*n+j]*r;
					}
				}
				double s1 = 0, s2 = 0;
				s1 = prim( n, val, len, s2 );
				if( fabs(s1)<eps ) break;
				r += s1/s2;
			}
			printf("%.3lf\n", r);
		}
		delete []cost;
		delete []len;
		delete []val;
	}
};

int main(){
	Graph::ans();
	return 0;
}