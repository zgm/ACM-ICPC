#include<iostream>
#include<math.h>
#include<cmath>
#include<algorithm>
using namespace std;

struct Link{
	int v, w, next;
	void set( int V=-1, int W=-1, int Next=-1){ v=V; w=W; next=Next; }
};
class List{
public:
	int end; Link *V;
	List( int size = 1024 ){ V = new Link[size]; }
	~List(){ delete []V; }
	void init( int n ){ end = n; for(int i=0; i<n; i++) V[i].next = -1; }
	void insert( int u, int v, int w ){
		V[end].set( v, w, V[u].next );
		V[u].next = end++;
	}
};

class TreeDP{
	struct Node{ int l, r, n; double *a; };
public:
	static int value( int n, int *a, int k ){
		int s = 0;
		for(int i=n-1; i>=0; i--){
			if( k>=(1<<i) ){ s += a[n-1-i]; k -= (1<<i); }
		}
		return s;
	}
	static void DFS( int v, Node *p, int m, int *a, int *f, int *s, int *c ){
		p[v].a = new double[1<<m];
		int l = p[v].l, r = p[v].r;
		if( l != -1 ){
			DFS( l, p, m, a, f, s, c );
			DFS( r, p, m, a, f, s, c );
			p[v].n = p[l].n + p[r].n;
			memset( f, 0, sizeof(int)*m );
			for(int i=0; i<p[v].n; i++) f[m-1-i] = 1;
			do{
				int k = 0;
				for(int i=0, j=0; i<m; i++){
					if( f[i] ){ k += (1<<(m-1-i)); c[j++] = (1<<(m-1-i)); }
				}
				p[v].a[k] = 1e30;
				int s1 = value( m, a, k );

				for(int i=0; i<p[v].n; i++) s[i] = 0;
				for(int i=0; i<p[l].n; i++) s[p[v].n-1-i] = 1;
				do{
					int d = 0;
					for(int i=0; i<p[v].n; i++) if( s[i] ) d += c[i];
					int s2 = value( m, a, d );
					p[v].a[k] = min( p[v].a[k], p[l].a[d]+p[r].a[k-d]+fabs((double)s2/s1-0.5)*1000 );
				}while( next_permutation( s, s+p[v].n ) );
			}while( next_permutation( f, f+m ) );
		}
		else{
			p[v].n = 1;
			for(int i=(1<<(m-1)); i>0; i>>=1 ) p[v].a[i] = 0;
		}
	}
	/*static void ans(){ // poj 3719 Art of Balance
		int t, n, m, a[100], f[16], s[16], c[16];
		scanf("%d",&t);
		Node p[300];
		while( t-- ){
			scanf("%d",&n);
			for(int i=1; i<=n; i++){
				scanf("%d%d", &p[i].l, &p[i].r);
			}
			scanf("%d",&m);
			for(int i=0; i<m; i++) scanf("%d",&a[i]);
			DFS( 1, p, m, a, f, s, c );
			printf("%.3lf\n", p[1].a[(1<<m)-1] );
		}
	}*/
	static void TreeDown( int u, Link *V, int *p, int *down ){
		down[u] = 0;
		for(int i=V[u].next; i!=-1; i=V[i].next){
			if( V[i].v == p[u] ) continue;
			p[V[i].v] = u;
			TreeDown( V[i].v, V, p, down );
			down[u] = max( down[u], down[V[i].v]+V[i].w );
		}
	}
	static void TreeUp( int u, Link *V, int *p, int *down, int *up ){
		bool find = false;
		int max2 = 0;
		for(int i=V[u].next; i!=-1; i=V[i].next){
			if( V[i].v == p[u] ) continue;
			if( down[u] == down[V[i].v]+V[i].w ){
				if( !find ) find = true;
				else{ max2 = down[u]; break; }
			}
			else max2 = max( max2, down[V[i].v]+V[i].w );
		}
		for(int i=V[u].next; i!=-1; i=V[i].next){
			if( V[i].v == p[u] ) continue;
			if( down[u] == down[V[i].v]+V[i].w ) up[V[i].v] = max( up[u], max2 )+V[i].w;
			else up[V[i].v] = max( up[u], down[u] )+V[i].w;
			TreeUp( V[i].v, V, p, down, up );
		}
	}
	static void ans(){ // poj 3162 Walking Race
		const int N = 1000008;
		int n, m, v, w, *p = new int[N], *up = new int[N], *down = new int[N];
		List s( 4000000 );
		while( scanf("%d%d",&n,&m)==2 ){
			s.init( n );
			for(int i=1; i<n; i++){
				scanf("%d%d",&v,&w);
				s.insert( i, v-1, w );
				s.insert( v-1, i, w );
			}
			p[0] = -1;
			TreeDown( 0, s.V, p, down );
			up[0] = 0;
			TreeUp( 0, s.V, p, down, up );
			for(int i=0; i<n; i++) p[i] = max( up[i], down[i] );
			int h1 = 0, t1 = 0, h2 = 0, t2 = 0;
			int res = 0;
			for(int i=0, k=0; i<n; i++){
				while( t1>h1 && p[up[t1-1]]>=p[i] ) t1--;
				if( t1==h1 || p[up[t1-1]]<p[i] ) up[t1++] = i;
				while( t2>h2 && p[down[t2-1]]<=p[i] ) t2--;
				if( t2==h2 || p[down[t2-1]]>p[i] ) down[t2++] = i;
				while( p[down[h2]]-p[up[h1]]>m ){
					k = min( down[h2], up[h1] )+1;
					if( down[h2] < up[h1] ) h2++;
					else h1++;
				}
				res = max( res, i-k+1 );
			}
			printf("%d\n", res );
		}
		delete []up;
		delete []down;
		delete []p;
	}
};

