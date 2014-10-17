#include<iostream>
#include<stdlib.h>
#include<stdio.h>
using namespace std;

const int INF = (1<<28);

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
		V[end].set( u, V[v].next, w );
		V[v].next = end++;
	}
};

class Tree{
	struct Node{
		int l, r, v;
		void set( int L, int R, int V ){ l = L; r = R; v = V; }
	};
public:
	static void Init( int ps, int l, int r, int *v, Node *p ){
		p[ps].set( l, r, v[l] );
		if( l==r ) return;
		Init( 2*ps, l, (l+r)/2, v, p );
		Init( 2*ps+1, (l+r)/2+1, r, v, p );
		p[ps].v = max( p[2*ps].v, p[2*ps+1].v );
	}
	static void Change( int ps, int lr, int v, Node *p ){
		if( p[ps].l==p[ps].r ){ p[ps].v = v; return; }
		int mid = (p[ps].l+p[ps].r)/2;
		Change( (lr<=mid ? (2*ps):(2*ps+1)), lr, v, p );
		p[ps].v = max( p[2*ps].v, p[2*ps+1].v );
	}
	static int Max( int ps, int l, int r, Node *p ){
		if( l<=p[ps].l && p[ps].r<=r ) return p[ps].v;
		int v = -INF, mid = (p[ps].l+p[ps].r)/2;
		if( l<=mid) v = max( v, Max( 2*ps, l, r, p ) );
		if( r>mid ) v = max( v, Max( 2*ps+1, l, r, p ) );
		return v;
	}
	static void Get( int u, int *p, int *L, int *next, int *w, Link *V, int *size ){
		size[u] = 1; int id = -1;
		for(int i=V[u].next; i!=-1; i=V[i].next){
			int v = V[i].v;
			if( p[u] == v ) continue;
			p[v] = u;
			L[v] = L[u]+1;
			w[v] = V[i].w;
			Get( v, p, L, next, w, V, size );
			if( id==-1 || size[id]<size[v] ) id = v;
			size[u] += size[v];
		}
		next[u] = id;
	}
	static void Build( int u, Link *V, int *p, int *next, int *w, int *id, int root, int *r, int &pn, Node *P, int qn, int *q ){
		int v = next[u];
		if( v == -1 ){
			if( qn>0 ){ Init( 1, 0, qn-1, q, P+pn ); pn += 4*qn; }
			return;
		}
		id[v] = qn; r[v] = root; q[qn++] = w[v];
		Build( next[u], V, p, next, w, id, root, r, pn, P, qn, q );
		for(int i=V[u].next; i!=-1; i=V[i].next){
			v = V[i].v;
			if( p[u]==v || v==next[u] ) continue;
			qn = 0; id[v] = pn; r[v] = v;
			Build( v, V, p, next, w, id, v, r, pn, P, qn, q );
		}
	}
	static int Query( int u, int v, int *w, int *p, int *L, int *r, int *id, Node *P ){
		int res = -INF;
		while( r[u]!=r[v] ){
			if( L[r[u]]>=L[r[v]] ){
				if( r[u]==u ){ res = max( res, w[u] ); u = p[u]; continue; }
				res = max( res, Max( 1, 0, id[u], P+id[r[u]] ) );
				u = r[u];
			}
			else{
				if( r[v]==v ){ res = max( res, w[v] ); v = p[v]; continue; }
				res = max( res, Max( 1, 0, id[v], P+id[r[v]] ) );
				v = r[v];
			}
		}
		if( u == v ) return res;
		if( L[u]>L[v] ) swap( u, v );
		if( r[u]==u ) res = max( res, Max( 1, 0, id[v], P+id[u] ) );
		else res = max( res, Max( 1, id[u]+1, id[v], P+id[r[u]] ) );
		return res;
	}
	static void ans(){ // spoj 375  Query on a tree
		const int N = 10008;
		char ch[20];
		List s( 3*N );
		int t, n, u[N], v[N], w[N], q[N];
		int p[N], L[N], next[N], id[N], r[N];
		Node P[4*N];
		
		scanf("%d",&t);
		while( t-- ){
			scanf("%d",&n);
			s.init( n );
			for(int i=0; i<n-1; i++){
				scanf("%d%d%d",&u[i],&v[i],&w[i]);
				u[i]--; v[i]--;
				s.insert( u[i], v[i], w[i] );
			}
			p[0] = -1; L[0] = 0;
			Get( 0, p, L, next, w, s.V, q );
			int pn = 0; id[0] = 0; r[0] = 0;
			Build( 0, s.V, p, next, w, id, 0, r, pn, P, 0, q );

			while( scanf("%s",ch)==1 && ch[0]!='D' ){
				int x, y, z;
				scanf("%d%d",&x,&y);
				if( ch[0]=='C' ){
					z = y; y = v[x-1]; x = u[x-1];
					if( p[x]==y ) swap( x, y );
					if( next[x]!=y ) w[y] = z;
					else Change( 1, id[y], z, P+id[r[y]] );
				}
				else printf("%d\n", Query( --x, --y, w, p, L, r, id, P ) );
			}
		}
	}
};

