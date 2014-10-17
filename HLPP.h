#include<iostream>
#include<time.h>
using namespace std;

struct Link{
	int u, v, c, next;
	void set( int U, int V, int C, int Next){ u=U; v=V; c=C; next=Next; }
};
class List{
public:
	int end; Link *V;
	List( int size = 1024 ){ V = new Link[size]; }
	~List(){ delete []V; }
	void init( int n ){ end = n = (n&1 ? n+1:n); for(int i=0; i<n; i++) V[i].next = -1; }
	void insert( int u, int v, int c1, int c2=0 ){
		V[end].set( u, v, c1, V[u].next );
		V[u].next = end++;
		V[end].set( v, u, c2, V[v].next );
		V[v].next = end++;
	}
};

class Graph{
public:
	static void Initialize( int s, int t, int n, Link *V, int *e, int *h, int *hn, int &level, Link *L){
		memset( e, 0, sizeof(int)*n );
		for(int i=V[s].next; i!=-1; i=V[i].next){
			if( i%2 ) continue;
			V[i+1].c = V[i].c;
			e[V[i].v] += V[i].c;
			e[s] -= V[i].c;
			V[i].c = 0;
		}

		for(int i=0; i<n; i++) h[i] = n;
		int head = 0, tail = 0, *q = new int[4*n];
		for(q[tail++]=t, h[t]=0; head<tail; ){
			int u = q[head++], v;
			for(int i=V[u].next; i!=-1; i=V[i].next){
				v = V[i].v;
				if( v!=s && h[v]>h[u]+1 && i%2 && V[i-1].c>0 ){
					h[v] = h[u]+1; q[tail++] = v;
				}
			}
		}
		delete []q;
		memset( hn, 0, sizeof(int)*2*n );
		for(int i=0; i<n; i++) hn[h[i]]++;
		for(int i=0; i<2*n; i++) L[i].next = -1;
		for(int i=0; i<n; i++){
			if( i==s || i==t || e[i]==0 ) continue;
			level = max( level, h[i] );
			L[2*n+i].v = i;
			L[2*n+i].next = L[h[i]].next;
			L[h[i]].next = 2*n+i;
		}
	}
	static void Push( int u, int r, int t, int n, Link *V, int *e, int *h, int &level, Link *L){
		int v = V[r].v;
		if( v != t && e[v] == 0 ){
			L[2*n+v].v = v;
			L[2*n+v].next = L[h[v]].next;
			L[h[v]].next = 2*n+v;
			level = max( level, h[v] );
		}
		int es = min( e[u], V[r].c );
		V[r].c -= es;
		V[r^1].c += es;
		e[u] -= es;
		e[v] += es;
	}
	static void Relabel( int u, int s, int n, Link *V, int *h, int *hn ){
		int mn = -1, v, oldh = h[u];
		for(int i=V[u].next; i!=-1; i=V[i].next){
			v = V[i].v;
			if( V[i].c>0 && (mn==-1 || mn>h[v]) ) mn = h[v];
		}
		hn[h[u]]--; h[u]=mn+1; hn[mn+1]++;
		// GapHeuristic
		if( hn[oldh] != 0 || oldh >= n ) return;
		for(int i=0; i<n; i++){
			if( h[i] > oldh && h[i] <= n && i != s ){
				hn[h[i]]--; h[i] = n+1; hn[n+1]++;
			}
		}
	}
	static void Discharge( int u, int s, int t, int n, Link *V, int *e, int *h, int *hn, int &level, Link *L){
		int r = V[u].next, v;
		while( e[u] > 0 ){
			v = V[r].v;
			if( r == -1 ){ Relabel( u, s, n, V, h, hn ); r = V[u].next; }
			else if( V[r].c > 0 && h[u] == h[v]+1 ) Push( u, r, t, n, V, e, h, level, L );
			else r = V[r].next;
		}
	}
	static int maxflow( int s, int t, int n, Link *V){
		int flow = 0;
		int *e = new int[n], *h = new int[n], *hn = new int[2*n], level=-1;
		Link *L = new Link[3*n];
		Initialize( s, t, n, V, e, h, hn, level, L );
		while( level >= 0 ){
			int j = L[level].next;
			int u = L[j].v;
			L[level].next = L[j].next;
			while( level >= 0 && L[level].next == -1 ) level--;
			if( e[u] > 0 ) Discharge( u, s, t, n, V, e, h, hn, level, L );
		}
		for(int i=V[s].next; i!=-1; i=V[i].next) flow += V[i^1].c;
		delete []e;
		delete []h;
		delete []hn;
		delete []L;
		return flow;
	}
	static void ans(){ // poj 3469 Dual Core CPU
		int n, m;
		List s( 600000 );
		while( scanf("%d%d",&n,&m)==2 ){
			s.init( n+2 );
			int a, b, w;
			for(int i=0; i<n; i++){
				scanf("%d%d",&a,&b);
				s.insert( 0, i+1, a );
				s.insert( i+1, n+1, b );
			}
			for(int i=0; i<m; i++){
				scanf("%d%d%d",&a,&b,&w);
				s.insert( a, b, w, w );
			}
			printf("%d\n", maxflow( 0, n+1, n+2, s.V ) );
		}
	}
	/*static void ans(){ // poj 1459 Power Network
		int n, np, nc, m; char ch[100];
		while( scanf("%d%d%d%d",&n,&np,&nc,&m)==4 ){
			n += 2;
			List s( n+2*m+2*np+2*nc+4 );
			s.init( n );
			int u, v, w, j;
			for(int i=0; i<m; i++){
				//scanf("\n(%d,%d)%d",&u,&v,&w);
				scanf("%s",ch);
				for(u=0, j=1; ch[j]!=','; j++) u = u*10+ch[j]-'0';
				for(v=0, ++j; ch[j]!=')'; j++) v = v*10+ch[j]-'0';
				for(w=0, ++j; ch[j]!='\0'; j++) w = w*10+ch[j]-'0';
				s.insert( u+1, v+1, w, 0 );
			}
			for(int i=0; i<np; i++){
				//scanf("\n(%d)%d",&u,&w);
				scanf("%s",ch);
				for(u=0, j=1; ch[j]!=')'; j++) u = u*10+ch[j]-'0';
				for(w=0, ++j; ch[j]!='\0'; j++) w = w*10+ch[j]-'0';
				s.insert( 0, u+1, w, 0 );
			}
			for(int i=0; i<nc; i++){
				//scanf("\n(%d)%d",&u,&w);
				scanf("%s",ch);
				for(u=0, j=1; ch[j]!=')'; j++) u = u*10+ch[j]-'0';
				for(w=0, ++j; ch[j]!='\0'; j++) w = w*10+ch[j]-'0';
				s.insert( u+1, n-1, w, 0 );
			}
			printf("%d\n",maxflow( 0, n-1, n, s.V ) );
		}
	}*/
	/*static int bfs( int u, int s, int t, int n, Link *V ){
		bool *vis = new bool[n];
		for(int i=0; i<n; i++) vis[i] = false;
		vis[u] = vis[t] = true;
		int head = 0, tail = 0, res = 0, *q = new int[4*n];
		for(int i=V[u].next; i!=-1; i=V[i].next){
			if( i%2 == 1 || V[i].c == 0 ) continue;
			res++; vis[V[i].v] = true;
			q[tail++] = V[i].v;
		}
		while( head < tail ){
			int k = q[head++];
			for(int i=V[k].next; i!=-1; i=V[i].next){
				int r = V[i].v;
				if( vis[r] || (i%2==1&&V[i].c<=0) ) continue;
				res++; vis[r] = true;
				q[tail++] = r;
			}
		}
		delete []vis;
		return res;
	}
	static void ans(){ // poj 2987 Firing
		int n, m, v, u;
		while( scanf("%d%d",&n,&m)==2 ){
			n += 2;
			List s( n+2*n+2*m+4 );
			s.init( n );
			__int64 res = 0;
			for(int i=1; i<n-1; i++){
				scanf("%d",&v );
				if( v > 0 ){
					res += v;
					s.insert( 0, i, v, 0 );
				}
				else if( v < 0 ) s.insert( i, n-1, -v, 0 );
			}
			const int INF = (1<<28);
			for(int i=0; i<m; i++){
				scanf("%d%d",&v,&u);
				s.insert( v, u, INF, 0 );
			}
			__int64 flow = maxflow( 0, n-1, n, s.V );
			printf("%d %I64d\n",bfs( 0, 0, n-1, n, s.V), res-flow );
		}
	}*/
};

int main(){
	Graph::ans();
	return 0;
}