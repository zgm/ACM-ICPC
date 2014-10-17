#include<iostream>
using namespace std;

struct node{ int v, w; node* next; };

class FIFP{
public:
	int n, m, *h, *gap, *q, *e;
	node *pool, *pp, **adj, **current;

	FIFP( int N, int M ) : n( N ), m( M ){
		pool = new node[2 * m];
		adj = new node*[2 * n];
		h = new int[4 * n + 1];
		e = new int[n];

		current = adj + n;
		q = h + n;
		gap = q + n;
		Initial( n );
	}
	~FIFP( ){
		delete [] pool;
		delete [] adj;
		delete [] h;
		delete [] e;
	}
	void Initial( int N ){
		n = N, pp = pool;
		memset( adj, 0, n * sizeof( adj[0] ) );
	}
	void AddEdge( int a, int b, int c ){
		pp -> v = b;
		pp -> w = c;
		pp -> next = adj[a];
		adj[a] = pp++;
		pp -> v = a;
		pp -> w = 0;
		pp -> next = adj[b];
		adj[b] = pp++;
	}
	void ClearFlow( ){
		node *p, *end = pool + 2 * m;
		for ( p = pool; p < end; p += 2 ){
			p -> w += ( p + 1 ) -> w;
			( p + 1 ) -> w = 0;
		}
	}
	__int64 MaxFlow( int s, int t ){
		node *p;
		int minH, oh, u, top, rear, i, d;

		memset( e, 0, n * sizeof( e[0] ) );
		memset( h, 0, n * sizeof( h[0] ) );
		memset( gap, 0, n * sizeof( gap[0] ) );
		memcpy( current, adj, n * sizeof( adj[0] ) );
	    
		h[s] = n, gap[n] = 1, gap[0] = n - 1;
		for ( top = rear = 0, p = adj[s]; p; p = p -> next ){
			if ( ( p - pool + 1 ) & 1 ){
				( p + 1 ) -> w = p -> w;
				e[s] -= p -> w;
				if ( 0 == e[p -> v] && p -> v != s && p -> v != t ){
					q[rear++] = p -> v;
				}
				e[p -> v] += p -> w;
				p -> w = 0;
			}
		}
	    
		e[t] = e[s];
		while ( top != rear ){
			u = q[top];
			top = top == n - 1 ? 0 : top + 1;
			while ( true ){
				for ( p = current[u]; p; p = p -> next ){
					if ( p -> w > 0 && h[u] == h[p -> v] + 1 ){
						d = e[u] > p -> w ? p -> w : e[u];
						e[u] -= d;
						p -> w -= d;
						( pool + ( ( p - pool ) ^ 1 ) ) -> w += d;
	                    
						if ( 0 == e[p -> v] ){
							q[rear] = p -> v;
							rear = rear == n - 1 ? 0 : rear + 1;
						}
						e[p -> v] += d;
						if ( 0 == e[u] ) break;
					}
				}
				if ( p ){
					current[u] = ( p -> w == 0 ) ? p -> next : p;    
					break;
				}
				for ( p = adj[u], minH = INT_MAX; p; p = p -> next ){
					if ( p -> w > 0 && minH > h[p -> v] ){
						minH = h[p -> v];
						current[u] = p;
					}
				}
				oh = h[u], h[u] = ++minH;
				++gap[minH], --gap[oh];
	            
				if ( gap[oh] == 0 && oh < n ){
					for ( i = 0; i < n; i++ ){
						if ( i != s && h[i] > oh && h[i] <= n ){
							--gap[h[i]];
							h[i] = n + 1;
						}
					}
				}
			}
		}
		__int64 flow = 0;
		node *tp;
		for( p=adj[s]; p; p=p->next){ tp = opt( p ); flow += tp->w; }
		printf("%d\n",flow);
		return flow;
	}

	node* opt( node *p ){
		return ( ((p-pool)&1) ? (p-1):(p+1) );
	}