int main(){
	Tree::ans();
	return 0;
}


/*
#include<iostream>
#include<stdlib.h>
#include<stdio.h>
using namespace std;

const int INF = (1<<28);

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
		V[end].set( u, V[v].next, w );
		V[v].next = end++;
	}
};

class Tree{
	struct Node{
		int l, r, v, w; bool neg;
		void set( int L, int R, int V ){ l = L; r = R; v = V; w = V; neg = false; }
	};
public:
	static void Init( int ps, int l, int r, int *v, Node *p ){
		p[ps].set( l, r, v[l] );
		if( l==r ) return;
		Init( 2*ps, l, (l+r)/2, v, p );
		Init( 2*ps+1, (l+r)/2+1, r, v, p );
		p[ps].v = max( p[2*ps].v, p[2*ps+1].v );
		p[ps].w = min( p[2*ps].w, p[2*ps+1].w );
	}
	static void Swap( int &u, int &v ){ u = -u; v = -v; swap( u, v ); }
	static void Change( int ps, int lr, int v, Node *p ){
		if( p[ps].l==p[ps].r ){
            p[ps].v = p[ps].w= v;
            p[ps].neg = false; return;
        }
        int mid = (p[ps].l+p[ps].r)/2;
		v = ( p[ps].neg ? -v:v );
		Change( (lr<=mid ? (2*ps):(2*ps+1)), lr, v, p );
        p[ps].v = max( p[2*ps].v, p[2*ps+1].v );
        p[ps].w = min( p[2*ps].w, p[2*ps+1].w );
		if( p[ps].neg ) Swap( p[ps].v, p[ps].w );
	}
	static void Negate( int ps, int l, int r, Node *p ){
        if( l<=p[ps].l && p[ps].r<=r ){
            p[ps].neg = !p[ps].neg;
            Swap( p[ps].v, p[ps].w );
            return;
        }
        int mid = (p[ps].l+p[ps].r)/2;
        if( l<=mid ) Negate( 2*ps, l, r, p );
        if( r>mid ) Negate( 2*ps+1, l, r, p );
        p[ps].v = max( p[2*ps].v, p[2*ps+1].v );
        p[ps].w = min( p[2*ps].w, p[2*ps+1].w );
		if( p[ps].neg ) Swap( p[ps].v, p[ps].w );
    }
	static void Max( int ps, int l, int r, Node *p, int &mx, int &mn ){
		if( l<=p[ps].l && p[ps].r<=r ){
			mx = max( mx, p[ps].v );
			mn = min( mn, p[ps].w );
			return;
		}
		int Mx = -INF, Mn = INF, mid = (p[ps].l+p[ps].r)/2;
		if( l<=mid) Max( 2*ps, l, r, p, Mx, Mn );
		if( r>mid ) Max( 2*ps+1, l, r, p, Mx, Mn );
		if( p[ps].neg ) Swap( Mx, Mn );
		mx = max( mx, Mx );
		mn = min( mn, Mn );
	}
	static void Get( int u, int *p, int *L, int *next, int *w, Link *V, int *size ){
		size[u] = 1; int id = -1;
		for(int i=V[u].next; i!=-1; i=V[i].next){
			int v = V[i].v;
			if( p[u] == v ) continue;
			p[v] = u;
			L[v] = L[u]+1;
			w[v] = V[i].w;
			Get( v, p, L, next, w, V, size );
			if( id==-1 || size[id]<size[v] ) id = v;
			size[u] += size[v];
		}
		next[u] = id;
	}
	static void Build( int u, Link *V, int *p, int *next, int *w, int *id, int root, int *r, int &pn, Node *P, int qn, int *q ){
		int v = next[u];
		if( v == -1 ){
			if( qn>0 ){ Init( 1, 0, qn-1, q, P+pn ); pn += 4*qn; }
			return;
		}
		id[v] = qn; r[v] = root; q[qn++] = w[v];
		Build( next[u], V, p, next, w, id, root, r, pn, P, qn, q );
		for(int i=V[u].next; i!=-1; i=V[i].next){
			v = V[i].v;
			if( p[u]==v || v==next[u] ) continue;
			qn = 0; id[v] = pn; r[v] = v;
			Build( v, V, p, next, w, id, v, r, pn, P, qn, q );
		}
	}
	static void Negate( int u, int v, int *w, int *p, int *L, int *r, int *id, Node *P ){
		while( r[u]!=r[v] ){
			if( L[r[u]]>=L[r[v]] ){
				if( r[u]==u ){ w[u] = -w[u]; u = p[u]; continue; }
				Negate( 1, 0, id[u], P+id[r[u]] );
				u = r[u];
			}
			else{
				if( r[v]==v ){ w[v] = -w[v]; v = p[v]; continue; }
				Negate( 1, 0, id[v], P+id[r[v]] );
				v = r[v];
			}
		}
		if( u == v ) return;
		if( L[u]>L[v] ) swap( u, v );
		if( r[u]==u ) Negate( 1, 0, id[v], P+id[u] );
		else Negate( 1, id[u]+1, id[v], P+id[r[u]] );
	}
	static int Query( int u, int v, int *w, int *p, int *L, int *r, int *id, Node *P ){
		int mx = -INF, mn = INF;;
		while( r[u]!=r[v] ){
			if( L[r[u]]>=L[r[v]] ){
				if( r[u]==u ){ mx = max( mx, w[u] ); u = p[u]; continue; }
				Max( 1, 0, id[u], P+id[r[u]], mx, mn );
				u = r[u];
			}
			else{
				if( r[v]==v ){ mx = max( mx, w[v] ); v = p[v]; continue; }
				Max( 1, 0, id[v], P+id[r[v]], mx, mn );
				v = r[v];
			}
		}
		if( u == v ) return mx;
		if( L[u]>L[v] ) swap( u, v );
		if( r[u]==u ) Max( 1, 0, id[v], P+id[u], mx, mn );
		else Max( 1, id[u]+1, id[v], P+id[r[u]], mx, mn );
		return mx;
	}
	static void ans(){ // poj 3237 Tree
		const int N = 10008;
		char ch[20];
		List s( 3*N );
		int t, n, u[N], v[N], w[N], q[N];
		int p[N], L[N], next[N], id[N], r[N];
		Node *P = new Node[4*N];
		
		scanf("%d",&t);
		while( t-- ){
			scanf("%d",&n);
			s.init( n );
			for(int i=0; i<n-1; i++){
				scanf("%d%d%d",&u[i],&v[i],&w[i]);
				u[i]--; v[i]--;
				s.insert( u[i], v[i], w[i] );
			}
			p[0] = -1; L[0] = 0;
			Get( 0, p, L, next, w, s.V, q );
			int pn = 0; id[0] = 0; r[0] = 0;
			Build( 0, s.V, p, next, w, id, 0, r, pn, P, 0, q );

			while( scanf("%s",ch)==1 && ch[0]!='D' ){
				int x, y, z;
				scanf("%d%d",&x,&y);
				if( ch[0]=='C' ){
					z = y; y = v[x-1]; x = u[x-1];
					if( p[x]==y ) swap( x, y );
					if( next[x]!=y ) w[y] = z;
					else Change( 1, id[y], z, P+id[r[y]] );
				}
				else if( ch[0]=='N' ){
                    Negate( --x, --y, w, p, L, r, id, P );
                }
                else printf("%d\n", Query( --x, --y, w, p, L, r, id, P ) );
			}
		}
	}
};

int main(){
	Tree::ans();
	return 0;
}
*/