class DP{
public:
	/*static void ans(){ // 2008 beijing  Minimal Ratio Tree
		const int N = 15, INF = (1<<26);
		int n, m, a[N], b[N][N], d[1<<N], w[1<<N], r[N];
		while( scanf("%d%d",&n,&m)==2 && n+m ){
			for(int i=0; i<n; i++) scanf("%d",&a[i]);
			for(int i=0; i<n; i++){
				for(int j=0; j<n; j++) scanf("%d",&b[i][j]);
			}
			d[0] = w[0] = 0;
			for(int i=1, ed=(1<<n)-(1<<(n-m))+1; i<=ed; i++){
				w[i] = INF;
				int rn = 0;
				for(int j=0; j<n; j++) if( i&(1<<j) ) r[rn++] = j;
				for(int j=0; j<rn; j++){
					d[i] = d[i-(1<<r[j])]+a[r[j]];
					if( i==(1<<r[j]) ){ w[i] = 0; continue; }
					for(int k=0; k<rn; k++){
						if( ((i-(1<<r[j]))&(1<<r[k]))==0 ) continue;
						w[i] = min( w[i], w[i-(1<<r[j])]+b[r[k]][r[j]] );
					}
				}
			}
			int id = -1;
			for(int i=1, ed=(1<<n)-(1<<(n-m))+1; i<=ed; i++){
				int k = 0;
				for(int j=0; j<n; j++) if( i&(1<<j) ) k++;
				if( k != m ) continue;
				if( id == -1 ) id = i;
				else if( w[id]*d[i] > w[i]*d[id] ) id = i;
			}
			int t = 0;
			for(int i=0; i<n; i++){
				if( id&(1<<i) ){
					if( t ) printf(" ");
					t++;
					printf("%d",i+1);
				}
			}
			printf("\n");
		}
	}*/
	/*static void ans(){ // 2008 beijing  Parade
		const int N = 108, M = 10008;
		int n, m, k, *a = new int[N*M], *b = new int[N*M];
		int x[M], y[M], s[M], t[M], q[M];
		while( scanf("%d%d%d",&n,&m,&k)==3 && n+m+k ){
			for(int i=0; i<=n; i++){
				for(int j=0; j<m; j++) scanf("%d",&a[i*m+j]);
			}
			for(int i=0; i<=n; i++){
				for(int j=0; j<m; j++) scanf("%d",&b[i*m+j]);
			}
			memset( x, 0, sizeof(int)*(m+1) );
			for(int i=0; i<=n; i++){
				s[0] = 0;
				for(int j=1; j<=m; j++) s[j] = s[j-1]+a[i*m+j-1];
				t[0] = 0;
				for(int j=1; j<=m; j++) t[j] = t[j-1]+b[i*m+j-1];
				memcpy( y, x, sizeof(int)*(m+1) );

				for(int j=0; j<=m; j++) x[j] -= s[j];
				int q1 = 0, q2 = 0;
				for(int j=0; j<=m; j++){
					while( q1<q2 && t[j]-t[q[q1]]>k ) q1++;
					if( q1!=q2 ) y[j] = max( y[j], x[q[q1]]+s[j] );
					while( q1<q2 && x[q[q2-1]]<=x[j] ) q2--;
					q[q2++] = j;
				}

				for(int j=0; j<=m; j++) x[j] += 2*s[j];
				q1 = 0; q2 = 0;
				for(int j=m; j>=0; j--){
					while( q1<q2 && t[q[q1]]-t[j]>k ) q1++;
					if( q1!=q2 ) y[j] = max( y[j], x[q[q1]]-s[j] );
					while( q1<q2 && x[q[q2-1]]<=x[j] ) q2--;
					q[q2++] = j;
				}

				memcpy( x, y, sizeof(int)*(m+1) );
			}
			int res = -1;
			for(int j=0; j<=m; j++) res = max( res, y[j] );
			printf("%d\n", res);
		}
		delete []a;
		delete []b;
	}*/
	/*static void fastfood(){ // nkoj 1436 fast food
		const int N = 1008, K = 58;
		int t = 0, n, k, w[K][N];
		int d[N], s[N], v[K][N];
		while( scanf("%d%d",&n,&k)==2 && n+k ){
			for(int i=1; i<=n; i++) scanf("%d",&d[i]);
			//sort( d+1, d+n+1 );
			s[0] = v[0][0] = 0;
			for(int i=1; i<=n; i++) v[0][i] = -1;
			for(int i=1; i<=n; i++) s[i] = s[i-1]+d[i];
			for(int i=1; i<=n; i++){
				for(int j=1; j<=k; j++){
					int x = j, y = i+j-1;
					if( x>=y ){ v[x][y] = 0; w[x][y] = y; continue; }
					v[x][y] = -1;
					int dn = w[x][y-1], up = (x<k ? w[x+1][y]:y);
					for(int r=dn; r<=up; r++){
						int m = (r+y)/2;
						int val = s[y]-s[m]-(y-m)*d[m]+(m-r)*d[m]-s[m-1]+s[r-1];
						if( v[x-1][r-1]!=-1 && (v[x][y]==-1 || v[x][y]>v[x-1][r-1]+val) ){
							v[x][y] = v[x-1][r-1]+val;
							w[x][y] = r;
						}
					}
				}
			}
			printf("Chain %d\nTotal distance sum = %d\n", ++t,v[k][n] );
		}
	}*/
	static void fastfood(){ // ÌÚÑ¶ 2009 BÌâ
		const int N = 10008;
		int n, k, w[N];
		__int64 d[N], s[N], v[2][N];
		while( scanf("%d%d",&n,&k)==2 ){
			for(int i=1; i<=n; i++) scanf("%I64d",&d[i]);
			sort( d+1, d+n+1 );
			s[0] = 0;
			for(int i=1; i<=n; i++) s[i] = s[i-1]+d[i];

			v[1][1] = 0;
			for(int i=2; i<=n; i++) v[1][i] = v[1][i-1]+d[i]-d[(1+i)>>1];
			for(int i=2; i<=k; i++){
				int t = (i&1);
				v[t][i] = 0; w[i] = i;
				for(int j=i+1; j<=n; j++){
					v[t][j] = -1;
					int num = 0;
					for(int r=w[j-1]-1; r<=j-1; r++){
						int x = r+1, y = j, m = (r+1+j)/2;
						int val = s[y]-s[m]-(y-m)*d[m]+(m-x)*d[m]-s[m-1]+s[x-1];
						if( v[t][j]==-1 || v[t][j]>v[1-t][r]+val ){
							v[t][j] = v[1-t][r]+val;
							w[j] = r+1;
							num = 0;
						}
						else num++;
						if( num > 100 ) break;
					}
				}
			}
			printf("%I64d\n", v[k&1][n]);
		}
	}
	static void ans(){ // poj 3612  Telephone Wire
		const int INF = (1<<26);
		int n, c, h[100001], d[2][101];
		while( scanf("%d%d",&n,&c)==2 ){
			int res = INF, H = -1;
			for(int i=0; i<n; i++){
				scanf("%d",&h[i]);
				H = max( H, h[i] );
			}
			for(int i=1; i<=H; i++) d[0][i] = INF;
			for(int i=h[0]; i<=H; i++) d[0][i] = (i-h[0])*(i-h[0]);
			for(int i=1,r=1; i<n; i++,r=1-r){
				for(int j=1; j<=H; j++) d[r][j] = INF;
				for(int j=1,t=INF; j<=H; j++){
					if( d[1-r][j]!=INF ) t = min( t, d[1-r][j]-c*j );
					if( j>=h[i] && t!=INF ) d[r][j] = t+c*j+(j-h[i])*(j-h[i]);
				}
				for(int j=H,t=INF; j>=h[i]; j--){
					if( d[1-r][j]!=INF ) t = min( t, d[1-r][j]+c*j );
					if( t!=INF ) d[r][j] = min( d[r][j], t-c*j+(j-h[i])*(j-h[i]) );
				}
				if( i==n-1 ) for(int j=h[n-1]; j<=H; j++) res = min( res, d[r][j] );
			}
			printf("%d\n", res);
		}
	}
};

int main(){
	//TreeDP::ans();
	DP::ans();
	//DP::fastfood();
	return 0;
}