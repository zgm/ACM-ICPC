#include<iostream>
#include<math.h>
#include<cmath>
#include<vector>
#include<time.h>
#include<queue>
#include<map>
#include<set>
#include<cstring>
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
	void init( int n ){ end = (n&1 ? n+1:n); for(int i=0; i<end; i++) V[i].next = -1; }
	void insert( int u, int v, int c1, int c2=0 ){
		V[end].set( u, v, c1, V[u].next );
		V[u].next = end++;
		V[end].set( v, u, c2, V[v].next );
		V[v].next = end++;
	}
	void resume( int n ){
		for(int i=n; i<end; i+=2){ V[i].c += V[i+1].c; V[i+1].c = 0; }
	}
	void resume( int u, int v ){
		for(int i=V[u].next; i!=-1; i=V[i].next){
			if( V[i].v==v ){ V[i].c = V[i+1].c; V[i+1].c = 0; break; }
		}
	}
	bool find( int u, int v ){ // 判断u v之间是否有流
		for(int i=V[u].next; i!=-1; i=V[i].next){
			if( i%2==0 && V[i].v==v && V[i+1].c>0 ) return true;
		}
		return false;
	}
	void edit( int u, int v, int c ){
		for(int i=V[u].next; i!=-1; i=V[i].next){
			if( i%2==0 && V[i].v==v ){ V[i].c += c; V[i+1].c -= c; break; }
		}
	}
	void change( int u, int v, int c ){
		for(int i=V[u].next; i!=-1; i=V[i].next){
			if( i%2==0 && V[i].v==v ){ V[i].c += c; break; }
		}
	}
	void clear( int u, int v ){
		for(int i=V[u].next; i!=-1; i=V[i].next){
			if( i%2==0 && V[i].v==v ){ V[i].c = 0; V[i+1].c = 0; break; }
		}
	}
	void show( int n ){
		for(int i=0; i<n; i++){
			for(int j=V[i].next; j!=-1; j=V[j].next){
				if( j%2==0 && V[j+1].c>0 ) printf("%d %d %d\n",i,V[j].v,V[j+1].c);
			}
		}
	}
};

