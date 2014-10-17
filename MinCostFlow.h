#include<iostream>
#include<math.h>
#include<cmath>
#include<time.h>
#include<queue>
#include<stack>
#include<map>
using namespace std;
const int INF = (1<<28);

struct Link{
	int v, c, w, next;
	void set( int V=-1, int C=-1, int W=-1, int Next=-1){ v=V; c=C; w=W; next=Next; }
};
class List{
public:
	int end; Link *V;
	List( int size = 1024 ){ V = new Link[size]; }
	~List(){ delete []V; }	
	void init( int n ){ end = n; for(int i=0; i<n; i++) V[i].next = -1; }
	void insert( int u, int v, int c, int w = -1 ){
		V[end].set( v, c, w, V[u].next );
		V[u].next = end++;
		V[end].set( u, 0, -w, V[v].next );
		V[v].next = end++;
	}
};

class MaxFlow{
public:
	static int opt( int v ){ return (v^1); }
	static bool bfs( int s, int t, int n, int *q, int *h, Link *V){
		bool *vis = new bool[n];
		memset( vis, 0, sizeof(bool)*n );
		int head = 0, tail = 0; vis[s] = true;
		for(int i=0; i<n; i++) h[i] = INF;
		for(h[s]=0, q[tail++]=s; head!=tail; ){
			int u = q[head++];
			head = (head==n ? 0:head);
			for(int i=V[u].next; i!=-1; i=V[i].next){
				int v = V[i].v;
				if( V[i].c > 0 && h[v] > h[u]+1 ){
					h[v] = h[u]+1;
					if( !vis[v] ){ q[tail++] = v; tail = (tail==n ? 0:tail); vis[v] = true; }
				}
			}
			vis[u] = false;
		}
		delete []vis;
		if( h[t] == INF ) return false;
		return true;
	}
	static void dfs( int s, int t, int *q, int *h, Link *V, int &flow, int &cost){
		int tail = -1; q[++tail] = s;
		V[q[0]].v = s;
		for(int i=s, j; tail>=0; ){
			if( i == t ){
				int r = INF, w = 0, ri;
				for(i=1; i<=tail; i++) if( r > V[q[i]].c ){ r = V[q[i]].c; ri = i; }
				for(i=1; i<=tail; i++){ w += V[q[i]].w; V[q[i]].c -= r; V[opt(q[i])].c += r; }
				tail = ri-1;
				i = V[q[tail]].v;
				flow += r;
				cost += r*w;
			}
			for(j=V[i].next; j!=-1; j=V[j].next){
				if( V[j].c > 0 && h[i]+1 == h[V[j].v] ){ q[++tail] = j; i = V[j].v; break; }
			}
			if( j == -1 ){
				if( tail == 0 ) break;
				h[ V[q[tail]].v ] = -1;
				i = V[ q[--tail] ].v;
			}
		}
	}
	static int Dinic( int s, int t, int n, Link *V, int &cost ){
		int flow = 0, *q = new int[n], *h = new int[n];
		while( bfs( s, t, n, q, h, V ) ){
			dfs( s, t, q, h, V, flow, cost );
		}
		delete []q;
		delete []h;
		return flow;
	}
	static int Dinic( int s, int t, int n, Link *V ){
		int cost = -1;
		return Dinic( s, t, n, V, cost );
	}

