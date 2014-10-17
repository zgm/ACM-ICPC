#include<iostream>
using namespace std;

struct Link{
	int v, next; double w;
	void set( int V, int Next, double W){ v=V; next=Next; w=W; }
};
class List{
public:
	int end; Link *V;
	List( int size = 1024 ){ V = new Link[size]; }
	~List(){ delete []V; }	
	void init( int n ){ end = n; for(int i=0; i<n; i++) V[i].next = -1; }
	void insert( int u, int v, double w ){
		V[end].set( v, V[u].next, w );
		V[u].next = end++;
	}
};

class Graph{
	struct Node{
		int u, v, w;
		void set( int U, int V, int W ){ u=U; v=V; w=W; }
	};
public:
	static void BellmanFord( int s, int t, int n, Link *V, int *d ){
		memset( d, 0, sizeof(int)*n );
		for(int i=0; i<n; i++){
			bool end = true;
			for(int j=0; j<n; j++){
				for(int k=V[j].next; k!=-1; k=V[k].next){
					if( d[j]+V[k].w > d[V[k].v] ){
						d[V[k].v] = d[j]+V[k].w;
						end = false;
					}
				}
			}
			if( end ) break;
		}
	}
	/*static void BellmanFord( int s, int t, int n, int m, Node *P, int *d ){ // get the max
		memset( d, 0, sizeof(int)*n );
		for(int i=0; i<n; i++){
			bool end = true;
			for(int i=0; i<m; i++){
				int u = P[i].u, v = P[i].v;
				if( d[u]+P[i].w > d[v] ){ d[v] = d[u]+P[i].w; end = false; }
			}
			if( end ) break;
		}
	}
	static void ans(){ // poj 1201 Intervals, 1716 Integer Intervals
		const int N = 50008;
		int n, d[N];
		Node P[3*N];
		while( scanf("%d",&n)==1 ){
			int mn = (1<<25), mx = -1, m = n;
			for(int i=0; i<n; i++){
				scanf("%d%d%d",&P[i].u,&P[i].v,&P[i].w);
				mn = min( mn, P[i].u );
				mx = max( mx, ++P[i].v );
			}
			for(int i=mn+1; i<=mx; i++) P[m++].set( i-1, i, 0 );
			for(int i=mx-1; i>=mn; i--) P[m++].set( i+1, i, -1 );
			BellmanFord( mn, mx, mx-mn+1, m, P, d );
			printf("%d\n", d[mx]);
		}
	}*/
	static bool BellmanFord( int n, int m, Node *P, int *d ){
		memset( d, 0, sizeof(int)*n );
		for(int i=0; i<n; i++){
			bool end = true;
			for(int i=0; i<m; i++){
				int u = P[i].u, v = P[i].v;
				if( d[u]+P[i].w < d[v] ){ d[v] = d[u]+P[i].w; end = false; }
			}
			if( end ) return false;
		}
		return true;
	}
	static void ans(){ // 09 Wuhan regional
		freopen("G:\\program\\contest\\09 Îäºº\\B\\TestData\\B.in", "r", stdin);
		freopen("G:\\program\\contest\\09 Îäºº\\B\\TestData\\B.out", "w", stdout);
		const int N = 300000;
		int cs = 0, n, R, d1[N], d2[N], d3[N];
		char c[10];
		Node *px = new Node[N], *py = new Node[N], *pz = new Node[N];
		while( scanf("%d%d",&n,&R)==2 && n+R ){
			int u, v, pxn = 0, pyn = 0, pzn = 0;
			for(int i=0; i<n; i++){
				px[pxn++].set( i+n, i, -1 );
				py[pyn++].set( i+n, i, -1 );
				pz[pzn++].set( i+n, i, -1 );
			}
			for(int i=0; i<R; i++){
				scanf("%s %d %d",c,&u,&v);
				u--; v--;
				if( c[0]=='I' ){
					px[pxn++].set( v+n, u, -1 );
					px[pxn++].set( u+n, v, -1 );
					py[pyn++].set( v+n, u, -1 );
					py[pyn++].set( u+n, v, -1 );
					pz[pzn++].set( v+n, u, -1 );
					pz[pzn++].set( u+n, v, -1 );
				}
				else if( c[0]=='X' ){
					px[pxn++].set( v, u+n, -1 );
				}
				else if( c[0]=='Y' ){
					py[pyn++].set( v, u+n, -1 );
				}
				else if( c[0]=='Z' ){
					pz[pzn++].set( v, u+n, -1 );
				}
			}
			printf("Case %d: ",++cs);
			if( BellmanFord( 2*n, pxn, px, d1 ) || BellmanFord( 2*n, pyn, py, d2 ) || BellmanFord( 2*n, pzn, pz, d3 ) ){
				printf("IMPOSSIBLE\n");
			}
			else{
				printf("POSSIBLE\n");
				for(int i=0; i<n; i++){
					printf("%d %d %d %d %d %d\n", d1[i], d2[i], d3[i], d1[i+n], d2[i+n], d3[i+n] );
				}
			}
		}
	}
};

int main(){
	Graph::ans();
	return 0;
}