class MaxFlow{
public:
	static bool bfs( int s, int t, int n, bool *vis, int *q, int *h, Link *V, bool *del=NULL){
		memset( vis, 0, sizeof(bool)*n );
		for(int i=0; i<n; i++) h[i] = n;
		int head = 0, tail = 0, u, v;
		for(h[s]=0, q[tail++]=s, vis[s]=1; head!=tail; ){
			u = q[head++];
			head = (head==n ? 0:head);
			for(int i=V[u].next; i!=-1; i=V[i].next){
				v = V[i].v;
				if( del!=NULL && del[v] ) continue;
				if( V[i].c <= 0 || h[v] <= h[u]+1 ) continue;
				h[v] = h[u]+1;
				if( !vis[v] ){ q[tail++]=v; tail=(tail==n?0:tail); vis[v]=true; }
			}
			vis[u] = false;
		}
		return (h[t]!=n);
	}
	static void dfs( int s, int t, int *q, int *h, Link *V, int &flow ){
		q[0] = s; V[q[0]].v = s;
		for(int i=s, j, tail=0; tail>=0; ){
			if( i == t ){
				int r = -1, ri = -1;
				for(i=1; i<=tail; i++) if( r==-1 || r>V[q[i]].c ){ r = V[q[i]].c; ri = i; }
				for(i=1; i<=tail; i++){ V[q[i]].c -= r; V[q[i]^1].c += r; }
				tail = ri-1;
				i = V[q[tail]].v;
				flow += r;
			}
			for(j=V[i].next; j!=-1; j=V[j].next){
				if( V[j].c>0 && h[i]+1==h[V[j].v] ){ q[++tail] = j; i = V[j].v; break; }
			}
			if( j!=-1 ) continue;
			if( tail == 0 ) break;
			h[ V[q[tail]].v ] = -1;
			i = V[ q[--tail] ].v;
		}
	}
	static int Dinic( int s, int t, int n, Link *V, bool *del=NULL ){
		int flow = 0, *q = new int[2*n];
		bool *vis = new bool[n];
		while( bfs( s, t, n, vis, q, q+n, V, del ) ){
			dfs( s, t, q, q+n, V, flow );
		}
		delete []q;
		delete []vis;
		return flow;
	}
	static bool Augment( int u, int v, int n, Link *V, int *q, int *p, bool *del=NULL ){
		int head = 0, tail = 0;
		memset( p, 0, sizeof(int)*n );
		for(q[tail++]=u; head!=tail && p[v]==0; ){
			int k = q[head++];
			if( head == n ) head = 0;
			for(int i=V[k].next; i!=-1; i=V[i].next){
				int t = V[i].v;
				if( p[t] || (del!=NULL && del[t]) ) continue;
				if( V[i].c > 0 ){
					p[t] = i; q[tail++] = t;
					if( tail == n ) tail = 0;
				}
			}
		}
		if( p[v]==0 ) return false;
		for( ; v!=u; v=V[p[v]].u ){ V[p[v]].c--; V[p[v]^1].c++; }
		return true;
	}
	static void ans(){ // nkoj 2126  最长递增子序列问题
		const int N = 512;
		int n, a[N], d[N];
		List s( 100000 );
		while( scanf("%d",&n)==1 ){
			for(int i=0; i<n; i++) scanf("%d",&a[i]);
			int res = 1;
			for(int i=n-1; i>=0; i--){
				d[i] = 1;
				for(int j=i+1; j<n; j++)
					if( a[i]<a[j] ) d[i] = max( d[i], d[j]+1 );
				res = max( res, d[i] );
			}
			printf("%d\n",res);
			//for(int i=0; i<n; i++) printf("%d ",d[i]); printf("\n");
			s.init( 2*n+2 );
			for(int i=0; i<n; i++){
				s.insert( i, i+n, 1 );
				for(int j=i+1; j<n; j++)
					if( a[i]<a[j] && d[i]==d[j]+1 ) s.insert( i+n, j, 1 );
				if( d[i]==res ) s.insert( 2*n, i, 1 );
				if( d[i]==1 ) s.insert( i+n, 2*n+1, 1 );
			}
			printf("%d\n",Dinic( 2*n, 2*n+1, 2*n+2, s.V ) );
			s.resume( 2*n+2 );
			s.change( 2*n, 0,  n );
			s.change( 0, n, n );
			s.change( n-1, 2*n-1, n );
			s.change( 2*n-1, 2*n+1, n );
			printf("%d\n",Dinic( 2*n, 2*n+1, 2*n+2, s.V ) );
		}
	}
	/*static void ans(){ // spoj 839  Optimal Marks
		const int N = 512, M = 3008, INF = (1<<28);
		int T, n, m, k, p[M][2], d[N], r[N];
		List s( 1000000 );
		scanf("%d",&T);
		queue<int> q; bool vis[N];
		while( T-- ){
			scanf("%d%d",&n,&m);
			int nn = (n%2 ? n+3:n+2);
			for(int i=0; i<m; i++) scanf("%d%d",&p[i][0],&p[i][1]);
			scanf("%d",&k);
			memset( d, -1, sizeof(int)*N );
			for(int i=0,u,v; i<k; i++){
				scanf("%d%d",&u,&v);
				d[u] = v;
			}
			memset( r, 0, sizeof(int)*N );
			for(int i=0; i<32; i++){
				s.init( nn );
				for(int j=0; j<m; j++){
					s.insert( p[j][0], p[j][1], 1 );
					s.insert( p[j][1], p[j][0], 1 );
				}
				for(int j=1; j<=n; j++){
					if( d[j]<0 ) continue;
					if( d[j]&(1<<i) ) s.insert( 0, j, INF );
					else s.insert( j, n+1, INF );
				}
				Dinic( 0, n+1, n+2, s.V );
				memset( vis, 0, sizeof(bool)*N );
				for(q.push(0),vis[0]=vis[n+1]=1; !q.empty(); q.pop()){
					for(int j=s.V[q.front()].next; j!=-1; j=s.V[j].next){
						int v = s.V[j].v;
						if( s.V[j].c && !vis[v] ){ vis[v]=1; r[v]+=(1<<i); q.push( v ); }
					}
				}
			}
			for(int i=1; i<=n; i++) printf("%d\n",d[i]==-1?r[i]:d[i]);
		}
	}*/
	/*static void ans(){ // poj 1815 Friendship
		int n, s, t, a[201][201], p[401], q[401];
		bool del[401];
		List r( 10000 );
		while( scanf("%d%d%d",&n,&s,&t)==3 ){
			s--; t--;
			r.init( 2*n );
			for(int i=0; i<n; i++) r.insert( i, i+n, 1 );
			for(int i=0; i<n; i++){
				for(int j=0; j<n; j++){
					scanf("%d",&a[i][j]);
					if( j<=i || a[i][j]==0 ) continue;
					r.insert( i+n, j, 1 );
					r.insert( j+n, i, 1 );
				}
			}
			if( a[s][t] ){ printf("NO ANSWER!\n"); continue; }
			int flow = Dinic( s+n, t, 2*n, r.V );
			printf("%d\n", flow );
			if( flow == 0 ) continue;
			memset( del, 0, sizeof(bool)*2*n );
			for(int i=0, j; i<n && flow>0; i++){
				if( i==s || i==t ) continue;
				for(j=r.V[i].next; j!=-1; j=r.V[j].next){
					if( j%2==0 && r.V[j+1].c>0 ) break;
				}
				if( j==-1 ) continue;
				del[i] = true;
				Augment( i, s+n, 2*n, r.V, q, p, del );
				Augment( t, i+n, 2*n, r.V, q, p, del );
				r.resume( i, i+n );
				if( !Augment( s+n, t, 2*n, r.V, q, p, del ) ){
					flow--; printf("%d", i+1);
					if( flow>0 ) printf(" ");
					del[i] = del[i+n] = true;
				}
				else del[i] = false;
			}
			printf("\n");
		}
	}*/
};

