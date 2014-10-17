#include <iostream>
#include <cmath>
#include <math.h>
using namespace std;

class RMQ{
public:
	static void RMQ_Process( int *a, int n, int *rmq ){
		for(int i=0; i<n; i++) rmq[i] = i;
		for(int i=1; (1<<i)<=n; i++){
			for(int j=0; j+(1<<i)-1<n; j++){
				int *tmp = rmq + n*(i-1);
				int k = j+(1<<(i-1));
				if( a[tmp[j]] < a[tmp[k]] ) rmq[n*i+j] = tmp[j];
				else rmq[n*i+j] = tmp[k];
			}
		}
	}
	static int Get_RMQ( int *a, int n, int *rmq, int l, int r ){
		int k = (int) log( (double)(r-l+1) );
		int *tmp = rmq+n*k;
		return min( a[tmp[l]], a[tmp[r+1-(1<<k)]] );
	}
	static void ans( ){
		int *a, n, m, *rmq;
		while( scanf("%d%d",&n,&m)!=EOF ){
			a = new int[n];
			int lg = 1;
			for(lg=1; (1<<lg)<=n; lg++);
			rmq = new int[ n * lg ];
			for(int i=0; i<n; i++) scanf("%d",&a[i]);
			RMQ_Process( a, n, rmq );
			
			int l, r;
			for(int i=0; i<m; i++){
				scanf("%d%d",&l,&r);
				printf("%d\n",Get_RMQ( a, n, rmq, l, r ));
			}
			delete []a;
			delete []rmq;
		}
	}
};

struct Link{
	int v, w, next;
	void set( int V, int Next, int W){ v=V; next=Next; w=W; }
};
class List{
public:
	int end; Link *V;
	List( int size = 1024 ){ V = new Link[size]; }
	~List(){ delete []V; }	
	void init( int n ){ end = n; for(int i=0; i<n; i++) V[i].next = -1; }
	void insert( int u, int v, int w ){
		V[end].set( v, V[u].next, w );
		V[u].next = end++;
	}
};

const int N = 100008;
struct Point{ int l, r; }Pt[N];
struct Node{ int l, r, s, t, val; }P[5*N];

class Tree{
public:
	static void Init( int l, int r, Node *p, int &end ){
		P[end].l = l; P[end].r = r; P[end].val = 0;
		if( r > l ){
			P[end].s = (end+1);
			int old_end = end++;
			Init( l, (l+r)>>1, p, end );
			P[old_end].t = end;
			Init( ((l+r)>>1)+1, r, p, end );
		}
		else{ P[end].s = P[end].t = -1; end++; }
	}
	static void Add( int l, int r, int add, Node *p, int ps ){
		if( l <= p[ps].l && r >= p[ps].r ) p[ps].val += add;
		else{
			int mid = (p[ps].l+p[ps].r)/2;
			if( l <= mid ) Add( l, r, add, p, p[ps].s );
			if( r > mid ) Add( l, r, add, p, p[ps].t );
		}
	}
	static int Get( int l, Node *p, int ps ){
		if( p[ps].l == p[ps].r ) return p[ps].val;
		else{
			int res = 0;
			if( l >= p[ps].l && l <= p[ps].r ) res += p[ps].val; 
			int mid = (p[ps].l+p[ps].r)/2;			
			if( l <= mid ) res += Get( l, p, p[ps].s );
			else res += Get( l, p, p[ps].t );
			return res;
		}
	}
};

class LCA{
public:
	// p[i]: the parent of every node i
	static void Get_P( int n, int *p, Link *V, int st = 0 ){
		int head =0, tail = 0, *q = new int[n];
		for(int i=0; i<n; i++) p[i] = -2;
		for(p[st]=-1, q[tail++]=st; head<tail; ){
			int u = q[head++];
			for(int j=V[u].next; j!=-1; j=V[j].next ){
				int v = V[j].v;
				if( p[v] == -2 ){ p[v] = u; q[tail++] = v; }
			}
		}
		delete []q;
	}
	static void LCA_Process( int n, int *p, int *lca, int level = -1 ){
		int i, j, *tmp;
		if( level == -1 ) level = n;
		for(i=0; (1<<i) < level; i++){
			tmp = lca + n*i;
			for(j=0; j<n; j++) tmp[j] = -1;
		}
		memcpy( lca, p, sizeof(int)*n );
		for(i=1; (1<<i) < level; i++){
			tmp = lca + n*(i-1);
			for(j=0; j<n; j++) if( tmp[j] != -1 ) lca[n*i+j] = tmp[tmp[j]];
		}
	}
	static void Get_L( int n, int *p, int *L ){
		int tail, *q = new int[n];
		memset( L, -1, sizeof(int)*n );
		for(int i=0; i<n; i++){
			if( L[i] != -1 ) continue;
			for( tail=0, q[0]=i; tail>=0; ){
				int u = q[tail--];
				if( p[u] == -1 ) L[u] = 0;
				else if( L[p[u]] != -1 ) L[u] = L[p[u]]+1;
				else{ q[++tail] = u; q[++tail] = p[u]; }
			}
		}
		delete []q;
	}
	// L[i]: the level of node i in the tree
	static int Get_LCA( int n, int *lca, int *p, int *L, int u, int v ){
		int tmp, log, i;
		if( L[u] < L[v] ){ tmp = u; u = v; v = tmp; }
		// compute the value of [log(L[p])]
		for(log=1; (1<<log)<=L[u]; log++); log--;
		for(i=log; i>=0; i--) if( L[u]-(1<<i) >= L[v] ) u = lca[n*i+u];
		if( u == v ) return u;

		for(i=log; i>=0; i--){
			if( lca[n*i+u] != -1 && lca[n*i+u] != lca[n*i+v] ){
				u = lca[n*i+u]; v = lca[n*i+v];
			}
		}
		return p[u];
	}