	/*static void ans(){ // poj 1637 Sightseeing tour 混合图欧拉回路
		const int N = 208;
		int t, n, m, u, v, w, e[N], p[1008][3];
		scanf("%d",&t);
		while( t-- ){
			scanf("%d%d",&n,&m);
			List s( 4*n+2*m+4 );
			s.init( ((n+2)%2 ? (n+3):(n+2)) );
			int i, flow = 0;
			for(i=0; i<n; i++) e[i] = 0;
			for(i=0; i<m; i++){
				scanf("%d%d%d",&p[i][0],&p[i][1],&p[i][2]);
				e[p[i][0]-1]++; e[p[i][1]-1]--;
			}
			for(i=0; i<n; i++) if( abs(e[i])%2 ) break;
			if( i<n ){ printf("impossible\n"); continue; }
			for(i=0; i<m; i++){
				if( p[i][2] == 1 ) continue;
				s.insert( p[i][0]-1, p[i][1]-1, 1 );
			}
			for(i=0; i<n; i++){
				flow += abs( e[i]/2 );
				if( e[i] > 0 ) s.insert( n, i, e[i]/2 );
				else if( e[i] < 0 ) s.insert( i, n+1, -e[i]/2 );
			}
			if( flow/2 == Dinic( n, n+1, n+2, s.V ) ) printf("possible\n");
			else printf("impossible\n");
		}
	}*/
	/*static void ans(){ // poj 1459 Power Network
		int n, np, nc, m; char ch[100];
		while( scanf("%d%d%d%d",&n,&np,&nc,&m)==4 ){
			n += 2;
			List s( n+2*m+2*np+2*nc+4 );
			s.init( ((n%2)?(n+1):n) );
			int u, v, w, j;
			for(int i=0; i<m; i++){
				//scanf("\n(%d,%d)%d",&u,&v,&w);
				scanf("%s",ch);
				for(u=0, j=1; ch[j]!=','; j++) u = u*10+ch[j]-'0';
				for(v=0, ++j; ch[j]!=')'; j++) v = v*10+ch[j]-'0';
				for(w=0, ++j; ch[j]!='\0'; j++) w = w*10+ch[j]-'0';
				s.insert( u+1, v+1, w );
			}
			for(int i=0; i<np; i++){
				//scanf("\n(%d)%d",&u,&w);
				scanf("%s",ch);
				for(u=0, j=1; ch[j]!=')'; j++) u = u*10+ch[j]-'0';
				for(w=0, ++j; ch[j]!='\0'; j++) w = w*10+ch[j]-'0';
				s.insert( 0, u+1, w );
			}
			for(int i=0; i<nc; i++){
				//scanf("\n(%d)%d",&u,&w);
				scanf("%s",ch);
				for(u=0, j=1; ch[j]!=')'; j++) u = u*10+ch[j]-'0';
				for(w=0, ++j; ch[j]!='\0'; j++) w = w*10+ch[j]-'0';
				s.insert( u+1, n-1, w );
			}
			printf("%d\n",Dinic( 0, n-1, n, s.V ) );
		}
	}*/
	static void ans(){ // nkoj 1825 Dining
		const int N = 108;
		int n, f, d;
		List s( 4*N*N );
		while( scanf("%d%d%d",&n,&f,&d)==3 ){
			int ff, dd, v;
			s.init( ((f+2*n+d+2)%2 ?(f+2*n+d+3):(f+2*n+d+2)) );
			for(int i=0; i<f; i++) s.insert( 0, i+1, 1 );
			for(int i=0; i<n; i++) s.insert( f+1+i, f+n+1+i, 1 );
			for(int i=0; i<d; i++) s.insert( f+2*n+i+1, f+2*n+d+1, 1 );
			for(int i=0; i<n; i++){
				scanf("%d%d",&ff,&dd);
				for(int j=0; j<ff; j++){
					scanf("%d",&v);
					s.insert( v, f+i+1, 1 );
				}
				for(int j=0; j<dd; j++){
					scanf("%d",&v);
					s.insert( f+n+i+1, f+2*n+v, 1 );
				}
			}
			printf("%d\n",Dinic( 0, f+2*n+d+1, f+2*n+d+2, s.V ) );
		}
	}
};

