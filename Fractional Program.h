#include<stdlib.h>
#include<stdio.h>
#include<cstring>
#include<cmath>
#include<math.h>
#include<queue>
#include<set>
#include<vector>
#include<algorithm>
using namespace std;


struct Link{
	int u, v, next; double c, f; // // （u,v)为边,c为容量,f备用
	void set( int U, int V, double C, int Next){ u=U; v=V; c=C; next=Next; }
	void set( const Link &s ){ set( s.u, s.v, s.c, s.next ); }
};
class List{
public:
	int end; Link *V;
	List( int size = 1024 ){ V = new Link[size]; }
	~List(){ delete []V; }
	void init( int n ){ end=(n&1 ? n+1:n); for(int i=0; i<end; i++) V[i].next=-1; }
	void insert( int u, int v, double c1, double c2=0 ){
		V[end].set( u, v, c1, V[u].next );
		V[u].next = end++;
		V[end].set( v, u, c2, V[v].next );
		V[v].next = end++;
	}
	void resume( int n ){
		for(int i=n; i<end; i+=2){ V[i].c += V[i+1].c; V[i+1].c = 0; }
	}
	void edit( int u, int v, double c ){
		for(int i=V[u].next; i!=-1; i=V[i].next){
			if( i%2==0 && V[i].v==v ){ V[i].c = c; V[i+1].c = 0; break; }
		}
	}
};

const double eps = 1e-6;

