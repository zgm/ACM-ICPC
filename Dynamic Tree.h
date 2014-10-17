#include<iostream>
#include<stdlib.h>
#include<stdio.h>
#include<algorithm>
#include<time.h>
#include<string.h>
#include<cstring>
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

class SplayTree{
	struct Node{
		int key, size;
		Node *ch[2], *p;
		void set( int Key ){ key=Key; size=1; ch[0]=ch[1]=p=NULL; }
	};
public:
	static int Size( Node *t ){ return (t ? t->size:0); }
	static void Update( Node *t ){
		if( !t ) return;
		t->size = Size(t->ch[0])+Size(t->ch[1])+1;
	}
	static void Rotate( Node *t, bool r ){ // false: left  true: right
		Node *p = t->p;
		p->ch[!r] = t->ch[r];
		if( t->ch[r] ) t->ch[r]->p = p;
		t->ch[r] = p; t->p = p->p; p->p = t;

		Update( p ); Update( t );
		if( ! t->p ) return;
		t->p->ch[t->p->ch[1]==p] = t;
	}
	static Node* Splay( Node *t ){
		for(Node *p,*pp; t->p; ){
			p = t->p;
			if( ! p->p ){ Rotate( t, p->ch[0]==t ); continue; }

			pp = p->p;
			if( p->ch[0]==t ){
				if( pp->ch[0]==p ){ Rotate( p, 1 ); Rotate( t, 1 ); }
				else{ Rotate( t, 1 ); Rotate( t, 0 ); }
			}
			else{
				if( pp->ch[0]==p ){ Rotate( t, 0 ); Rotate( t, 1 ); }
				else{ Rotate( p, 0 ); Rotate( t, 0 ); }
			}
		}
		return t;
	}
	static Node* Maximum( Node *t ){ return t->ch[1] ? Maximum( t->ch[1] ):t; }
	static Node* Minimum( Node *t ){ return t->ch[0] ? Minimum( t->ch[0] ):t; }
	static Node *Predecessor( Node *t ){
		if( t->ch[0] ) return Maximum( t->ch[0] );
		Node *r = t->p;
		for( ; r && t==r->ch[0]; t=r,r=r->p);
		return r;
	}
	static Node *Successor( Node *t ){
		if( t->ch[1] ) return Minimum( t->ch[1] );
		Node *r = t->p;
		for( ; r && t==r->ch[1]; t=r,r=r->p);
		return r;
	}
	static Node *Join( Node *t1, Node *t2 ){
		if( !t1 ) return t2;
		if( !t2 ) return t1; 
		Node *t = Splay( Maximum(t1) );
		t->ch[1] = t2; t->size += t2->size; t2->p = t;
		Update( t );
		return t;
	}
	static void Show( int k, Node *t ){
		if( t->ch[1] ) Show( k+1, t->ch[1] );
		for(int i=0; i<k; i++) printf("  "); printf("%d\n",t->key);
		if( t->ch[0] ) Show( k+1, t->ch[0] );
	}
	static void Show( int &num, int n, Node *t ){
		if( t->ch[0] ) Show( num, n, t->ch[0] );
		if( num!=0 && num!=n ){
			if( num!=1 ) printf(" ");
			printf("%d",t->key);
		}
		num++;
		if( t->ch[1] ) Show( num, n, t->ch[1] );
	}
	static void bfs( int s, int n, Link *V, int &an, int *a, int *q, int *next ){
		int qn = 0; q[0] = s;
		while( qn>=0 ){
			int u = q[qn];
			if( next[u]==V[u].next ) a[an++] = u;
			if( next[u]==-1 ){ a[an++] = u+n; qn--; continue; }
			q[++qn] = V[next[u]].v;
			next[u] = V[next[u]].next;
		}
	}
	/*static void dfs( int u, int n, Link *V, int &an, int *a ){
		a[an++] = u;
		for(int i=V[u].next; i!=-1; i=V[i].next){
			int v = V[i].v;
			dfs( v, n, V, an, a );
		}
		a[an++] = u+n;
	}*/
	static Node* Init( Node *pp, Node *P, int l, int r, int *x ){
		int mid = (l+r)/2, w = x[mid];
		P[w].set( x[mid] ); P[w].p = pp;
		if( l<mid ) P[w].ch[0] = Init( &P[w], P, l, mid-1, x );
		if( mid<r ) P[w].ch[1] = Init( &P[w], P, mid+1, r, x );
		Update( &P[w] );
		return &P[w];
	}
	static void ans(){ // hdu 2475  Box
		const int N = 50008;
		int cs = 0, n, m, *x = new int[2*N], q[N], next[N], a, b;
		bool ok[N]; char c[10];
		Node *p = new Node[2*N], *root, *pre, *suc;
		List s( 3*N );
		while( scanf("%d",&n)==1 ){
			if( cs++ ) printf("\n");
			s.init( n );
			memset( ok, 0, sizeof(bool)*n );
			for(int i=0,j; i<n; i++){
				scanf("%d",&j);
				if( j==0 ) ok[i] = true;
				else s.insert( j-1, i );
			}
			for(int i=0; i<n; i++) next[i] = s.V[i].next;
			for(int i=0; i<n; i++){
				if( !ok[i] ) continue;
				int an = 0;
				bfs( i, n, s.V, an, x, q, next );
				//dfs( i, n, s.V, an, x );
				//for(int i=0; i<an; i++) printf("%d ",x[i]>=n ? x[i]-n:x[i]); printf("\n");
				//for(int i=0; i<an; i++) printf("%d ",x[i]); printf("\n");
				root = Init( NULL, p, 0, an-1, x );
				//Show( 0, root );
			}
			
			scanf("%d",&m);
			while( m-- ){
				scanf("%s",c);
				if( c[0]=='M' ){
					scanf("%d%d",&a,&b);
					if( a==b ) continue;
					a--; b--;
					root = Splay( &p[a] );
					if( root->ch[0]==NULL ){
						if( b==-1 ) continue;
						root = Minimum( Splay(&p[b]) );
						if( root==&p[a] ) continue;
						root = Join( &p[a], p[b].ch[1] );
						p[b].ch[1] = root; root->p = &p[b]; Update( &p[b] );
					}
					else{
						if( b!=-1 && Minimum(Splay(&p[a]))==Minimum(Splay(&p[b])) ){
							root = Splay( &p[a] ); int size1 = Size( root->ch[0] );
							root = Splay( &p[a+n] ); int size2 = Size( root->ch[0] );
							root = Splay( &p[b] ); int size3 = Size( root->ch[0] );
							root = Splay( &p[b+n] ); int size4 = Size( root->ch[0] );
							if( size1<size3 && size4<size2 ) continue;
						}

						pre = Predecessor( &p[a] );
						suc = Successor( &p[a+n] );
						Splay( suc ); Splay( pre );
						if( pre->ch[1]!=suc ) Rotate( suc, 0 );
						root = suc->ch[0];
						suc->ch[0]->p = NULL; suc->ch[0] = NULL;
						Update( suc ); Update( pre );

						if( b!=-1 ){
							Splay( &p[b] );
							root = Join( root, p[b].ch[1] );
							p[b].ch[1] = root; root->p = &p[b]; Update( &p[b] );
						}
					}
				}
				else{
					scanf("%d",&a);
					root = Minimum( Splay(&p[a-1]) );
					printf("%d\n",root->key+1);
				}
				/*for(int i=0; i<n; i++){
					root = Splay( &p[i] );
					if( root->ch[0]==NULL ) Show( 0, root );
				}*/
			}
		}
	}
};

