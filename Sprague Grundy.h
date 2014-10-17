#include<iostream>
#include<algorithm>
using namespace std;

struct Link{
	int v, next; bool vis;
	void set( int V=-1, int Next=-1 ){ v=V; next=Next; vis=false; }
};
class List{
public:
	int end; Link *V;
	List( int size = 1024 ){ V = new Link[size]; }
	~List(){ delete []V; }	
	void init( int n ){ end=n=(n&1 ? n+1:n); for(int i=0; i<n; i++) V[i].next = -1; }
	void insert( int u, int v ){
		V[end].set( v, V[u].next );
		V[u].next = end++;
	}
};

class SG{
public:
	static void Get_SG( int u, int n, Link *V, int *sg ){
		if( sg[u] != -1 ) return;
		bool *vis = new bool[n];
		memset( vis, 0, sizeof(bool)*n );
		for(int i=V[u].next; i!=-1; i=V[i].next){
			int v = V[i].v;
			Get_SG( v, n, V, sg );
			vis[sg[v]] = true;
		}
		for(int i=0; i<n; i++) if( !vis[i] ){ sg[u] = i; break; }
		delete []vis;
	}
	/*static void ans(){ // poj 2425 A Chess Game
		int n, k, sg[1008];
		List s( 1000000 );
		while( scanf("%d",&n)==1 ){
			s.init( n );
			for(int i=0; i<n; i++){
				scanf("%d",&k);
				for(int j=k; j>0; j--){
					scanf("%d",&k);
					s.insert( i, k );
				}
			}
			memset( sg, -1, sizeof(sg) );
			for(int i=0; i<n; i++) Get_SG( i, n, s.V, sg );
			while( scanf("%d",&k)==1 && k!=0 ){
				int res = 0, u;
				for(int i=0; i<k; i++){
					scanf("%d",&u);
					res ^= sg[u];
				}
				if( res == 0 ) printf("LOSE\n");
				else printf("WIN\n");
			}
		}
	}*/
	static int HaveCircle( int s, int u, Link *V ){
		for(int i=V[u].next; i!=-1; i=V[i].next){
			if( V[i].vis || V[i^1].vis ) continue;
			V[i].vis = true;
			if( V[i].v == s ) return 2;
			int len = HaveCircle( s, V[i].v, V );
			if( len != 0 ) return len+1;
			V[i].vis = false;
		}
		return 0;
	}
	static int Dfs( int u, Link *V ){
		int sg = 0;
		for(int i=V[u].next; i!=-1; i=V[i].next){
			if( V[i].vis || V[i^1].vis ) continue;
			V[i].vis = true;
			int len = HaveCircle( u, V[i].v, V );
			if( len != 0 ) sg ^= (len&1);
			else sg ^= (Dfs( V[i].v, V )+1);
		}
		return sg;
	}
	/*static void ans(){ // poj 3710 Christmas Game
		int t, n, m;
		List s( 2008 );
		while( scanf("%d",&t)==1 ){
			int u, v, res = 0;
			while( t-- ){
				scanf("%d%d",&n,&m);
				s.init( n );
				for(int i=0; i<m; i++){
					scanf("%d%d",&u,&v);
					s.insert( u-1, v-1 );
					s.insert( v-1, u-1 );
				}
				res ^= Dfs( 0, s.V );
			}
			if( res ) printf("Sally\n");
			else printf("Harry\n");
		}
	}*/
	static int nimMultiPower( int x, int y ){
		if( x<=1 ) return (x==1?y:0);
		int n;
		for(n=0; ; n++)
			if( (1<<(1<<n))<=x && x<(1<<(1<<(n+1))) ) break;
		int M = (1<<(1<<n)), P = x/M, S = y/M, T = y%M;
		int d1 = nimMultiPower( P, S );
		int d2 = nimMultiPower( P, T );
		return (M*(d1^d2))^nimMultiPower(M/2,d1);
	}
	static int nimMulti( int x, int y ){
		if( x<y ) swap( x, y );
		if( x<=1 ) return (x==1?y:0);
		int n;
		for(n=0; ; n++)
			if( (1<<(1<<n))<=x && x<(1<<(1<<(n+1))) ) break;
		int M = (1<<(1<<n)), p = x/M, q = x%M, s = y/M, t = y%M;
		int c1 = nimMulti( p, s );
		int c2 = nimMulti( p, t )^nimMulti( q, s );
		int c3 = nimMulti( q, t );
		return ((c1^c2)*M)^c3^nimMultiPower( M/2, c1 );
	}
	static void ans(){  // hdu  3404  Switch lights
		int T, n;
		scanf("%d",&T);
		while( T-- ){
			scanf("%d",&n);
			int res = 0, x, y;
			while( n-- ){
				scanf("%d%d",&x,&y);
				res ^= nimMulti( x, y );
			}
			if( res==0 ) printf("Don't waste your time.\n");
			else printf("Have a try, lxhgww.\n");
		}
	}
	/*static void ans(){  // hud  1729  Stone Game
		int cs = 0, n, s, c;
		while( scanf("%d",&n)==1 && n!=0 ){
			int SG = 0;
			while( n-- ){
				scanf("%d%d",&c,&s);
				if( s==c || s==0 ) continue;
				int sg = 0;
				while( true ){
					int t = (int)sqrt((double)c)+1;
					while( t+t*t>=c ) t--;
					if( t+1<=s ){ sg = c-s; break; }
					c = t;
				}
				SG ^= sg;
			}
			printf("Case %d:\n",++cs);
			if( SG==0 ) printf("No\n");
			else printf("Yes\n");
		}
		return 0;
	}*/
};


/*int main(){
	SG::ans();
	return 0;
}*/