class ConnectingAirports{
public:
	static vector<string> getSchedule(vector<int>A, vector<int>U){ // TCO'09 Round 2 DIV1
		int n = A.size(), m = U.size(), an = 0, un = 0;
		for(int i=0; i<n; i++) an += A[i];
		for(int i=0; i<m; i++) un += U[i];
		vector<string> res;
		if( an != un ) return res;
		List s( 10000 );
		int N = n+m+2;
		s.init( N%2 ? (N+1):N );
		for(int i=0; i<n; i++) s.insert( 0, i+1, A[i] );
		for(int i=0; i<m; i++) s.insert( n+i+1, N-1, U[i] );
		for(int i=0; i<n; i++){
			for(int j=0; j<m; j++) s.insert( i+1, n+j+1, 1 );
		}
		int flow = MaxFlow::Dinic( 0, N-1, N, s.V );
		if( flow != an ) return res;
		bool *del = new bool[N];
		int *q = new int[2*N], *p = q+N;
		memset( del, 0, sizeof(bool)*N );
		for(int i=0, j; i<n; i++){
			string st = "";
			for(j=s.V[i+1].next; j!=-1; j=s.V[j].next){
				if( j%2==0 && s.V[j+1].c>0 ) break;
			}
			if( j==-1 ){
				for(j=0; j<m; j++) st += '0';
				res.push_back( st ); continue;
			}
			for(j=0; j<m; j++){
				if( !s.find( i+1, n+j+1 ) ){ st += '0'; s.clear( i+1, n+j+1 ); continue; }
				MaxFlow::Augment( N-1, n+j+1, N, s.V, q, p, del );
				if( MaxFlow::Augment( i+1, N-1, N, s.V, q, p, del ) ){
					s.clear( i+1, n+j+1 );
					st += '0';
				}
				else{ st += '1'; s.edit( n+j+1, N-1, -1 ); }
			}
			del[i] = true;
			res.push_back( st );
		}
		delete []del;
		delete []q;
		return res;
	}
};

int main(){
	//freopen("C:\\Documents and Settings\\zgmcn\\桌面\\t.out","r",stdin);
	//freopen("C:\\Documents and Settings\\zgmcn\\桌面\\out.txt","w",stdout);
	/*int n, m, v;
	while( scanf("%d%d",&n, &m)==2 ){
		vector<int> a, u;
		for(int i=0; i<n; i++){ scanf("%d",&v); a.push_back(v); }
		for(int i=0; i<m; i++){ scanf("%d",&v); u.push_back(v); }
		ConnectingAirports::getSchedule( a, u );
	}*/
	//clock_t st = clock();
	MaxFlow::ans();
	//clock_t ed = clock();
	//printf("%.6lf\n", (double)(ed-st)/CLOCKS_PER_SEC);
	return 0;
}