int main(){
	SplayTree::ans();
	return 0;
}



/*const int N = 10000, INF = (1<<30);

struct Node{
	int cost, maxcost, p, ch[2], nodetype;
};

Node P[N];
int dest[2*N], next[2*N], cost[2*N], start[N], bottom[N];

void dfs( int node ){
	for(int i=start[node]; i!=0; i=next[i] ){
		if( dest[i]!=P[node].p ){
			bottom[(i+1)/2] = dest[i];
			P[dest[i]].p = node;
			P[dest[i]].ch[0] = 0;
			P[dest[i]].ch[1] = 0;
			P[dest[i]].nodetype = 2;
			P[dest[i]].cost = cost[i];
			P[dest[i]].maxcost = cost[i];
			dfs( dest[i] );
		}
	}
}
void update( int root ){
	P[root].maxcost = P[root].cost;
	int x = P[P[root].ch[0]].maxcost;
	int y = P[P[root].ch[1]].maxcost;
	if( x<y ) x = y;
	if( x>P[root].maxcost ) P[root].maxcost = x;
}
void rotate( int node, int x ){
	int p = P[node].p;
	int y = P[p].nodetype;
	P[node].p = P[p].p;
	P[node].nodetype = y;
	if( y!=2 ) P[P[p].p].ch[y] = node;
	y = 1-x;
	P[P[node].ch[y]].p = p;
	P[P[node].ch[y]].nodetype = x;
	P[p].ch[x] = P[node].ch[y];
	P[p].p = node;
	P[p].nodetype = y;
	P[node].ch[y] = p;
	update( p );
}
void splay( int z ){
	while( true ){
		int a = P[z].nodetype;
		if( a==2 ) break;
		int y = P[z].p;
		int b = P[y].nodetype;
		if( a==b ) rotate( y, a );
		else rotate( z, a );
		if( b==2 ) break;
		rotate( z, b );
	}
	update( z );
}
int query( int a, int b ){
	int i = 0, s = 0;
	while( a!= 0 ){
		splay( a );
		P[P[a].ch[1]].nodetype = 2;
		P[a].ch[1] = i;
		P[P[a].ch[1]].nodetype = 1;
		update( a );
		i = a;
		a = P[a].p;
	}
	i = 0;
	while( b!=0 ){
		splay( b );
		if( P[b].p==0 ){
			s = P[P[b].ch[1]].maxcost;
			if( P[i].maxcost>s ) s = P[i].maxcost;
		}
		P[P[b].ch[1]].nodetype = 2;
		P[b].ch[1] = i;
		P[P[b].ch[1]].nodetype = 1;
		update( b );
		i = b;
		b = P[b].p;
	}
	return s;
}

int main(){
	int t, n, a, b, c;
	scanf("%d",&t);
	for(int i=1; i<=t; i++){
		scanf("%d",&n);
		for(int j=1; j<=n; j++) start[j] = 0;
		for(int j=2,k=0; j<=n; j++){
			scanf("%d%d%d",&a,&b,&c);
			k++;
			dest[k] = b;
			cost[k] = c;
			next[k] = start[a];
			start[a] = k;

			k++;
			dest[k] = a;
			cost[k] = c;
			next[k] = start[b];
			start[b] = k;
		}
		P[0].ch[0] = P[0].ch[1] = 0;
		P[0].p = 0;
		P[0].cost = -INF;
		P[0].maxcost = -INF;
		P[0].nodetype = 2;

		P[1].ch[0] = P[1].ch[1] = 0;
		P[1].p = 0;
		P[1].cost = -INF;
		P[1].maxcost = -INF;
		P[1].nodetype = 2;
		dfs( 1 );

		char s[10];
		while( true ){
			scanf("%s",s);
			if( s[0]=='C' ){
				scanf("%d%d",&n,&c);
				P[bottom[n]].cost = c;
				splay( bottom[n] );
			}
			else if( s[0]=='Q' ){
				scanf("%d%d",&a,&b);
				printf("%d\n",query(a,b));
			}
			else break;
		}
	}
	return 0;
}*/