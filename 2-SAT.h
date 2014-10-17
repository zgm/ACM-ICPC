#include<iostream>
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
	void insert( int u, int v, Link *E = NULL ){
		if( E != NULL ){ E[end].set( u, E[v].next ); E[v].next = end; }
		V[end].set( v, V[u].next );
		V[u].next = end++;
	}
};

class Kosaraju{
public:
	static void dfsa( int *order, int &time, bool *vis, Link *V, int v ){
		vis[v] = true;
		for(int i=V[v].next; i!=-1; i=V[i].next)
			if( !vis[V[i].v] ) dfsa( order, time, vis, V, V[i].v );
		order[time++] = v;
	}
	static void dfst( int *id, int cnt, bool *vis, Link *E, int v ){
		vis[v] = true; id[v] = cnt;
		for(int i=E[v].next; i!=-1; i=E[i].next)
			if( !vis[E[i].v] ) dfst( id, cnt, vis, E, E[i].v );
	}
	static int kosaraju( int n, Link *V, Link *E, int *id ){
		bool *vis = new bool[n];
		int time = 0, cnt = 0, *order = new int[n];
		memset( vis, false, sizeof(bool)*n );
		for(int i=0; i<n; i++)
			if( !vis[i] ) dfsa( order, time, vis, V, i );
		memset( vis, false, sizeof(bool)*n );
		for(int i=n-1; i>=0; i--)
			if( !vis[order[i]] ){ dfst( id, cnt, vis, E, order[i] ); cnt++; }
        delete []vis;
		delete []order;
		return cnt;
	}
	static bool TwoSat( int n, int *id, int *pair = NULL ){
		for(int i=0; i<n; i++){
			int u = i+n/2;
			if( pair != NULL ) u = pair[i];
			else if( i >= n/2 ) break;
			if( id[i] == id[u] ) return false;
		}
		return true;
	}
	static void ShrinkPoint( Link *V, List &t, int n, int cnt, int *id ){
		t.init( cnt );
		for(int i=0; i<n; i++){
			for(int j=V[i].next; j!=-1; j=V[j].next)
				if( id[i] != id[V[j].v] ) t.insert( id[i], id[V[j].v] );
		}
	}
	static void topological_sort( Link *V, int n, int *order ){
		bool *vis = new bool[n];
		memset( vis, 0, sizeof(bool)*n );
		for(int i=0, time=0; i<n; i++)
			if( !vis[i] ) dfsa( order, time, vis, V, i );
		delete []vis;
	}