class MinCostFlow{
public:
	static int opt( int v ){ return (v^1); }
	static bool spfa( int s, int t, int n, Link *V, int *pre, int *pos, int *d, int *q ){
		bool *vis = new bool[n]; // 有时去掉效果更好, 去掉后要把q的容量调大
		memset( vis, 0, sizeof(bool)*n );
		for(int i=0; i<n; i++){ pre[i] = -1; d[i] = INF; }
		int head = 0, tail = 0;
		for( d[s]=0, q[tail++]=s, vis[s]=1, vis[t]=1; head!=tail; ){
			int u = q[head++];
			head = (head==n ? 0:head);
			for(int i=V[u].next; i!=-1; i=V[i].next){
				int v = V[i].v;
				if( V[i].c > 0 && d[v]>d[u]+V[i].w ){
					d[v] = d[u]+V[i].w;
					pre[v] = u;
					pos[v] = i;
					if( !vis[v] ){ q[tail++] = v; tail =(tail==n ? 0:tail); vis[v] = 1; }
				}
			}
			vis[u] = 0;
		}
		delete []vis;
		if( d[t] < INF ) return true;
		return false;
	}
	static void SSP( int s, int t, int n, Link *V, int &flow, int &cost ){
		int *pre = new int[n], *pos = new int[n], *d = new int[n], *q = new int[n];
		while( spfa( s, t, n, V, pre, pos, d, q) ){
			int r = INF;
			for(int i=t; i!=s; i=pre[i] ) r = min( r, V[pos[i]].c );
			for(int i=t; i!=s; i=pre[i] ){
				V[pos[i]].c -= r;
				V[opt(pos[i])].c += r;
				cost += r*V[pos[i]].w;
			}
			flow += r;
		}
		delete []pre;
		delete []pos;
		delete []d;
		delete []q;
	}

	static bool spfa( int s, int t, int n, Link *V, List &st, int *d, int *q, int *pos ){
		bool *vis = new bool[n];
		memset( vis, 0, sizeof(bool)*n );
		for(int i=0; i<n; i++) d[i] = INF;
		int head = 0, tail = 0;
		for( d[s]=0, q[tail++]=s, vis[s]=1, vis[t]=1; head!=tail; ){
			int u = q[head++];
			head = (head==n ? 0:head);
			for(int i=V[u].next; i!=-1; i=V[i].next){
				int v = V[i].v;
				if( V[i].c > 0 && d[v]>d[u]+V[i].w ){
					d[v] = d[u]+V[i].w;
					if( !vis[v] ){ q[tail++] = v; tail = (tail==n ? 0:tail); vis[v] = 1; }
				}
			}
			vis[u] = 0;
		}
		if( d[t] >= INF ){ delete []vis; return false; }

		st.init( ( (n%2) ? (n+1):n ) );
		head = tail = 0;
		memset( vis, 0, sizeof(bool)*n );
		for( q[tail++]=t, vis[s]=1, vis[t]=1; head!=tail; ){
			int u = q[head++];
			for(int i=V[u].next; i!=-1; i=V[i].next){
				int v = V[i].v, p = opt( i );
				if( V[p].c > 0 && d[v]+V[p].w == d[u] ){
					pos[st.end] = p;
					pos[st.end+1] = i;
					st.insert( v, u, V[p].c, V[p].w );
					if( !vis[v] ){ q[tail++] = v; vis[v] = 1; }
				}
			}
		}
		delete []vis;
		return true;
	}
	static void PrimalDual( int s, int t, int n, int m, Link *V, int &flow, int &cost ){
		int *d = new int[n], *pos = new int[2*m], *q = new int[n];
		List st( 2*m );
		while( spfa( s, t, n, V, st, d, q, pos ) ){
			int ct = d[t];
			int r = MaxFlow::Dinic( s, t, n, st.V );
			flow += r;
			cost += r*ct;
			for(int i=(n%2 ? (n+1):n); i<st.end; i+=2){
				V[pos[i]].c -= st.V[i+1].c;
				V[opt(pos[i])].c += st.V[i+1].c;
			}
		}
		delete []d;
		delete []pos;
		delete []q;
	}