	static void EulerSequence( int root, int n, Link *V, int *L, Point *P ){
		int i, k = 0, tail = 0;
		int *q = new int[n], *next = new int[n];
		bool *vis = new bool[n];
		memset( vis, 0, sizeof(bool)*n );
		for(i=0; i<n; i++) next[i] = V[i].next;

		for( q[0]=root; tail>=0; ){
			int u = q[tail];
			if( !vis[u] ){ P[u].l = ++k; vis[u] = 1; }
			for( i=next[u]; i!=-1; i=V[i].next){
				int v = V[i].v;
				if( L[v] == L[u]+1 ){ q[++tail] = v; break; }
			}
			next[u] = ( (i==-1) ? -1:V[i].next );
			if( i == -1 ){ tail--; P[u].r = ++k; }
		}
		delete []q;
		delete []next;
		delete []vis;
	}
	static void Spfa( int s, int n, Link *V, int *d ){
		bool *vis = new bool[n];
		const int INF = 1<<30;
		for(int i=0; i<n; i++){ d[i] = INF; vis[i] = 0; }

		d[s] = 0; vis[s] = 1;
		int head = 0, tail = 0, *q = new int[3*n];
		for( q[tail++]=s; head<tail; ){
			int v = q[head++];
			for(int i=V[v].next; i!=-1; i=V[i].next){
				int u = V[i].v;
				if( d[v]+V[i].w < d[u] ){
					d[u] = d[v]+V[i].w;
					if( !vis[u] ){ q[tail++] = u; vis[u] = true; }
				}
			}
			vis[v] = false;
		}
		delete []vis;
		delete []q;
	}
	static void ans(){ // poj 2763 Housewife Wind
		int n, q, st, in[N][3], t[N], L[N], d[N], lca[18*N];
		while( 3==scanf("%d%d%d",&n,&q,&st)){
			List s( n+2*n+4 );
			s.init( n );
			for(int i=0; i<n-1; i++){
				scanf("%d%d%d",&in[i][0],&in[i][1],&in[i][2]);
				in[i][0]--; in[i][1]--;
				s.insert( in[i][0], in[i][1], in[i][2] );
				s.insert( in[i][1], in[i][0], in[i][2] );
			}
			st--;
			Get_P( n, t, s.V, 0 );
			LCA_Process( n, t, lca );
			Get_L( n, t, L );

			Spfa( 0, n, s.V, d );
			EulerSequence( 0, n, s.V, L, Pt );
			int end = 0;
			Tree::Init( 1, Pt[0].r, P, end );
			while( q-- ){
				int u, v, w;
				scanf("%d",&u);
				if( u == 0 ){
					scanf("%d",&v); v--;
					int res = d[st]+Tree::Get( Pt[st].l, P, 0 );
					res += d[v]+Tree::Get( Pt[v].l, P, 0 );

					int p = Get_LCA( n, lca, t, L, st, v );
					res -= 2*(d[p]+Tree::Get( Pt[p].l, P, 0 ));
					printf("%d\n",res);
					st = v;
				}
				else{
					scanf("%d%d",&v,&w); v--;
					int t1 = in[v][0], t2 = in[v][1], add;
					int l, r;
					if( t[t2] == t1 ){ l = Pt[in[v][1]].l;  r = Pt[in[v][1]].r; }
					else{ l = Pt[in[v][0]].l;  r = Pt[in[v][0]].r; }
					Tree::Add( l, r, w-in[v][2], P, 0 );
					in[v][2] = w;
				}
			}
		}
	}
	/*static void ans( ){ // poj 1470 Closest Common Ancestors
		const int N = 908;
		int n, t[N], lca[10*N], L[N];
		int m, i, j, k, u, v, num[N];
		char c;
		while( scanf("%d",&n)!=EOF ){
			for(i=0; i<n; i++) t[i]=-1;
			for(i=0; i<n; i++){
				for( scanf("%d",&u), k=-1; true; ){
					c=getchar();
					if( c>='0' && c<='9' ){
						if( k == -1 ) k = 0;
						k = 10*k+c-'0';
					}
					else if( k != -1 ) break;
				} //scanf("%d:(%d)",&u,&k);
				for(j=0; j<k; j++){ scanf("%d",&v);	t[v-1]=u-1; }
			}
			LCA_Process( n, t, lca );
			Get_L( n, t, L );

			scanf("%d",&m);
			memset( num, 0, sizeof(int)*n );
			for(i=0; i<m; i++){
				for( u = v = 0; true; ){
					c = getchar();
					if(c>='0' && c<='9') u = 10*u+c-'0';
					else if( u != 0 ) break;
				}
				while( true ){
					c = getchar();
					if(c>='0' && c<='9') v = 10*v+c-'0';
					else if( v != 0 ) break;
				} //scanf("\n(%d%d)",&u,&v);
				k = Get_LCA( n, lca, t, L, u-1, v-1 );
				num[k]++;
			}
			for(i=0; i<n; i++)
			if(num[i]) printf("%d:%d\n",i+1,num[i]);
		}
	}*/
};

int main(){
	//RMQ::ans( );
	LCA::ans( );
	return 0;
}