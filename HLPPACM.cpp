#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <limits.h>
#define INF 0x7fffffffffffffffLL

template<typename WeightType>
class GraphNode
{
public:
	int v;
	WeightType w;
	GraphNode* next;
};

template<typename FlowType>
class HLPPACM
{
public:

	typedef GraphNode<FlowType> node;

	int n, m, *h, *head, *pre, *next, *gap;
	node *pool, *pp, **adj, **current;
	FlowType *e;

	HLPPACM( int N, int M ) : n( N ), m( M )
	{
		pool = new node[2 * m];
		adj = new node*[2 * n];
		h = new int[7 * n];
		e = new FlowType[n];

		current = adj + n;
		head = h + n;
		gap = head + 2 * n;
		pre = gap + 2 * n;
		next = pre + n;

		Initial( n );
	}

	~HLPPACM( void )
	{
		delete [] pool;
		delete [] adj;
		delete [] h;
		delete [] e;
	}

	void Initial( int N )
	{
		n = N, pp = pool;
		memset( adj, 0, n * sizeof( adj[0] ) );
	}

	void AddEdge( int a, int b, FlowType c )
	{
		pp -> v = b;
		pp -> w = c;
		pp -> next = adj[a];
		adj[a] = pp++;
		pp -> v = a;
		pp -> w = 0;
		pp -> next = adj[b];
		adj[b] = pp++;
	}

	void ClearFlow( void )
	{
		node *p, *end = pool + 2 * m;
		for ( p = pool; p < end; p += 2 )
		{
			p -> w += ( p + 1 ) -> w;
			( p + 1 ) -> w = 0;
		}
	}

	void ListInsert( int v, int h )
	{
		pre[v] = -1, next[v] = head[h];
		if ( next[v] != -1 ) pre[next[v]] = v;
		head[h] = v;
	}

	FlowType MaxFlow( int s, int t )
	{
		node *p;
		int i, minH, u, oh, maxH;
		FlowType d;
		
		for ( i = 0; i < 2 * n; i++ ) head[i] = -1;
 		memset( e, 0, n * sizeof( e[0] ) );
		memset( h, 0, n * sizeof( h[0] ) );
		memset( gap, 0, n * sizeof( gap[0] ) );
		memcpy( current, adj, n * sizeof( adj[0] ) );

		h[s] = n, maxH = 0, gap[0] = n - 1, gap[n] = 1;
		for ( p = adj[s]; p; p = p -> next )
		{
			if ( ( p - pool + 1 ) & 1 )
			{
				( p + 1 ) -> w = p -> w;
				e[s] -= p -> w;
	            
				if ( 0 == e[p -> v] && p -> v != s && p -> v != t )
				{
					ListInsert( p -> v, 0 );
				}
	            
				e[p -> v] += p -> w;
				p -> w = 0;
			}
		}
	    
		if ( -1 == head[0] ) maxH = -1;
		else e[t] = e[s];

		while ( maxH >= 0 )
		{
			u = head[maxH];
			head[maxH] = next[u];
			if ( next[u] != -1 ) pre[next[u]] = -1;

			while ( true )
			{
				for ( p = current[u], minH = h[u] - 1; p; p = p -> next )
				{
					if ( p -> w > 0 && h[p -> v] == minH )
					{
						d = e[u] > p -> w ? p -> w : e[u];
						e[u] -= d, p -> w -= d;
						( pool + ( ( p - pool ) ^ 1 ) ) -> w += d;
	                    
						if ( 0 == e[p -> v] )
						{
							ListInsert( p -> v, h[p -> v] );
						}
	                    
						e[p -> v] += d;
	                    
						if ( 0 == e[u] ) break;
					}
				}
	            
				if ( p )
				{
					current[u] = ( p -> w == 0 ) ? p -> next : p;
					break;
				}

				for ( p = adj[u], minH = INT_MAX; p; p = p -> next )
				{
					if ( p -> w > 0 && minH > h[p -> v] )
					{
						minH = h[p -> v];
						current[u] = p;
					}
				}
	            
				oh = h[u], h[u] = ++minH;
				++gap[minH], --gap[oh];
				if ( maxH < minH ) maxH = minH;
	            
				if ( gap[oh] == 0 && oh < n )
				{
					for ( i = 0; i < n; i++ )
					{
						if ( i != s && h[i] > oh && h[i] <= n )
						{
							--gap[h[i]];
							h[i] = n + 1;
						}
					}
				}
			}
			for ( ; maxH >= 0 && head[maxH] == -1; maxH-- ) {}
		}
	                                   
		return -e[s];
	}

	int GetMinCut( int s )
    {
		node *p;
		int i, res = 0;
		for ( h[s] = i = 0; i < n; i++ )
		{
            if ( h[i] >= n ) ++res;
        }
        h[s] = n;
		return res;
    }
};

typedef __int64 FlowType;

int main( void )
{
    int n, m, i, a, b;
    FlowType all, mul, res;
    scanf( "%d %d\n", &n, &m );
    mul = 2 * ( n + 2 ) + 2;
    
    HLPPACM<FlowType> calc( n + 2, n + m );
    for ( i = 0, all = 0; i < n; ++i )
    {
        scanf( "%d", &a );
        if ( a > 0 )
        {
            all += a;
            calc.AddEdge( n, i, ( FlowType )a * mul - 1 );
        }
        else calc.AddEdge( i, n + 1, ( FlowType )( -a ) * mul + 1 );
    }
    for ( i = 0; i < m; ++i )
    {
        scanf( "%d%d", &a, &b );
        calc.AddEdge( a - 1, b - 1, INF );
    }
    
    n += 2;
    res = calc.MaxFlow( n - 2, n - 1 );
    res = res % mul <= n + 1 ? res / mul : res / mul + 1;
    printf( "%d %I64d\n", calc.GetMinCut( n - 2 ), all - res );
    return 0;
}