	struct Node{ int s, t, l; };
	static bool Intersect( int l1, int r1, int l2, int r2 ){ return (l2<r1 && r2>l1 ); }
	static void ans(){ // poj 3683 Priest John's Busiest Day
		const int N = 1008;
		Node p[N];
		int n, id[2*N], order[2*N];
		bool vis[2*N];
		List s( 2*N+4*N*N ), t( 2*N+4*N*N ), tmp( 2*N+4*N*N );
		while( scanf("%d",&n)!=EOF ){
			s.init( 2*n );
			t.init( 2*n );
			for(int i=0, j, k; i<n; i++){
				scanf("%d:%d %d:%d %d",&j,&p[i].s,&k,&p[i].t, &p[i].l );
				p[i].s += 60*j; p[i].t += 60*k;
			}
			for(int i=0; i<n; i++){
				for(int j=i+1; j<n; j++){
					bool ok = true;
					if( !Intersect( p[i].s, p[i].s+p[i].l, p[j].s, p[j].s+p[j].l ) ){
						if( Intersect( p[i].s, p[i].s+p[i].l, p[j].t-p[j].l, p[j].t ) ){
							s.insert( i, j, t.V ); s.insert( j+n, i+n, t.V );
							ok = false;
						}
					}
					else{
						s.insert( i, j+n, t.V ); s.insert( j, i+n, t.V );
						if( ok && Intersect( p[i].s, p[i].s+p[i].l, p[j].t-p[j].l, p[j].t ) ){
							s.insert( i, j, t.V ); s.insert( j+n, i+n, t.V );
						}
					}
					ok = true;
					if( !Intersect( p[i].t-p[i].l, p[i].t, p[j].s, p[j].s+p[j].l ) ){
						if( Intersect( p[i].t-p[i].l, p[i].t, p[j].t-p[j].l, p[j].t ) ){
							s.insert( i+n, j, t.V ); s.insert( j+n, i, t.V );
							ok = false;
						}
					}
					else{
						s.insert( i+n, j+n, t.V ); s.insert( j, i, t.V );
						if( ok && Intersect( p[i].t-p[i].l, p[i].t, p[j].t-p[j].l, p[j].t ) ){
							s.insert( i+n, j, t.V ); s.insert( j+n, i, t.V );
						}
					}
				}
			}
			int cnt = kosaraju( 2*n, s.V, t.V, id );
			if( !TwoSat( 2*n, id ) ){ printf("NO\n"); continue; }
			printf("YES\n");
			ShrinkPoint( s.V, tmp, 2*n, cnt, id );
			topological_sort( tmp.V, cnt, order );
			memset( vis, 0, sizeof(bool)*2*n );
			for(int i=0, left=n; i<cnt && left>0; i++){
				for(int j=0; j<2*n; j++){
					if( id[j] == order[i] && !vis[j] ){
						int k = (j>=n ? j-n:j+n);
						if( vis[k] ) continue;
						vis[j] = true;
						left--;
					}
				}
			}
			for(int i=0; i<n; i++){
				int st = p[i].s, ed = p[i].s+p[i].l;
				if( !vis[i] ){ st = p[i].t-p[i].l; ed = p[i].t; }
				printf("%02d:%02d %02d:%02d\n",st/60, st%60, ed/60, ed%60);
			}
		}
	}
	/*static void ans(){ // poj 2723 Get Luffy Out
		int n, m, d[1<<11][2], id[1<<11];
		while( scanf("%d%d",&n,&m)==2 && n+m!=0 ){
			List s( 2*n+2*m+4 );
			List t( 2*n+2*m+4 );

			int *pair = new int[2*n], j, k;
			for(int i=0; i<n; i++){
				scanf("%d%d",&j,&k);
				pair[j] = k;
				pair[k] = j;
			}
			for(int i=0; i<m; i++){
				scanf("%d%d",&d[i][0],&d[i][1]);
			}
			int l = 1, r = m, mid, res = 0;
			while( l <= r ){
				mid = (l+r)/2;
				
				s.init( 2*n );
				t.init( 2*n );
				for(int i=0; i<mid; i++){
					s.insert( pair[d[i][0]], d[i][1], t.V );
					s.insert( pair[d[i][1]], d[i][0], t.V );
				}
				kosaraju( 2*n, s.V, t.V, id );
				if( TwoSat( n, id, pair ) ){ res = mid; l = mid+1; }
				else r = mid-1;
			}
			delete []pair;
			printf("%d\n",res);
		}
	}*/
	/*static void ans(){ // poj 2749 Building roads
		int n, a, b, d[508][2], end[1008][2], id[1008];
		List s( 2*500+4*100+4*1000+8*500*500+4 );
		List t( 2*500+4*100+4*1000+8*500*500+4 );
		while( scanf("%d%d%d",&n,&a,&b)==3 ){
			int x1, y1, x2, y2, mx1 = 0, mx2 = 0, j, k;
			scanf("%d%d%d%d",&x1,&y1,&x2,&y2);
			int dij = abs(x1-x2)+abs(y1-y2);
			for(int i=0; i<n; i++){
				scanf("%d%d",&j,&k);
				d[i][0] = abs( j-x1 ) + abs( k-y1 );
				d[i][1] = abs( j-x2 ) + abs( k-y2 );
				mx1 = max( mx1, d[i][0] );
				mx2 = max( mx2, d[i][1] );
			}
			s.init( 2*n ); t.init( 2*n );
			for(int i=0; i<a; i++){
				scanf("%d%d",&j,&k);
				j--; k--;
				s.insert( j, k+n, t.V ); s.insert( k, j+n, t.V );
				s.insert( j+n, k, t.V ); s.insert( k+n, j, t.V );
			}
			for(int i=0; i<b; i++){
				scanf("%d%d",&j,&k);
				j--; k--;
				s.insert( j, k, t.V ); s.insert( k, j, t.V );
				s.insert( j+n, k+n, t.V ); s.insert( k+n, j+n, t.V );
			}

			kosaraju( 2*n, s.V, t.V, id );
			if( !TwoSat( 2*n, id ) ){ printf("-1\n"); continue; }

			int l = max( mx1, mx2), r = max( max( mx1*2, mx2*2 ), mx1+mx2+dij ), mid, res = 0;
			int old_end = s.end;
			for(int i=0; i<2*n; i++) end[i][0] = s.V[i].next;
			for(int i=0; i<2*n; i++) end[i][1] = t.V[i].next;
			while( l <= r ){
				mid = (l+r)/2;
				s.end = old_end;
				for(int i=0; i<2*n; i++) s.V[i].next = end[i][0];
				for(int i=0; i<2*n; i++) t.V[i].next = end[i][1];
				for(int i=0; i<n; i++){
					for(int j=i+1; j<n; j++){
						if( d[i][0]+d[j][0] > mid ){ s.insert( i, j+n, t.V );  s.insert( j, i+n, t.V ); }
						if( d[i][1]+d[j][1] > mid ){ s.insert( i+n, j, t.V ); s.insert( j+n, i, t.V ); }
						if( d[i][0]+d[j][1]+dij > mid ){ s.insert( i, j, t.V ); s.insert( j+n, i+n, t.V ); }
						if( d[i][1]+d[j][0]+dij > mid ){ s.insert( i+n, j+n, t.V ); s.insert( j, i, t.V ); }
					}
				}
				kosaraju( 2*n, s.V, t.V, id );
				if( TwoSat( 2*n, id ) ){ res = mid; r = mid-1; }
				else l = mid+1;
			}
			printf("%d\n",res);
		}
	}*/
};

int main(){
    Kosaraju::ans();
	return 0;
}