	int bfs( int u, int s, int t ){
		bool *vis = new bool[n];
		for(int i=0; i<n; i++) vis[i] = false;
		vis[u] = vis[t] = true;
		int head = 0, tail = 0, res = 0;
		node *p;
		for(p=adj[u]; p; p=p->next){
			if( ((p-pool)&1) || p->w == 0 ) continue;
			res++;
			vis[p->v] = true;
			q[tail++] = p->v;
		}
		while( head < tail ){
			int k = q[head++];
			for(p=adj[k]; p; p=p->next ){
				int r = p->v;
				if( vis[r] || (((p-pool)&1) && p->w<=0 ) ) continue;
				res++; vis[r] = true;
				q[tail++] = r;
			}
		}
		delete []vis;
		return res;
	}
};

int main(){
	const int INF = 1<<20;
	int n, m, a[108], b[108];
	while( 2==scanf("%d%d",&n,&m) ){
		FIFP s( 2*n+2, 2*n+m );
		for(int i=0; i<n; i++){
			scanf("%d",&a[i]);
			s.AddEdge( i+1+n, 2*n+1, a[i] );
		}
		for(int i=0; i<n; i++){
			scanf("%d",&b[i]);
			s.AddEdge( 0, i+1, b[i] );
		}
		int u, v;
		for(int i=0; i<m; i++){
			scanf("%d%d",&u,&v);
			s.AddEdge( u, v+n, INF );
		}
		s.MaxFlow( 0, 2*n+1 );
	}
	return 0;
}

/*int main(){ // poj 2987 Firing
	int n, m;
	while( scanf("%d%d",&n,&m)==2 ){
		int u, v, w;
		FIFP calc( n + 2, n + m );
		n += 2;
		__int64 res = 0;
		for(int i=1; i<n-1; i++){
			scanf("%d",&v);
			if( v > 0 ){
				res += v;
				calc.AddEdge( 0, i, v );
			}
			else if( v < 0 ){
				calc.AddEdge( i, n-1, -v );
			}
		}
		const int INF = 1<<30;
		for(int i=0; i<m; i++){
			scanf("%d%d",&v,&u);
			calc.AddEdge( v, u, INF );
		}
		__int64 flow = calc.MaxFlow( 0, n-1 );
		printf("%d %I64d\n",calc.bfs( 0, 0, n-1 ), res-flow);
	}
	return 0;
}*/

/*int main(){ // poj 1459 Power Network
	int n, np, nc, m; char ch[100];
	while( scanf("%d%d%d%d",&n,&np,&nc,&m)==4 ){
		n += 2;
		FIFP calc( n, n+m+np+nc+4 );
		int u, v, w, j;
		for(int i=0; i<m; i++){
			//scanf("\n(%d,%d)%d",&u,&v,&w);
			scanf("%s",ch);
			for(u=0, j=1; ch[j]!=','; j++) u = u*10+ch[j]-'0';
			for(v=0, ++j; ch[j]!=')'; j++) v = v*10+ch[j]-'0';
			for(w=0, ++j; ch[j]!='\0'; j++) w = w*10+ch[j]-'0';
			calc.AddEdge( u+1, v+1, w );
		}
		for(int i=0; i<np; i++){
			//scanf("\n(%d)%d",&u,&w);
			scanf("%s",ch);
			for(u=0, j=1; ch[j]!=')'; j++) u = u*10+ch[j]-'0';
			for(w=0, ++j; ch[j]!='\0'; j++) w = w*10+ch[j]-'0';
			calc.AddEdge( 0, u+1, w );
		}
		for(int i=0; i<nc; i++){
			//scanf("\n(%d)%d",&u,&w);
			scanf("%s",ch);
			for(u=0, j=1; ch[j]!=')'; j++) u = u*10+ch[j]-'0';
			for(w=0, ++j; ch[j]!='\0'; j++) w = w*10+ch[j]-'0';
			calc.AddEdge( u+1, n-1, w );
		}
		printf("%d\n",calc.MaxFlow( 0, n-1 ) );
	}
	return 0;
}*/