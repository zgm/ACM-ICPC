#include<iostream>
#include<cmath>
#include<math.h>
using namespace std;

struct Link{
	int v, next;
	void set( int V=-1, int Next=-1 ){ v=V; next=Next; }
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
		V[end].set( u, V[v].next );
		V[v].next = end++;
	}
};

class Arborescence{
public:
	static bool FindCircle( int n, bool *vis, int *pre, int *st, int *id, int *mk, int &top, int &sta ){
		memset( vis, 0, sizeof(bool)*n );
		for(int i=1; i<n; i++){
			if( id[i]!=i || vis[i] ) continue;
			memset( mk, 0, sizeof(int)*n );
			top = 1;
			int tmp = i;
			while( tmp!=0 && !mk[tmp] && !vis[tmp] ){
				vis[tmp] = true;
				mk[tmp] = top;
				st[top++] = tmp;
				tmp = pre[tmp];
			}
			if( mk[tmp] ){ sta = mk[tmp]; return true; }
		}
		return false;
	}
	static void Combine( double &res, int n, List &s, double *d, double *cost, int *pre, int *st, int *id, int top, int sta ){
		int now = st[sta];
		//int *vis = new int[n];
		//memset( vis, 0, sizeof(int)*n );
		//for(int i=s.V[now].next; i!=-1; i=s.V[i].next) vis[s.V[i].v] = 1;
		for(int i=sta; i<top; i++){
			int x = st[i];
			res += cost[x];
			id[x] = now;
			for(int j=0; j<n; j++){
				d[j*n+now] = min( d[j*n+now], d[j*n+x]-cost[x] );
				d[now*n+j] = min( d[now*n+j], d[x*n+j] );
			}
			/*for(int j=s.V[x].next; j!=-1; j=s.V[j].next){
				int v = s.V[j].v;
				if( vis[v] == 0 ){ vis[v] = 2; s.insert( now, v ); }
				d[v*n+now] = min( d[v*n+now], d[v*n+x]-cost[x] );
				d[now*n+v] = min( d[now*n+v], d[x*n+v] );
			}*/
		}
		//delete []vis;
		for(int i=1; i<n; i++) if( id[i]==i ) pre[i] = id[pre[i]];
		cost[now] = 1e20;
		for(int i=0; i<n; i++){
			if( i!=now && id[i]==i && d[i*n+now]<cost[now] ){
				cost[now] = d[i*n+now]; pre[now] = i;
			}
		}
	}
	static double MinArborescence( int n, List &s, double *d, bool *vis, int root=0 ){
		double res = 0, *cost = new double[n];
		for(int i=0; i<n; i++) cost[i] = 1e20;
		int *pre = new int[4*n];
		for(int i=1; i<n; i++){
			for(int j=s.V[i].next; j!=-1; j=s.V[j].next){
				int v = s.V[j].v;
				if( (j%2) && i!=v && cost[i]>d[v*n+i] ){ cost[i] = d[v*n+i]; pre[i] = v; }
			}
		}
		int *id = pre+n, *st = pre+2*n, top, sta, *mk = pre+3*n;
		for(int i=0; i<n; i++) id[i] = i;
		while( FindCircle( n, vis, pre, st, id, mk, top, sta ) )
			Combine( res, n, s, d, cost, pre, st, id, top, sta );
		for(int i=1; i<n; i++) if( id[i]==i ) res += cost[i];
		delete []cost;
		delete []pre;
		return res;
	}
	static void DFS( int v, int &cnt, int n, Link *V, bool *vis ){
		cnt++;
		vis[v] = true;
		for(int i=V[v].next; i!=-1; i=V[i].next)
			if( (i%2)==0 && !vis[V[i].v] ) DFS( V[i].v, cnt, n, V, vis ); 
	}
	static void ans(){ // UVa 11183 Teen Girl Squad
		const int N = 1001, M = 40001;
		int t, n, m;
		double *d = new double[N*N];
		bool vis[N];
		List s( 3*M );
		scanf("%d",&t);
		for(int k=1; k<=t; k++){
			scanf("%d%d",&n,&m);
			s.init( (n%2) ? (n+1):n );
			for(int i=n*n-1; i>=0; --i) d[i] = 1e30;
			for(int i=m-1, u, v; i>=0; --i){
				scanf("%d%d",&u,&v);
				scanf("%lf",&d[u*n+v]);
				s.insert( u, v );
			}
			int cnt = 0;
			memset( vis, 0, sizeof(bool)*n );
			DFS( 0, cnt, n, s.V, vis );
			printf("Case #%d: ", k);
			if( cnt < n ) printf("Possums!\n");
			else printf("%.lf\n",MinArborescence( n, s, d, vis ) );
		}
		delete []d;
	}
	/*static void ans(){ // toj 2248 Channel Design
		const int N = 101, M = 10001;
		int n, m;
		double d[N*N];
		bool vis[N];
		List s( 40000 );
		while( scanf("%d%d",&n,&m)==2 && n+m ){
			s.init( (n%2) ? (n+1):n );
			for(int i=0; i<n*n; i++) d[i] = 1e30;
			for(int i=0, u, v; i<m; i++){
				scanf("%d%d",&u,&v);
				u--; v--;
				scanf("%lf",&d[u*n+v]);
				s.insert( u, v );
			}
			int cnt = 0;
			memset( vis, 0, sizeof(bool)*n );
			DFS( 0, cnt, n, s.V, vis );
			if( cnt < n ) printf("impossible\n");
			else printf("%.lf\n",MinArborescence( n, s, d, vis ) );
		}
	}*/
	/*static void ans(){ // poj 3164 Command Network
		const int N = 101, M = 10001;
		int n, m, x[N], y[N];
		double d[N*N];
		bool vis[N];
		List s( 40000 );
		while( scanf("%d%d",&n,&m)==2 ){
			s.init( ( (n%2) ? (n+1):n ) );
			for(int i=0; i<n*n; i++) d[i] = 1e30;
			for(int i=0; i<n; i++) scanf("%d%d",&x[i],&y[i]);
			for(int i=0, u, v; i<m; i++){
				scanf("%d%d",&u,&v);
				u--; v--;
				if( u<0 || v<0 || u>=n || v>=n ) continue;
				d[u*n+v] = sqrt( (double)(x[u]-x[v])*(x[u]-x[v])+(y[u]-y[v])*(y[u]-y[v]) );
				s.insert( u, v );
			}
			int cnt = 0;
			memset( vis, 0, sizeof(bool)*n );
			DFS( 0, cnt, n, s.V, vis );
			if( cnt < n ) printf("poor snoopy\n");
			else printf("%.2lf\n", MinArborescence( n, s, d, vis ) );
		}
	}*/
};

int main(){
	Arborescence::ans();
	return 0;
}