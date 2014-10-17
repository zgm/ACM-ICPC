#include<iostream>
#include<cmath>
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
	void insert( int u, int v, double w=-1, Link *E=NULL ){
		V[end].set( v, V[u].next, w );
		if( E ){ E[end].set( u, E[v].next, w ); E[v].next = end; }
		V[u].next = end++;
	}
    void edit( int u, int v, double w ){
        for(int i=V[u].next; i!=-1; i=V[i].next)
			if( V[i].v == v ){ V[i].w = w; return; }
    } 
};

class Heap{
	struct Node{ int v; double w; };
public:
	int size, *hp; Node *h;
	bool Type; // true: MinHeap, false: MaxHeap
	Heap( int n=1024 ){ h=new Node[n+1]; hp=new int[n]; }
	~Heap(){ delete []h; delete []hp; }
	void Init( int n, bool T=true ){ size=0; Type=T; memset(hp,-1,sizeof(int)*n); }
	void Insert( int v, double w ){
		h[++size].v = v;
		h[size].w = w;
		hp[v] = size;
		Up( size );
	}
	void Swap( int u, int v ){
		hp[h[u].v] = v; hp[h[v].v] = u;
		h[0] = h[u]; h[u] = h[v]; h[v] = h[0];
	}
	void Up( int p ){
		for( ; p>1; p/=2){
            if( Type && h[p/2].w<=h[p].w ) break;
            if( !Type && h[p/2].w>=h[p].w ) break;
            Swap( p/2, p );
        }
	}
	void Down( int p ){
		for(p*=2; p<=size; p*=2){
            if( p+1<=size ){
                if( Type && h[p+1].w<h[p].w ) p++;
                if( !Type && h[p+1].w>h[p].w ) p++;
            }
            if( Type && h[p].w>=h[p/2].w ) break;
            if( !Type && h[p].w<=h[p/2].w ) break;
            Swap( p/2, p );
        }
    }
	int Delete( double &w ){
        int v = h[1].v;
		w += h[1].w; hp[v] = -2;
        Swap( 1, size-- );
        Down( 1 );
		return v;
    }
};

class Graph{
public:
    static double Prim(int n, Link *V, double INF=1e20 ){
        double *d = new double[n], res = 0;
        for(int i=1; i<n; i++) d[i] = INF;
        for(int i=V[0].next; i!=-1; i=V[i].next) d[V[i].v] = V[i].w;
        bool *vis = new bool[n];
        memset( vis, 0, sizeof(bool)*n );
		vis[0] = true;
        for(int i=1; i<n; i++){
            double mn = INF; int id;
            for(int j=1; j<n; j++)
                if( !vis[j] && mn>d[j] ){ mn = d[j]; id = j; }
            res += mn;
            vis[id] = true;
            for(int j=V[id].next; j!=-1; j=V[j].next)
                if( !vis[V[j].v] ) d[V[j].v] = min( d[V[j].v], V[j].w );
        }
        delete []d;
        delete []vis;
        return res;
    }
	static double Prim(int n, Link *V, int st=0, double INF=1e20 ){
        double *d = new double[n], res = 0;
        for(int i=0; i<n; i++) d[i] = INF;

        Heap s( n ); s.Init( n ); s.Insert( st, 0 ); d[st] = 0;
        for(int i=0; i<n; i++){
            int id = s.Delete( res );
            for(int j=V[id].next; j!=-1; j=V[j].next){
                int v = V[j].v;
				if( v==st || s.hp[v]==-2 ) continue;
                if( d[v] > V[j].w ){
                    d[v] = V[j].w;
					if( s.hp[v]==-1 ) s.Insert( v, d[v] );
					else{ s.h[s.hp[v]].w = d[v]; s.Up( s.hp[v] ); }
                }
            }
        }
        delete []d;
        return res;
    }
	static double SPFA( int s, int n, Link *V, int t=-1, double INF=1e20 ){
		double *d = new double[n], res = 0;
		bool *vis = new bool[n];
		for(int i=0; i<n; i++) d[i] = INF;
		memset( vis, 0, sizeof(bool)*n );

		int head = 0, tail = 0, *q = new int[n];
		for(q[tail++]=s,d[s]=0,vis[s]=1; head!=tail; ){
			int v = q[head++];
			head = (head==n ? 0:head);
			for(int i=V[v].next; i!=-1; i=V[i].next){
				int u = V[i].v;
				if( d[v]+V[i].w >= d[u] ) continue;
				d[u] = d[v]+V[i].w;
				if( !vis[u] ){ q[tail++]=u; tail=(tail==n?0:tail); vis[u]=1; }
			}
			vis[v] = false;
		}
		delete []vis;
		delete []q;

		if( t!=-1 ) res = d[t];
		else for(int i=0; i<n; i++) res += d[i];
		delete []d;
		return ( res==INF ? -1:res );
	}
	static double Dijkstra( int st, int n, Link *V, double INF=1e20 ){
        double *d = new double[n], res = 0;
        for(int i=0; i<n; i++) d[i] = INF;

        Heap s( n ); s.Init( n ); s.Insert( st, 0 ); d[st] = 0;
        for(int i=0; i<n; i++){
			if( s.size <= 0 ) return -1;
            int id = s.Delete( res );
            for(int j=V[id].next; j!=-1; j=V[j].next){
                int v = V[j].v;
				if( v==st || s.hp[v]==-2 || d[v]<=d[id]+V[j].w ) continue;
                d[v] = d[id]+V[j].w;
				if( s.hp[v] == -1 ) s.Insert( v, d[v] );
				else{ s.h[s.hp[v]].w = d[v]; s.Up( s.hp[v] ); }
            }
        }
        delete []d;
        return res;
	}

	/*static double dis( int x1, int y1, int x2, int y2){
        double x = (double)(x1-x2);
        double y = (double)(y1-y2);
        return sqrt(x*x+y*y);
    }
    static void ans(){ // poj 3625 Building Roads
        int n, m, i, j, k, p[1008][2];
        while( scanf("%d%d",&n,&m)!=EOF ){
            for(i=0; i<n; i++) scanf("%d%d",&p[i][0],&p[i][1]);
            List s( n+n*n+4 );
            s.init( n );
            for(i=0; i<n; i++){
                for(j=0; j<n; j++){
                    if( i == j ) continue;
                    s.insert( i, j, dis(p[i][0],p[i][1],p[j][0],p[j][1]) );
                }
            }
            for(i=0; i<m; i++){
                scanf("%d%d",&j,&k);
                s.edit( j-1, k-1, 0 );
                s.edit( k-1, j-1, 0 );
            }
            printf("%.2lf\n",Prim( n, s.V ) );
        }
    }*/
	static void ans(){ // poj 1511 Invitation Cards
		int t, n, m;
		scanf("%d",&t);
		while( t-- ){
			scanf("%d%d",&n,&m);
			List s( n+m+4 ); s.init( n );
			List t( n+m+4 ); t.init( n );

			int u, v; double w;
			for(int i=0; i<m; i++){
				scanf("%d%d%lf",&u,&v,&w);
				s.insert( u-1, v-1, w, t.V );
			}
			
			//double res = SPFA( 0, n, s.V );
			//res += SPFA( 0, n, t.V );
			double res = Dijkstra( 0, n, s.V );
			res += Dijkstra( 0, n, t.V );
			printf("%.0lf\n",res);
		}
	}
};

int main(){
    Graph::ans();
    return 0;
}