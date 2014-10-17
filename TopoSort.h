#include<iostream>
#include<stack>
#include<queue>
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

class TopoSort{
public:
	static void dfs( int u, Link *V, int n, bool *vis, int *f, int &time ){
		vis[u] = true;
		for(int i=V[u].next; i!=-1; i=V[i].next){
			if( !vis[V[i].v] ) dfs( V[i].v, V, n, vis, f, time );
		}
		f[u] = time++;
	}
	//static bool topo_sort( int n, Link *V, int *f, int *ord ){
	//	bool *vis = new bool[n];
	//	memset( vis, 0, sizeof(bool)*n );
	//	/*stack<int> s;
	//	for(int i=0, time=0; i<n; i++){
	//		if( vis[i] ) continue;
	//		for(s.push( i ); !s.empty(); ){
	//			int u = s.top(), j;
	//			vis[u] = true;
	//			for(j=V[u].next; j!=-1; j=V[j].next){
	//				if( !vis[V[j].v] ){ s.push( V[j].v ); break; }
	//			}
	//			if( j == -1 ){ f[u] = time++; s.pop(); }
	//		}
	//	}*/
	//	for(int i=0, time=0; i<n; i++)
	//		if( !vis[i] ) dfs( i, V, n, vis, f, time );
	//	for(int i=0; i<n; i++) ord[n-1-f[i]] = i;
	//	delete []vis;
	//	for(int i=0; i<n; i++){
	//		for(int j=V[i].next; j!=-1; j=V[j].next){
	//			if( f[i] < f[V[j].v] ) return false;
	//		}
	//	}
	//	return true;
	//}
	// for each step, if there are more than one solution, get the one of the largest number
	static int topo_sort( int n, Link *V, int *in, int *ord ){
		memset( in, 0, sizeof(int)*n );
		for(int i=0; i<n; i++)
			for(int j=V[i].next; j!=-1; j=V[j].next) in[V[j].v]++;
		priority_queue<int> q;
		for(int i=0; i<n; i++) if( in[i]==0 ) q.push( i );
		int only = 1;
		for(int i=0; i<n; i++){
			if( q.size() == 0 ) return -1;
			if( q.size() > 1 ) only = 0;
			ord[i] = q.top();
			q.pop();
			in[ord[i]] = -1;
			for(int j=V[ord[i]].next; j!=-1; j=V[j].next){
				in[V[j].v]--;
				if( in[V[j].v] == 0 ) q.push( V[j].v );
			}
		}
		return only;
	}
	static void ans(){ // poj 3687 Labeling Balls
		const int N = 208;
		int t, n, m, in[N], ord[N], w[N];
		scanf("%d",&t);
		List s( 50000 );
		while( t-- ){
			scanf("%d%d",&n,&m);
			s.init( n );
			for(int i=0,u,v; i<m; i++){
				scanf("%d%d",&u,&v);
				s.insert( v-1, u-1 );
			}
			if( topo_sort( n, s.V, in, ord ) == -1 ){ printf("-1\n"); continue; }
			for(int i=0; i<n; i++) w[ord[i]] = n-i;
			printf("%d",w[0]);
			for(int i=1; i<n; i++) printf(" %d", w[i]);
			printf("\n");
		}
	}
	/*static void ans( ){ // poj 1094 Sorting It All Out
		const int N = 30;
		int i, n, m, u, v, in[N], order[N];
		char c[4];
		List s( 1000 );
		while( scanf("%d %d",&n,&m)!=EOF && n+m != 0 ){
			getchar();
			s.init( n );
			for(i=0; i<m; i++){
				scanf("%s",c);
				u = c[0]-'A';
				v = c[2]-'A';
				if( c[1] == '>' ) swap( u, v );
				s.insert( u, v );

				int result = topo_sort( n, s.V, in, order );
				if( result == -1 ){
					printf("Inconsistency found after %d relations.\n",i+1);
					for(i=i+1; i<m; i++) scanf("%s",c);
					i = -1;
					break;
				}
				if( result == 0 ) continue;
				printf("Sorted sequence determined after %d relations: ",i+1);
				for(i=i+1; i<m; i++) scanf("%s",c);
				for(i=0; i<n; i++) printf("%c",'A'+order[i]); printf(".\n");
				i = -1;
				break;
			}
			if( i >= m ) printf("Sorted sequence cannot be determined.\n");
		}
	}*/
};

int main( ){
	TopoSort::ans( );
	return 0;
}