	static void InitFlow( int n, Link *V, int *e, int &cost ){
		for(int i=0; i<n; i++){
			for(int j=V[i].next; j!=-1; j=V[j].next){
				int v = V[j].v;
				if( j%2==0 && V[j].w<=0 ){
					cost += V[j].c*V[j].w;
					e[v] += V[j].c;
					e[i] -= V[j].c;
					V[opt(j)].c = V[j].c;
					V[j].c = 0;
				}
			}
		}
	}
	static void AdjustPotential( int n, bool *vis, int *x, int *e, Link *V, int &cost ){
		for(int i=0; i<n; i++){
			if( !vis[i] ) continue;
			for(int j=V[i].next; j!=-1; j=V[j].next){
				int v = V[j].v;
				if( !vis[v] && V[j].w-x[i]+x[v] == 0 ){
					cost += V[j].c*V[j].w;
					e[v] += V[j].c;
					e[i] -= V[j].c;
					V[opt(j)].c += V[j].c;
					V[j].c = 0;
				}
			}
		}
		int alpha = INF;
		for(int i=0; i<n; i++){
			if( !vis[i] ) continue;
			for(int j=V[i].next; j!=-1; j=V[j].next){
				int v = V[j].v;
				if( !vis[v] && V[j].c > 0 ) alpha = min( alpha, V[j].w-x[i]+x[v] );
			}
		}
		for(int i=0; i<n; i++) if( vis[i] ) x[i] += alpha;
	}
	static void AdjustFlow( int s, int j, int *pred, int *ps, int *e, Link *V, int &cost ){
		int dt = min( e[s], -e[j] );
		for(int i=j; i!=s; i=pred[i] ) dt = min( dt, V[ps[i]].c );
		for(int i=j; i!=s; i=pred[i] ){
			cost += dt*V[ps[i]].w;
			V[ps[i]].c -= dt;
			V[opt(ps[i])].c += dt;
		}
		e[s] -= dt; e[j] += dt;
	}
	static void Relaxation( int s, int t, int n, Link *V, int flow, int &cost ){
		int *x = new int[n], *e = new int[n], *pred = new int[n], *ps = new int[n];
		memset( x, 0, sizeof(int)*n );
		memset( e, 0, sizeof(int)*n );
		e[s] = flow; e[t] = -flow;
		InitFlow( n, V, e, cost );

		bool *vis = new bool[n];
		int *q = new int[n];
		while( true ){
			for( s=0; s<n && e[s]<=0; s++);
			if( s >= n ) break;
			memset( vis, 0, sizeof(bool)*n );
			vis[s] = true;
			int es = e[s], r = 0, v = 0;
			for(int i=V[s].next; i!=-1; i=V[i].next){
				if( V[i].w-x[s]+x[V[i].v] == 0 ) r += V[i].c;
			}
			if( es > r ) AdjustPotential( n, vis, x, e, V, cost );

			int head = 0, tail = 0;
			for( q[tail++]=s; head<tail; ){
				int u = q[head++], k;
				for(k=V[u].next; k!=-1; k=V[k].next){
					v = V[k].v;
					if( vis[v] || V[k].w-x[u]+x[v]!=0 || V[k].c<=0 ) continue;
					pred[v] = u; ps[v] = k;
					if( e[v] < 0 ) break;
					q[tail++] = v; vis[v] = true;
					es += e[v];
					for(int i=V[v].next; i!=-1; i=V[i].next){
						int j = V[i].v;
						if( vis[j] ){
							if( V[opt(i)].w-x[j]+x[v] == 0 ) r -= V[opt(i)].c;
						}
						else if( V[i].w-x[v]+x[j] == 0 ) r += V[i].c;
					}
					if( es > r ) break;
				}
				if( k != -1 ) break;
			}
			if( es > r ) AdjustPotential( n, vis, x, e, V, cost );
			else AdjustFlow( s, v, pred, ps, e, V, cost );
		}
		for(int i=V[s].next; i!=-1; i=V[i].next){
			if( i%2 == 0 ) flow += V[opt(i)].c;
		}
		delete []x;
		delete []e;
		delete []pred;
		delete []ps;
		delete []vis;
		delete []q;
	}

