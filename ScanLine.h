#include<stdlib.h>
#include<stdio.h>
#include<cstring>

class Heap{
	struct Node{ int v; double w; };
public:
	int size, *hp; Node *h;
	bool Type; // true: MinHeap, false: MaxHeap
	Heap( int n=1024 ){ h=new Node[n+1]; hp=new int[n]; }
	~Heap(){ delete []h; delete []hp; }
	void Init( int n, bool T=true ){ size=0; Type=T; memset(hp,-1,sizeof(int)*n); }
	void Insert( int v, double w ){
		h[++size].v = v;
		h[size].w = w;
		hp[v] = size;
		Up( size );
	}
	void Swap( int u, int v ){
		hp[h[u].v] = v; hp[h[v].v] = u;
		h[0] = h[u]; h[u] = h[v]; h[v] = h[0];
	}
	void Up( int p ){
		for( ; p>1; p/=2){
            if( Type && h[p/2].w<=h[p].w ) break;
            if( !Type && h[p/2].w>=h[p].w ) break;
            Swap( p/2, p );
        }
	}
	void Down( int p ){
		for(p*=2; p<=size; p*=2){
            if( p+1<=size ){
                if( Type && h[p+1].w<h[p].w ) p++;
                if( !Type && h[p+1].w>h[p].w ) p++;
            }
            if( Type && h[p].w>=h[p/2].w ) break;
            if( !Type && h[p].w<=h[p/2].w ) break;
            Swap( p/2, p );
        }
    }
	int Delete( double &w ){
        int v = h[1].v;
		w += h[1].w; hp[v] = -2;
        Swap( 1, size-- );
        Down( 1 );
		return v;
    }
};

class Race{
public:
	static int Lowbit( int t ){ return ( t&(t^(t-1)) ); }
	static void Plus( int ps, int ad, int *a, int n ){
		while( ps < n ){
			a[ps] += ad;
			ps += Lowbit( ps );
		}
	}
	static int Sum( int ps, int *a ){
		int s = 0;
		while( ps > 0 ){
			s += a[ps];
			ps -= Lowbit( ps );
		}
		return s;
	}
	static void ans(){ // poj 2274  The Race
		const int N = 250008, M = 108;
		const double INF = 1e20, D = 1e10;
		int n, X[N], v[N], a[M], pre[N], next[N];
		Heap s( N );
		while( scanf("%d",&n)==1 ){
			memset( a, 0, sizeof(int)*M );
			int res = 0;
			for(int i=0; i<n; i++,res%=1000000){
				scanf("%d%d",&X[i],&v[i]);
				res += i-Sum( v[i], a );
				Plus( v[i], 1, a, M );
				pre[i] = i-1;
				next[i] = i+1;
			}
			printf("%d\n",res);

			s.Init( n );
			for(int i=0; i<n; i++) s.Insert( i, INF );
			for(int i=0; i<=n-2; i++){
				if( v[i]<=v[i+1] ) continue;
				double W = (double)(X[i+1]-X[i])/(v[i]-v[i+1]);
				s.h[s.hp[i]].w = D*W+X[i]+v[i]*W;
				s.Up( s.hp[i] );
			}
			for(int i=0,x,y,z,w; i<10000; i++){
				if( s.h[1].w>=INF ) break;
				y = s.h[1].v;
				printf("%d %d\n",y+1,next[y]+1);
				x = pre[y]; z = next[y]; w = next[z];
				if( x>=0 ) next[x] = z;
				pre[z] = x; next[z] = y; pre[y] = z; next[y] = w;
				if( w<n ) pre[w] = y;
				if( x>=0 ){
					s.h[s.hp[x]].w = INF;
					s.Down( s.hp[x] );
					if( v[x]>v[z] ){
						double W = (double)(X[z]-X[x])/(v[x]-v[z]);
						s.h[s.hp[x]].w = D*W+X[x]+v[x]*W;
						s.Up( s.hp[x] );
					}
				}
				s.h[s.hp[z]].w = INF;
				s.Down( s.hp[z] );
				s.h[s.hp[y]].w = INF;
				if( w<n && v[y]>v[w] ){
					double W = (double)(X[w]-X[y])/(v[y]-v[w]);
					s.h[s.hp[y]].w = D*W+X[y]+v[y]*W;
				}
				s.Down( s.hp[y] );
			}
		}
	}
};

int main(){
	Race::ans();
	return 0;
}