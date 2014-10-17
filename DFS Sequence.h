#include<iostream>
#include<cstdio>
#include<cstring>
#include<algorithm>
using namespace std;

const int INF = (1<<25);

struct Link{
	int v, next;
	void set( int V, int Next ){ v = V; next = Next; }
};
class List{
public:
	int end; Link *V;
	List(int n=1024){ V = new Link[n]; }
	~List(){ delete []V; }
	void init(int n){ end = n; for(int i=0; i<n; i++) V[i].next = -1; }
	void insert( int u, int v ){
		V[end].set( v, V[u].next );
		V[u].next = end++;
	}
};

void bfs( int u, Link *V, int &dn, int *d, int *pos, int *q, int *next, bool *vis, int n ){
	int tail = 0; q[0] = u; vis[u] = true;
	d[dn++] = -1; d[dn++] = u; pos[u] = dn-1;
	for(int i=0; i<n; i++) next[i] = V[i].next;
	while( tail>=0 ){
		int u = q[tail], i;
		for(i=next[u]; i!=-1; i=V[i].next){
			if( !vis[V[i].v] ){
				q[++tail] = V[i].v;
				d[dn++] = -1; d[dn++] = V[i].v; pos[V[i].v] = dn-1;
				vis[V[i].v] = true;
				break;
			}
		}
		if( i!=-1 ){ next[u] = V[i].next; }
		else{ tail--; next[u] = -1; d[dn++] = -2; }
	}
}
void dfs( int u, int p, Link *V, int &n, int *d, int *pos ){
	d[n++] = -1; d[n++] = u; pos[u] = n-1;
	for(int i=V[u].next; i!=-1; i=V[i].next){
		if( V[i].v!=p ) dfs( V[i].v, u, V, n, d, pos );
	}
	d[n++] = -2;
}

struct Node{
	int right, left, rp, rm, lp, lm, mx;
	void set( int key, bool color ){
		if( key==-1 ){ right = 0; left = 1; }
		else if( key==-2 ){ right = 1; left = 0; }
		else{ right = 0; left = 0; }
		mx = -INF;
		if( key>=0 && !color ) rp = rm = lp = lm = 0;
		else rp = rm = lp = lm = -INF;
	}
};

void Update( int ps, Node *p ){
	if( p[2*ps].left>=p[2*ps+1].right ){
		p[ps].right = p[2*ps].right;
		p[ps].left = p[2*ps].left-p[2*ps+1].right+p[2*ps+1].left;
	}else{
		p[ps].right = p[2*ps].right-p[2*ps].left+p[2*ps+1].right;
		p[ps].left = p[2*ps+1].left;
	}
	p[ps].mx = max( p[2*ps].mx, p[2*ps+1].mx );
	p[ps].mx = max( p[ps].mx, p[2*ps].rp+p[2*ps+1].lm );
	p[ps].mx = max( p[ps].mx, p[2*ps].rm+p[2*ps+1].lp );

	p[ps].rp = max( p[2*ps+1].rp, p[2*ps].rp-p[2*ps+1].right+p[2*ps+1].left );
	p[ps].rp = max( p[ps].rp, p[2*ps].rm+p[2*ps+1].right+p[2*ps+1].left );

	p[ps].rm = max( p[2*ps].rm+p[2*ps+1].right-p[2*ps+1].left, p[2*ps+1].rm );

	p[ps].lp = max( p[2*ps].lp, p[2*ps+1].lp+p[2*ps].right-p[2*ps].left );
	p[ps].lp = max( p[ps].lp, p[2*ps+1].lm+p[2*ps].right+p[2*ps].left );

	p[ps].lm = max( p[2*ps+1].lm-p[2*ps].right+p[2*ps].left, p[2*ps].lm );
}
void Init( int ps, Node *p, int l, int r, int *d ){
	if( l==r ) p[ps].set( d[r], false );
	else{
		Init( 2*ps, p, l, (l+r)/2, d );
		Init( 2*ps+1, p, (l+r)/2+1, r, d );
		Update( ps, p );
	}
}
void Set( int ps, Node *p, int l, int r, int w, bool color ){
	if( l==r ) p[ps].set( 0, color );
	else{
		if( w<=(l+r)/2 ) Set( 2*ps, p, l, (l+r)/2, w, color );
		else Set( 2*ps+1, p, (l+r)/2+1, r, w, color );
		Update( ps, p );
	}
}


const int N = 100008;
int n, m, u, v, d[3*N], pos[N], q[2*N]; char c[10]; bool color[N];
Node *p = new Node[12*N];
List s( 2000000 );

int main(){ // zjoi 1095  Hide
	while( scanf("%d",&n)==1 ){
		s.init( n );
		for(int i=1; i<n; i++){
			scanf("%d%d",&u,&v);
			s.insert( u-1, v-1 );
			s.insert( v-1, u-1 );
		}
		int dn = 0, num = n;
		memset( color, 0, sizeof(bool)*n );
		bfs( 0, s.V, dn, d, pos, q, q+N, color, n );
		//dfs( 0, -1, s.V, dn, d, pos );
		Init( 1, p, 0, dn-1, d );
		memset( color, 0, sizeof(bool)*n );
		for(scanf("%d",&m); m>0; m-- ){
			scanf("%s",c);
			if( c[0]=='G' ){
				if( num==0 ) printf("-1\n");
				else if( num==1 ) printf("0\n");
				else printf("%d\n",p[1].mx);
			}else{
				int u; scanf("%d",&u); u--;
				color[u] = (!color[u]);
				if( color[u] ) num--;
				else num++;
				Set( 1, p, 0, dn-1, pos[u], color[u] );
			}
		}
	}
	return 0;
}