	static int FindCycle( int n, int *p, int *id ){
		memset( id, 0, sizeof(int)*n );
		for(int i=0, k=0; i<n; i++){
			if( id[i] != 0 ) continue;
			++k;
			for(int j=i; j!=-1; id[j]=k, j=p[j] ) if( id[j] == k ) return j;
		}
		return -1;
	}
	static int spfa( int s, int n, Link *V, int *d, int *p, int *ps, bool *vis ){
		for(int i=V[s].next; i!=-1; i=V[i].next){
			int v = V[i].v;
			if( V[i].c>0 && d[v]>d[s]+V[i].w ){
				d[v] = d[s]+V[i].w;
				p[v] = s; ps[v] = i;
				if( vis[v] ) return v;
				vis[v] = true;
				int res = spfa( v, n, V, d, p, ps, vis );
				if( res != -1 ) return res;
				vis[v] = false;
			}
		}
		return -1;
	}
	static int BellmanFord( int s, int n, Link *V, int *d, int *p, int *ps, bool *vis ){
		for(int i=0; i<n; i++) d[i] = 0;
		for(int i=0; i<n; i++) p[i] = -1;
		/*for(int i=0; i<=n; i++){
			bool relaxation = false;
			for(int j=0; j<n; j++){
				for(int k=V[j].next; k!=-1; k=V[k].next){
					int v = V[k].v;
					if( V[k].c>0 && d[v] > d[j]+V[k].w ){
						d[v] = d[j]+V[k].w;
						p[v] = j; ps[v] = k;
						relaxation = true;
					}
				}
			}
			if( !relaxation ) return -1;
		}
		return FindCycle( n, p, d );*/
		memset( vis, 0, sizeof(bool)*n );
		vis[s] = true;
		int res = spfa( s, n, V, d, p, ps, vis );
		if( res!=-1 ) return res;
		vis[s] = false;
		for(int i=0; i<n; i++){
			if( p[i] == -1 ){
				res = spfa( i, n, V, d, p, ps, vis );
				if( res!=-1 ) return res;
			}
		}
		return res;
	}
	static void CycleCanceling( int s, int t, int n, Link *V, int &flow, int &cost ){
		flow = MaxFlow::Dinic( s, t, n, V, cost );
		int *d = new int[n], *p = new int[n], *ps = new int[n], start;
		bool *vis = new bool[n];
		while( (start=BellmanFord( s, n, V, d, p, ps, vis))!=-1 ){
			int r = INF, w = 0, k = start;
			do{
				r = min( r, V[ps[k]].c );
				w += V[ps[k]].w;
				k = p[k];
			}while( k!=start );
			
			do{ V[ps[k]].c -= r; V[opt(ps[k])].c += r; k = p[k]; }while( k!=start );
			
			cost += r*w;
		}
		delete []d;
		delete []p;
		delete []ps;
		delete []vis;
	}