class MaxFlow{
	static bool bfs( int s, int t, int n, bool *vis, int *q, int *h, Link *V ){
		memset( vis, 0, sizeof(bool)*n );
		for(int i=0; i<n; i++) h[i] = n;
		int head = 0, tail = 0, u, v;
		for(h[s]=0, q[tail++]=s, vis[s]=true; head!=tail; ){
			u = q[head++];
			head = (head==n ? 0:head);
			for(int i=V[u].next; i!=-1; i=V[i].next){
				v = V[i].v;
				if( V[i].c<=0 || h[v]<=h[u]+1 ) continue;
				h[v] = h[u]+1;
				if( !vis[v] ){ q[tail++]=v; tail=(tail==n?0:tail); vis[v]=true; }
			}
			vis[u] = false;
		}
		return (h[t]!=n);
	}
	static void dfs( int s, int t, int *q, int *h, Link *V, double &flow ){
		q[0] = s; V[q[0]].v = s;
		for(int i=s, j, k, tail=0; tail>=0; ){
			if( i == t ){
				double r = 1e20;
				for(i=1; i<=tail; i++) if( r>V[q[i]].c ){ r = V[q[i]].c; k = i; }
				for(i=1; i<=tail; i++){ V[q[i]].c -= r; V[q[i]^1].c += r; }
				tail = k-1;
				i = V[q[tail]].v;
				flow += r;
			}
			for(j=V[i].next; j!=-1; j=V[j].next){
				if( V[j].c>0 && h[i]+1==h[V[j].v] ){ q[++tail]=j; i=V[j].v; break; }
			}
			if( j!=-1 ) continue;
			if( tail == 0 ) break;
			h[ V[q[tail]].v ] = -1;
			i = V[ q[--tail] ].v;
		}
	}
public:
	static double Dinic( int s, int t, int n, Link *V ){
		double flow = 0;
		bool *vis = new bool[n];
		int *q = new int[2*n];
		while( bfs( s, t, n, vis, q, q+n, V ) ){
			dfs( s, t, q, q+n, V, flow );
		}
		delete []vis;
		delete []q;
		return flow;
	}
	/*static void ans(){ // zoj 2676 Network Wars  最小化 费用/边
		const int N = 108;
		int n, m, id[N][N]; bool vs[N], vt[N];
		List s( 1000 );
		while( scanf("%d%d",&n,&m)==2 ){
			s.init( (n+1)/2*2 );
			double l = 0, r = 0, mid;
			for(int i=0,u,v,w; i<m; i++){
				scanf("%d%d%d",&u,&v,&w);
				if( r<w ) r = w;
				s.V[s.end].f = s.V[s.end+1].f = w;
				s.insert( u-1, v-1, w, w );
				id[u-1][v-1] = id[v-1][u-1] = i+1;
			}
			while( l+eps<r ){
				mid = (l+r)/2;
				double flow = 0;
				for(int i=(n+1)/2*2; i<s.end; i+=2){
					s.V[i].c = s.V[i+1].c = s.V[i].f-mid;
					if( s.V[i].c<eps ){ flow += s.V[i].c; s.V[i].c=s.V[i+1].c=0; }
				}
				//printf("%.4f %.4f %.4f\n",l,r,flow);
				flow += Dinic( 0, n-1, n, s.V );
				//printf("%.4f %.4f %.4f\n",l,r,flow);
				if( fabs(flow)<eps ) break;
				if( flow>0 ) l = mid;
				else r = mid;
			}
			//printf("%.4f\n", mid);
			queue<int> q;
			memset( vs, false, sizeof(bool)*N );
			for(q.push(0),vs[0]=vs[n-1]=true; !q.empty(); q.pop() ){
				for(int i=s.V[q.front()].next; i!=-1; i=s.V[i].next){
					int v = s.V[i].v;
					if( !vs[v] && s.V[i].c>eps ){ q.push(v); vs[v]=true; }
				}
			}
			memset( vt, false, sizeof(bool)*N );
			for(q.push(n-1),vt[n-1]=true; !q.empty(); q.pop() ){
				for(int i=s.V[q.front()].next; i!=-1; i=s.V[i].next){
					int v = s.V[i].v;
					if( !vs[v] && !vt[v] && s.V[i^1].c<s.V[i].c+eps ){ q.push(v); vt[v]=true; }
				}
			}
			vector<int> res;
			for(int i=0; i<n-1; i++){
				for(int j=s.V[i].next; j!=-1; j=s.V[j].next){
					int v = s.V[j].v;
					if( s.V[j].f-mid<-eps || vs[i]&&vt[v] )
						res.push_back( id[i][v] );
				}
			}
			//if( res.size()==0 ){ printf("0\n\n"); continue; }
			sort( res.begin(), res.end() );
			int num = 1;
			for(int i=1; i<res.size(); i++) if( res[i]!=res[i-1] ) num++;
			printf("%d\n%d",num,res[0]);
			for(int i=1; i<res.size(); i++)
				if( res[i]!=res[i-1] ) printf(" %d",res[i]);
			printf("\n\n");
		}
	}*/
	static void ans(){ // poj 3155 Hard Life  最优密度子图：边数/顶点
		int n, m, d[108]; bool vis[108];
		List s( 6000 );
		while( scanf("%d%d",&n,&m)==2 ){
			if( m==0 ){ printf("1\n1\n\n"); continue; }
			int nn = (n%2 ? n+3:n+2);
			s.init( nn );
			memset( d, 0, sizeof(int)*108 );
			for(int i=0,u,v; i<m; i++){
				scanf("%d%d",&u,&v);
				d[u]++; d[v]++;
				s.insert( u, v, 1.0 );
				s.insert( v, u, 1.0 );
			}
			for(int i=0; i<n; i++) s.insert( 0, i+1, m );
			for(int i=0; i<n; i++) s.insert( i+1, n+1, 0 );
			double l = 0, r = n, mid = 0;
			while( l+1e-8<r ){
				mid = (l+r)/2;
				s.resume( nn );
				for(int i=s.V[n+1].next;  i!=-1; i=s.V[i].next){
					if( i%2==0 ) continue;
					s.V[i-1].c = m+2*mid-d[s.V[i].v];
					s.V[i].c = 0;
				}
				double flow = Dinic( 0, n+1, n+2, s.V );
				if( m*n-flow<1e-3 ) r = mid;
				else l = mid;
			}

			memset( vis, 0, sizeof(bool)*(n+2) );
			int head = 0, tail = 0, num = 0;
			for(d[tail++]=0,vis[0]=vis[n+1]=1; head!=tail; ){
				for(int i=s.V[d[head++]].next; i!=-1; i=s.V[i].next){
					int v = s.V[i].v;
					if( s.V[i].c>eps && !vis[v] ){ vis[v]=1; num++; d[tail++]=v; }
				}
			}
			printf("%d\n",num);
			for(int i=1; i<=n; i++) if( vis[i] ) printf("%d\n",i);
			printf("\n");
		}
	}
};

int main(){
	MaxFlow::ans();
	return 0;
}