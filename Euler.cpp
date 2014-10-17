#include<stdlib.h>
#include<stdio.h>
#include<cstring>

struct Link{
	int v, next; bool vis;
	void set( int V=-1, int Next=-1){ v=V; next=Next; vis=false; }
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

int path[10008], ps = 0;

class Euler{
public:
	static int Find( int u, int *p ){
		if( p[u] < 0 ) return u;
		else{ p[u] = Find(p[u], p); return p[u]; }
	}
	static void Union( int u, int v, int *p ){
		int pu = Find( u, p );
		int pv = Find( v, p );
		if( pu != pv ){
			int tmp = p[pu]+p[pv];
			if( p[pu] > p[pv] ){ p[pu] = pv; p[pv] = tmp; }
			else{ p[pv] = pu; p[pu] = tmp; }
		}
	}
	static void euler( int u, Link *V ){
		for(int i=V[u].next; i!=-1; i=V[i].next){
			if( V[i].vis ) continue;
			V[i].vis = true; V[i^1].vis = true;
			euler( V[i].v, V );
			//break;
		}
		path[ps++] = u;
	}
	static void ans(){ // poj 2202 Ã»¹ý
		List s( 300000 );
		int n, m, d[10008], p[10008];
		while( scanf("%d%d",&n,&m)==2 ){
			s.init( n%2 ? (n+1):n );
			memset( d, 0, sizeof(int)*n );
			memset( p, -1, sizeof(int)*n );
			for(int i=0,u,v; i<m; i++){
				scanf("%d%d",&u,&v);
				u--; v--;
				d[u]++; d[v]++;
				Union( u, v, p );
				s.insert( u, v );
				s.insert( v, u );
			}ps = 0;
			if( p[0]!=-n ){ printf("-1\n"); continue; }
			bool ok = true;
			for(int i=0; i<n; i++) if( d[i]%2 ){ ok = false; break; }
			if( !ok ){ printf("-1\n"); continue; }
			
			euler( 0, s.V );
			for(int i=n-1; i>=0; i--){
				printf("%d",path[i]+1);
				if( i ) printf(" ");
			}
			printf("\n");
		}
	}
	/*static void euler( int u, int &top, int *st, int n, int *edge ){
		for(int i=0; i<n; i++){
			if( edge[u*n+i] ){
				edge[u*n+i]--;
				edge[i*n+u]--;
				euler( i, top, st, n, edge );
			}
		}
		st[top++] = u;
	}
	static void ans(){ // sgu 101
		const int N = 7, M = 101;
		int m, u[M], v[M], d[N], st[M], edge[N*N];
		while( scanf("%d",&m)==1 ){
			memset( d, 0, sizeof(int)*N );
			memset( edge, 0, sizeof(int)*N*N );
			for(int i=0; i<m; i++){
				scanf("%d%d",&u[i],&v[i]);
				d[u[i]]++; d[v[i]]++;
				edge[u[i]*N+v[i]]++;
				edge[v[i]*N+u[i]]++;
			}
			int top = 0, root;
			for(int i=0; i<N; i++)
				if( d[i]%2 ){ top++; root = i; }
			if( top>2 ){ printf("No solution\n"); continue; }
			if( top==0 ){
				for(int i=0; i<N; i++) if( d[i] ){ root = i; break; }
			}
			top = 0;
			euler( root, top, st, N, edge );
			if( top!=m+1 ){ printf("No solution\n"); continue; }
			else{
				for(int i=0; i<m; i++){
					for(int j=0; j<m; j++){
						if( u[j]==st[i]&&v[j]==st[i+1] ){
							printf("%d +\n",j+1);
							u[j] = v[j] = -1;
							break;
						}
						else if( v[j]==st[i]&&u[j]==st[i+1] ){
							printf("%d -\n",j+1);
							u[j] = v[j] = -1;
							break;
						}
					}
				}
			}
		}
	}*/
};

/*int main(){
	Euler::ans();
	return 0;
}*/