	/*static void ans(){ // poj 3686 The Windy's
		int t, n, m, w[51][51];
		scanf("%d",&t);
		List s( 800000 );
		while( t-- ){
			scanf("%d%d",&n,&m);
			for(int i=0; i<n; i++){
				for(int j=0; j<m; j++) scanf("%d",&w[i][j]);
			}
			int k = n+n*m+2;
			s.init( k%2 ? (k+1):k );
			for(int i=0; i<n; i++) s.insert( 0, i+1, 1, 0 );
			for(int i=n+1; i<=n+n*m; i++) s.insert( i, k-1, 1, 0 );
			for(int i=0; i<n; i++){
				for(int j=0; j<m; j++){
					for(int r=0; r<n; r++) s.insert( i+1, n+1+j*n+r, 1, w[i][j]*(r+1) );
				}
			}
			int flow = 0, cost = 0;
			SSP( 0, k-1, k, s.V, flow, cost );
			printf("%.6lf\n", (double)cost/n );
		}
	}*/
	/*static void ans(){ // poj 3422 Kaka's Matrix Travels
		int n, k, w;
		List s( 30000 );
		while( scanf("%d%d",&n,&k)==2 ){
			int size = 2*n*n+1;
			s.init( ( (size%2) ? (size+1):size ) );
			for(int i=0; i<n; i++){
				for(int j=0; j<n; j++){
					scanf("%d",&w);
					s.insert( i*n+j, i*n+j+n*n, 1, -w );
					s.insert( i*n+j, i*n+j+n*n, k, 0 );
					if( i < n-1 ) s.insert( i*n+j+n*n, i*n+j+n, k, 0 );
					if( j < n-1 ) s.insert( i*n+j+n*n, i*n+j+1, k, 0 );
				}
			}
			s.insert( 2*n*n-1, 2*n*n, k, 0 );
			int flow = 0, cost = 0;
			//SSP( 0, 2*n*n, 2*n*n+1, s.V, flow, cost );
			//PrimalDual( 0, 2*n*n, 2*n*n+1, s.end, s.V, flow, cost );
			CycleCanceling( 0, 2*n*n, 2*n*n+1, s.V, flow, cost ); // 超时
			//flow = k; Relaxation( 0, 2*n*n, 2*n*n+1, s.V, flow, cost );
			printf("%d\n", -cost);
		}
	}*/
	static void ans(){ // poj 3762  The Bonus Salary!
		const int N = 2008;
		int n, k, x[N], y[N], w[N];
		List st( 1000000 );
		map<int,int> mp;
		map<int,int>::iterator it;
		while( scanf("%d%d",&n,&k)==2 ){
			mp.clear();
			int nn = 0, h, m, s, u, v;
			for(int i=0; i<n; i++){
				scanf("%d:%d:%d",&h,&m,&s);
				x[i] = h*3600+m*60+s;
				if( mp.find(x[i])==mp.end() ){
					mp.insert( make_pair(x[i],nn) );
					nn++;
				}
				scanf("%d:%d:%d",&h,&m,&s);
				y[i] = h*3600+m*60+s;
				if( mp.find(y[i])==mp.end() ){
					mp.insert( make_pair(y[i],nn) );
					nn++;
				}
				scanf("%d",&w[i]);
			}
			st.init( nn%2 ? nn+3:nn+2 );
			for(int i=0; i<n; i++){
				it = mp.find( x[i] ); u = it->second;
				it = mp.find( y[i] ); v = it->second;
				st.insert( u, v, 1, -w[i] );
			}
			int ss = nn+1, tt = nn+2;
			u = -1; v = ss;
			for(it=mp.begin(); it!=mp.end(); it++){
				u = v;
				v = it->second;
				st.insert( u, v, k, 0 );
			}
			st.insert( v, tt, k, 0 );
			int flow = 0, cost = 0;
			SSP( ss, tt, nn+3, st.V, flow, cost );
			printf("%d\n",-cost);
		}
	}
	/*static void ans(){ // poj 2195 Going Home
		const int N = 128;
		int hh, ww, h[N], c[N];
		char line[N];
		List s( 2*N*N );
		while( scanf("%d%d",&hh,&ww)==2 && ww!=0 ){
			int n = 0, cn = 0, *phalanx;
			for(int i=0; i<hh; i++){
				scanf("%s",line);
				for(int j=0; j<ww; j++){
					if( line[j] == 'H' ) h[n++] = i*N+j;
					if( line[j] == 'm' ) c[cn++] = i*N+j;
				}
			}
			s.init( ((n+cn+2)%2 ? (n+cn+3):(n+cn+2)) );
			for(int i=0; i<n; i++) s.insert( 0, i+1, 1, 0 );
			for(int i=0; i<cn; i++) s.insert( n+1+i, n+cn+1, 1, 0 );
			for(int i=0; i<n; i++){
				int cr = c[i]/N, cc = c[i]%N;
				for(int j=0; j<n; j++){
					int hr = h[j]/N, hc = h[j]%N;
					s.insert( i+1, j+n+1, 1, abs(cr-hr)+abs(cc-hc) );
				}
			}
			int flow = 0, cost = 0;
			CycleCanceling( 0, n+cn+1, n+cn+2, s.V, flow, cost );
			printf("%d\n",cost );
		}
	}*/
	/*static void ans(){ // poj 2135 Farm Tour
		int n, m;
		List s( 4*10000 );
		while( scanf("%d%d",&n,&m)!=EOF && n+m!=0 ){
			s.init( (n+1)%2 ? (n+2):(n+1) );
			int u, v, w;
			s.insert( n, 0, 2, 0 );
			for(int i=0; i<m; i++){
				scanf("%d%d%d",&u,&v,&w);
				u--; v--;
				if( v != 0 ) s.insert( u, v, 1, w );
				if( v != n-1 ) s.insert( v, u, 1, w );
			}
			int flow = 0, cost = 0;
			//PrimalDual( n, n-1, n+1, s.end, s.V, flow, cost );
			CycleCanceling( n, n-1, n+1, s.V, flow, cost );
			//flow = 2; Relaxation( n, n-1, n+1, s.V, flow, cost );
			printf("%d\n", cost );
		}
	}*/
	/*static void ans(){ // poj 3068 "Shortest" pair of paths
		int n, m, t = 0;
		List s( 4*10000 );
		while( scanf("%d%d",&n,&m)!=EOF && n+m!=0 ){
			s.init( (2*n-1)%2 ? (2*n):(2*n-1) );
			int u, v, w;
			s.insert( 0, n, 2, 0 );
			for(int i=1; i<n-1; i++) s.insert( i, i+n, 1, 0 );
			for(int i=0; i<m; i++){
				scanf("%d%d%d",&u,&v,&w);
				if( u == n-1 || v == 0 ) continue;
				s.insert( u+n, v, 1, w );
			}
			int flow = 0, cost = 0;
			PrimalDual( 0, n-1, 2*n-1, s.end, s.V, flow, cost );
			printf("Instance #%d: ",++t);
			if( flow != 2 ) printf("Not possible\n");
			else printf("%d\n", cost );
		}
	}*/
	/*static void ans(){ // poj 2516 Minimum Cost
		const int N = 58, M = 58, K = 58;
		int n, m, k, nd[N][K], pd[M][K], w;
		List s( 4*N+2*N*N );
		while( scanf("%d%d%d",&n,&m,&k)!=EOF && n+m+k!=0 ){
			for(int i=0; i<n; i++){
				for(int j=0; j<k; j++) scanf("%d",&nd[i][j]);
			}
			for(int i=0; i<m; i++){
				for(int j=0; j<k; j++) scanf("%d",&pd[i][j]);
			}

			int flow, cost = 0, total;
			for(int r=0; r<k; r++){
				s.init( ((n+m+2)%2 ? (n+m+3):(n+m+2)) );
				for(int i=0; i<m; i++) s.insert( 0, i+1, pd[i][r], 0 );
				for(int i=0; i<n; i++){
					for(int j=0; j<m; j++){
						scanf("%d",&w);
						s.insert( j+1, i+m+1, INF, w );
					}
				}
				for(int i=0; i<n; i++) s.insert( i+m+1, n+m+1, nd[i][r], 0 );
				
				flow = 0; total = 0;
				PrimalDual( 0, n+m+1, n+m+2, s.end, s.V, flow, cost );
				for(int i=0; i<n; i++) total += nd[i][r];
				if( flow != total ){
					for(int i=0; i<(k-r-1)*n*m; i++) scanf("%d",&w );
					printf("-1\n");
					break;
				}
				else if( r == k-1 ) printf("%d\n", cost );
			}
		}
	}*/
};

int main(){
	clock_t st = clock();
	//MaxFlow::ans();
	MinCostFlow::ans();
	clock_t ed = clock();
	printf("%.4lf\n",(double)(ed-st)/CLOCKS_PER_SEC);
	return 0;
}