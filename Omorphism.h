#include<iostream>
#include<stack>
#include<cstring>
#include<algorithm>
using namespace std;

struct Link{
    int v, next, num; bool vis;
    void set( int V=-1, int Next=-1){ v=V; next=Next; vis=false; }
};
class List{
public:
    int end; Link *V;
    List( int size = 1024 ){ V = new Link[size]; }
    ~List(){ delete []V; }
	void init( int n ){ end = n; for(int i=0; i<n; i++){ V[i].next=-1; V[i].num=0; } }
    void insert( int u, int v ){
        V[end].set( v, V[u].next );
        V[u].next = end++; V[u].num++;
        
		//V[end].set( u, V[v].next );
        //V[v].next = end++; V[v].num++;
    }
};

class Omorphism{
public:
	static bool Same( int u, Link *V, int v, Link *E, int *d1, int *d2 ){
		if( V[u].num != E[v].num ) return false;
		for(int i=V[u].next; i!=-1; i=V[i].next){
			if( d1[V[i].v]<d1[u] ) continue;
			int j = E[v].next;
			for( ; j!=-1; j=E[j].next){
				if( d2[E[j].v]<d2[v] || E[j].vis ) continue;
				if( Same( V[i].v, V, E[j].v, E, d1, d2 ) ){ E[j].vis = true; break; }
			}
			if( j!=-1 ) continue;
			for(int j=E[v].next; j!=-1; j=E[j].next) E[j].vis=false;
			return false;
		}
		for(int i=E[v].next; i!=-1; i=E[i].next) E[i].vis=false;
		return true;
	}
	static void spfa( int s, int n, Link *V, int *d, bool *vis ){
		memset( vis, 0, sizeof(bool)*n );
		for(int i=0; i<n; i++) d[i] = n;
		d[s] = 0; vis[s] = true;
		int head = 0, tail = 0, *q = new int[10*n];
		for( q[tail++]=s; head<tail; ){
			int v = q[head++];
			for(int i=V[v].next; i!=-1; i=V[i].next){
				int u = V[i].v;
				if( d[v]+1 < d[u] ){
					d[u] = d[v]+1;
					if( !vis[u] ){ q[tail++] = u; vis[u] = true; }
				}
			}
			vis[v] = false;
		}
		delete []q;
	}
	static bool Same( int n, int *d1, int *d2, int *d ){
		memcpy( d, d1, sizeof(int)*n );
		sort( d, d+n );
		memcpy( d+n, d2, sizeof(int)*n );
		sort( d+n, d+2*n );
		for(int i=0; i<n; i++) if( d[i]!=d[i+n] ) return false;
		return true;
	}
	/*static void ans(){ // ustc 1117 : Bean Language
		const int N = 1008;
		int T, n, d1[N], d2[N], d[2*N];
		bool vis[N];
		scanf("%d",&T);
		List s( 4000 ), t( 4000 );
		while( T-- ){
			scanf("%d",&n);
			s.init( n ); t.init( n );
			for(int i=1,u,v; i<n; i++){
				scanf("%d%d",&u,&v);
				s.insert( u-1, v-1 );
			}
			for(int i=1,u,v; i<n; i++){
				scanf("%d%d",&u,&v);
				t.insert( u-1, v-1 );
			}
			spfa( 0, n, s.V, d1, vis );
			int i = 0;
			for( ; i<n; i++){
				spfa( i, n, t.V, d2, vis );
				//if( !Same( n, d1, d2, d ) ) continue;
				memset( vis, 0, sizeof(bool)*n );
				if( Same( 0, s.V, i, t.V, d1, d2 ) ){ printf("same\n"); break; }
			}
			if( i==n ) printf("different\n");
		}
	}*/
	static void ans(){ // poj 1635: Subway tree systems
		const int N = 3008;
		int T, d1[N], d2[N], d[2*N];
		bool vis[N]; char r[N];
		scanf("%d",&T);
		stack<int> p;
		List s( 5000 ), t( 5000 );
		while( T-- ){
			scanf("%s",r);
			int n = 0, m = 0;
			s.init( N ); t.init( N );
			while( !p.empty() ) p.pop(); p.push( 0 );
			for(int i=0,k=strlen(r); i<k; i++){
				if( r[i]=='0' ){ s.insert( p.top(), ++n ); p.push( n ); }
				else p.pop();
			}
			scanf("%s",r);
			while( !p.empty() ) p.pop(); p.push( 0 );
			for(int i=0,k=strlen(r); i<k; i++){
				if( r[i]=='0' ){ t.insert( p.top(), ++m ); p.push( m ); }
				else p.pop();
			}
			if( n!=m ){ printf("different\n"); continue; }
			n++;
			spfa( 0, n, s.V, d1, vis );
			spfa( 0, n, t.V, d2, vis );
			memset( vis, 0, sizeof(bool)*n );
			if(  Same( n, d1, d2, d ) && Same( 0, s.V, 0, t.V, d1, d2 ) ) printf("same\n");
			else printf("different\n");
		}
	}
};

int main(){
	Omorphism::ans();
	return 0;
}