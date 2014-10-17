#include<iostream>
#include<algorithm>
#include<vector>
#include<math.h>
#include<cmath>
using namespace std;

class PopKart{
	struct Point{ int v, w; Point(int x=0, int y=0){ v=x; w=y; } };
	struct Node{ int l, r, w; bool vis; };
public:
	static bool cmp( Point s, Point t){ return ((s.v<t.v) || (s.v==t.v&&s.w<t.w) ); }
	static void Init( int ps, int l, int r, Node *nd, Point *p ){
		nd[ps].l = l; nd[ps].r = r; nd[ps].vis = false;
		if( r == l ){ nd[ps].w = p[l].w; return; }
		Init( ps<<1, l, (l+r)>>1, nd, p );
		Init( (ps<<1)+1, ((l+r)>>1)+1, r, nd, p );
		nd[ps].w = max( nd[ps<<1].w, nd[(ps<<1)+1].w );
	}
	static void Update( int ps, Node *nd ){
		nd[ps].vis = ( nd[(ps<<1)+1].vis && nd[ps<<1].vis );
		if( nd[ps].vis ){ nd[ps].w = -1; return; }
		int w = -1;
		if( !nd[ps<<1].vis ) w = max( w, nd[ps<<1].w );
		if( !nd[(ps<<1)+1].vis ) w = max( w, nd[(ps<<1)+1].w );
		nd[ps].w = w;
	}
	static int Find( int ps, int w, Node *nd, Point *p, vector<Point> &v ){
		int t = -1;
		if( nd[ps].l == nd[ps].r ){
			v.push_back( Point(p[nd[ps].l].v, nd[ps].w ) );
			t = nd[ps].w; nd[ps].vis = true; nd[ps].w = -1;
			return t;
		}
		if( !nd[(ps<<1)+1].vis && nd[(ps<<1)+1].w>w ) t = Find( (ps<<1)+1, w, nd, p, v );
		else if( !nd[ps<<1].vis && nd[ps<<1].w>w ) t = Find( ps<<1, w, nd, p, v );
		Update( ps, nd );
		return t;
	}
	static void ans(){ // poj 3598 PopKart
		const int N = 10008;
		int n;
		Point p[N]; Node nd[3*N];
		while( 1==scanf("%d",&n) ){
			for(int i=0; i<n; i++) scanf("%d%d",&p[i].v,&p[i].w);
			sort( p, p+n, cmp );
			Init( 1, 0, n-1, nd, p );

			vector<Point> v;
			while( nd[1].w > -1 ){
				v.clear();
				for(int j=-1; nd[1].w>j; ) j = Find( 1, j, nd, p, v );
				printf("%d:", v.size() );
				for(int j=v.size()-1; j>=0; j--) printf(" (%d,%d)",v[j].v, v[j].w );
				printf("\n");
			}
		}
	}
};

class Interval{
	struct Node{ // Priority: key > invert > (l,r)
		int l, r, key; // key: 0 not cover, 1 cover, 2 part cover
		bool invert; // false: not invert  true: invert
		//int zero[4], one[4]; // total num, max continue num, left, right, of 0 and 1
		void set( int L, int R, bool Invert ){ l = L; r = R; key = 2; invert=Invert; }
		void set( int Key ){
			key = Key; invert = false;
			/*memset( zero, 0, sizeof(int)*4 );
			memset( one, 0, sizeof(int)*4 );
			int *num = ( key ? one:zero );
			for(int i=0; i<4; i++) num[i] = r-l+1;*/
		}
	};
	static void Update( Node &p, Node &s, Node &t ){
		/*int *sz = s.invert ? s.one:s.zero, *so = s.invert ? s.zero:s.one;
		int *tz = t.invert ? t.one:t.zero, *to = t.invert ? t.zero:t.one;

		p.zero[0] = sz[0]+tz[0];
		p.zero[1] = max( sz[1], tz[1] );
		p.zero[1] = max( p.zero[1], sz[3]+tz[2] );
		p.zero[2] = sz[2]+(so[0] ? 0:tz[2]);
		p.zero[3] = tz[3]+(to[0] ? 0:sz[3]);

		p.one[0] = so[0]+to[0];
		p.one[1] = max( so[1], to[1] );
		p.one[1] = max( p.one[1], so[3]+to[2] );
		p.one[2] = so[2]+(sz[0] ? 0:to[2]);
		p.one[3] = to[3]+(tz[0] ? 0:so[3]);*/

		p.key = ( s.key==t.key ? s.key:2 );
		if( p.key!=2 ) p.set( p.key );
	}
	static void Adjust( Node &p, Node &s, Node &t ){
		if( p.key!=2 ){ s.set(p.key); t.set(p.key); }
		else if( p.invert ){
			if( s.key!=2 ) s.set( 1-s.key );
			else s.invert = !s.invert;
			if( t.key!=2 ) t.set( 1-t.key );
			else t.invert = !t.invert;
			p.invert = false;
			//for(int i=0; i<4; i++) swap( p.zero[i], p.one[i] );
		}
	}
public:
	static void Init( int w, Node *p, int l, int r ){
	//static void Init( int w, Node *p, int l, int r, int *Key ){
		p[w].set( l, r, false );
		if( l==r ){ p[w].set( 0 ); return; }
		//if( l==r ){ p[w].set( Key[r] ); return; }
		int mid = (l+r)/2;
		Init( 2*w, p, l, mid );
		Init( 2*w+1, p, mid+1, r );
		Update( p[w], p[2*w], p[2*w+1] );
	}
	static void Set( int w, Node *p, int l, int r, int key ){
		if( l<=p[w].l && p[w].r<=r ){ p[w].set( key ); return; }
		Adjust( p[w], p[2*w], p[2*w+1] );
		int mid = (p[w].l+p[w].r)/2;
		if( l<=mid ) Set( 2*w, p, l, r, key );
		if( mid<r ) Set( 2*w+1, p, l, r, key );
		Update( p[w], p[2*w], p[2*w+1] );
	}
	static void Invert( int w, Node *p, int l, int r ){
		if( l<=p[w].l && p[w].r<=r ){
			if( p[w].key!=2 ) p[w].set( 1-p[w].key );
			else p[w].invert = !p[w].invert;
			return;
		}
		Adjust( p[w], p[2*w], p[2*w+1] );
		int mid = (p[w].l+p[w].r)/2;
		if( l<=mid ) Invert( 2*w, p, l, r );
		if( mid<r ) Invert( 2*w+1, p, l, r );
		Update( p[w], p[2*w], p[2*w+1] );
	}
	/*static int Num( int w, Node *p, int l, int r ){
		if( l<=p[w].l && p[w].r<=r ) return p[w].invert ? p[w].zero[0]:p[w].one[0];
		Adjust( p[w], p[2*w], p[2*w+1] );
		int num = 0, mid = (p[w].l+p[w].r)/2;
		if( l<=mid ) num += Num( 2*w, p, l, r );
		if( mid<r ) num += Num( 2*w+1, p, l, r );
		return num;
	}
	static int ContinueNum( int w, Node *p, int l, int r ){
		if( l<=p[w].l && p[w].r<=r ) return p[w].invert ? p[w].zero[1]:p[w].one[1];
		Adjust( p[w], p[2*w], p[2*w+1] );
		int left = ( p[2*w].invert ? p[2*w].zero[3]:p[2*w].one[3] );
		int right = ( p[2*w+1].invert ? p[2*w+1].zero[2]:p[2*w+1].one[2]);
		int mid = (p[w].l+p[w].r)/2, num = min(mid-l+1,left)+min(r-mid,right);
		if( l<=mid ) num = max( num, ContinueNum(2*w,p,l,r) );
		if( mid<r ) num = max( num, ContinueNum(2*w+1,p,l,r) );
		return num;
	}
	static void ans(){  // hdu 3397  Sequence operation
		const int N = 100008;
		Node *p = new Node[6*N];
		int t, n, m, v[N], op, a, b;
		scanf("%d",&t);
		while( t-- ){
			scanf("%d%d",&n,&m);
			for(int i=0; i<n; i++) scanf("%d",&v[i]);
			Init( 1, p, 0, n-1, v );
			while( m-- ){
				scanf("%d%d%d",&op,&a,&b);
				switch( op ){
					case 0: Set( 1, p, a, b, 0 ); break;
					case 1: Set( 1, p, a, b, 1 ); break;
					case 2: Invert( 1, p, a, b ); break;
					case 3: printf("%d\n",Num(1,p,a,b)); break;
					case 4: printf("%d\n",ContinueNum(1,p,a,b)); break;
					default: break;
				}
			}
		}
		delete []p;
	}*/
	static void Show( int w, Node *p, vector<int>&vt ){
		if( p[w].key==2 ){
			Adjust( p[w], p[2*w], p[2*w+1] );
			Show( 2*w, p, vt );
			Show( 2*w+1, p, vt );
		}
		else if( p[w].key ){
			int n = vt.size();
			if( n>0 && vt[n-1]==p[w].l-1 ) vt[n-1] = p[w].r;
			else{ vt.push_back( p[w].l ); vt.push_back( p[w].r ); }
		}
	}
	static void Out( Node *p ){
		vector<int> v;
		Show( 1, p, v );
		if( v.size() == 0 ) printf("empty set");
		for(int i=0; i<v.size(); i+=2){
			if( i != 0 ) printf(" ");
			if( v[i]%2 ) printf("(%d,",v[i]/2 );
			else printf("[%d,",v[i]/2 );
			if( v[i+1]%2 ) printf("%d)",v[i+1]/2+1 );
			else printf("%d]",v[i+1]/2 );
		}
		printf("\n");
	}
	static void ans(){ // poj 3225 Help with Intervals
		const int N = 65535;
		Node *p = new Node[4*2*N];
		Init( 1, p, 0, 2*N );
		char c, d, e;
		int u, v;
		while( scanf("%c %c%d,%d%c", &c,&d,&u,&v,&e)==5 ){
			getchar();
			u *= 2; v *= 2;
			if( d == '(' ) u++;
			if( e == ')' ) v--;
			if( u > v ) continue;
			switch( c ){
				case 'U': if( u<=v ) Set( 1, p, u, v, 1 ); break;
				case 'I':
					if( u>=1 ) Set( 1, p, 0, u-1, 0 );
					Set( 1, p, v+1, 2*N, 0 );
					break;
				case 'D': if( u<=v ) Set( 1, p, u, v, 0 ); break;
				case 'S': if( u<=v ) Invert( 1, p, u, v ); break;
				case 'C':
					if( u>v ){ Set( 1, p, 0, 2*N, 0 ); continue; }
					Invert( 1, p, u, v );
					if( u>1 ) Set( 1, p, 0, u-1, 0 );
					Set( 1, p, v+1, 2*N, 0 );
					break;
				default: break;
			}
		}
		Out( p );
		delete []p;
	}
};


class Star{
	struct Point{ __int64 x, y; int b; };
	struct Node{ int l, r; __int64 v, mx; };
public:
	static bool cmp( Point s, Point t){ return ((s.x<t.x) || (s.x==t.x&&s.y<t.y)); }
	static void Init( int ps, int l, int r, Node *nd ){
		nd[ps].l = l;
		nd[ps].r = r;
		nd[ps].v = 0;
		nd[ps].mx = 0;
		if( r == l ) return;
		int mid = (l+r)/2;
		Init( ps<<1, l, mid, nd );
		Init( (ps<<1)+1, mid+1, r, nd );
	}
	static void Insert( int ps, int l, int r, int val, Node *nd ){
		if( nd[ps].l>=l && nd[ps].r<=r ) nd[ps].v += val;
		else{
			int mid = (nd[ps].l+nd[ps].r)/2;
			if( mid >= l ) Insert( ps<<1, l, r, val, nd );
			if( mid < r ) Insert( (ps<<1)+1, l, r, val, nd );
		}
		if( nd[ps].l == nd[ps].r ) nd[ps].mx = nd[ps].v;
		else nd[ps].mx = max( nd[ps<<1].mx, nd[(ps<<1)+1].mx )+nd[ps].v;
	}
	static int HalfSearch( __int64 val, int len, __int64 *index ){
		int l = 0, r = len, mid;
		while( l <= r ){
			mid = (l+r)/2;
			if( index[mid] == val ) return mid;
			if( index[mid] > val ) r = mid-1;
			else l = mid+1;
		}
	}
	static void ans(){ // poj 2482 Stars in Your Window
		const int N = 10008;
		int n, w, h;
		__int64 index[2*N];
		Point *p = new Point[N];
		Node *nd = new Node[8*N];
		while( scanf("%d%d%d",&n,&w,&h)==3 ){
			w--; h--;
			for(int i=0; i<n; i++){
				scanf("%I64d%I64d%d",&p[i].x,&p[i].y,&p[i].b);
				index[2*i] = p[i].y;
				index[2*i+1] = p[i].y+h;
			}
			sort( p, p+n, cmp );
			sort( index, index+2*n );
			int len = 0, left = 0;
			for(int i=1; i<2*n; i++) if( index[i]!=index[len] ) index[++len] = index[i];
			__int64 res = 0;
			Init( 1, 0, len, nd );
			for(int i=0; i<n; i++){
				res = max( res, nd[1].mx );
				while( p[i].x>p[left].x+w ){
					int l = HalfSearch( p[left].y, len, index );
					int r = HalfSearch( p[left].y+h, len, index );
					Insert( 1, l, r, -p[left].b, nd );
					left++;
				}
				int l = HalfSearch( p[i].y, len, index );
				int r = HalfSearch( p[i].y+h, len, index );
				Insert( 1, l, r, p[i].b, nd );
			}
			res = max( res, nd[1].mx );
			printf("%I64d\n", res );
		}
		delete []p;
		delete []nd;
	}
};

class Horizon{
	struct Point{ int l, r, h; };
public:
	static void Init( int ps, int l, int r, Point *t ){
		t[ps].l = l;
		t[ps].r = r;
		t[ps].h = 0;
		if( l+1 >= r ) return;
		Init( ps<<1, l, (l+r)/2, t );
		Init( (ps<<1)+1, (l+r)/2, r, t );
	}
	static void Insert( int ps, int l, int r, int h, Point *t ){
		if( t[ps].l>=l && t[ps].r<=r ){ t[ps].h = max( t[ps].h, h ); return; }
		int mid = (t[ps].l+t[ps].r)/2;
		if( l<mid ) Insert( ps<<1, l, r, h, t );
		if( r>mid ) Insert( (ps<<1)+1, l, r, h, t );
	}
	static __int64 Sum( int ps, int h, Point *t, int *index ){
		h = max( h, t[ps].h );
		if( t[ps].l+1==t[ps].r ) return (__int64)h*(index[t[ps].r]-index[t[ps].l]);
		__int64 s = 0;
		s += Sum( ps<<1, h, t, index );
		s += Sum( (ps<<1)+1, h, t, index );
		return s;
	}
	static int HalfSearch( int n, int *index, int v ){
		int l = 0, r = n-1, mid, res;
		while( l<=r ){
			mid = (l+r)/2;
			if( index[mid] == v ) return mid;
			if( index[mid]<v ) l = mid+1;
			else r = mid-1;
		}
	}
	static void ans(){ // nkoj 1826 City Horizon
		const int N = 40008;
		int n, index[2*N];
		Point p[N], *t = new Point[8*N];
		while( scanf("%d",&n)==1 ){
			for(int i=0; i<n; i++){
				scanf("%d%d%d",&p[i].l,&p[i].r,&p[i].h);
				index[2*i] = p[i].l;
				index[2*i+1] = p[i].r;
			}
			sort( index, index+2*n );
			int len = 0;
			for(int i=1; i<2*n; i++)
				if( index[i]!=index[len] ) index[++len] = index[i];
			Init( 1, 0, len, t );
			for(int i=0; i<n; i++){
				int l = HalfSearch( len+1, index, p[i].l );
				int r = HalfSearch( len+1, index, p[i].r );
				Insert( 1, l, r, p[i].h, t );
			}
			printf("%I64d\n",Sum( 1, 0, t, index) );
		}
		delete []t;
	}
};

class Gourmet{
	struct Point{ int p, s; };
	struct Node{ int l, r, p; };
public:
	static bool cmpp( Point s, Point t ){ return s.p<t.p; }
	static bool cmps( Point s, Point t ){ return s.s<t.s; }
	static void Init( int ps, int l, int r, Node *nd, Point *P ){
		nd[ps].l = l; nd[ps].r = r;
		if( l == r ){ nd[ps].p = P[l].p; return; }
		int mid = (l+r)/2, t = 2*ps;
		Init( t, l, mid, nd, P );
		Init( t+1, mid+1, r, nd, P );
		nd[ps].p = min( nd[t].p, nd[t+1].p );
	}
	static int Find( int ps, Node *nd, int p, int s, Point *P ){
		if( nd[ps].p>p || P[nd[ps].l].s>s ) return -1;
		if( nd[ps].l == nd[ps].r ) return ps;
		int r = Find( (ps<<1)+1, nd, p, s, P );
		return ( r!=-1 ? r:Find( ps<<1, nd, p, s, P ) );
	}
	static void Update( int ps, Node *nd ){
		for( nd[ps].p=1000000001, ps/=2; ps>0; ps/=2 ){
			nd[ps].p = min( nd[ps<<1].p, nd[(ps<<1)+1].p );
		}
	}
	static void ans(){  // poj 3622  Gourmet Grazers
		const int N = 100008;
		int n, m;
		Point *p = new Point[2*N];
		Node *nd = new Node[8*N];
		while( scanf("%d%d",&n,&m)==2 ){
			for(int i=0; i<n+m; i++) scanf("%d%d",&p[i].p,&p[i].s);
			sort( p, p+n, cmps );
			sort( p+n, p+n+m, cmpp );
			Init( 1, 0, n-1, nd, p );
			__int64 res = 0; int k = 0;
			for(int i=n; i<n+m && k<n; i++){
				int t = Find( 1, nd, p[i].p, p[i].s, p );
				if( t == -1 ) continue;
				res += p[i].p;
				Update( t, nd );
				k++;
			}
			if( k < n ) res = -1;
			printf("%I64d\n", res);
		}
		delete []p;
		delete []nd;
	}
};

// poj Flower

#include<time.h>
class Cow{
	struct Node{ int lr; };
public:
	static void Insert( int ps, int *p, int val, int id, int level ){
		p[ps] = 1;
		if( level == 0 ){ p[ps] = id; return; }
		level--;
		if( val&(1<<level) ) Insert( ps<<1, p, val, id, level );
		else Insert( (ps<<1)+1, p, val, id, level );
	}
	static int Query( int ps, int *p, int val, int level ){
		if( level == 0 ) return p[ps];
		level--;
		if( p[(ps<<1)+1] && (p[ps<<1]==0 || (val&(1<<level))) ){
			return Query( (ps<<1)+1, p, val, level );
		}
		else return Query( ps<<1, p, val, level );
	}
	static void ans(){ // cow xor, Adrian Vladu 2005, waiting for submiting
		freopen("C:\\Documents and Settings\\zgmcn\\×ÀÃæ\\input.in", "r", stdin );
		freopen("C:\\Documents and Settings\\zgmcn\\×ÀÃæ\\acm.out", "w", stdout );
		int n, a[100001], *p = new int[(1<<22)];
		clock_t st = clock();
		while( scanf("%d",&n)==1 ){
			int mx = -1, len = 1;
			for(int i=1; i<=n; i++){
				scanf("%d",&a[i]);
				mx = max( mx, a[i] );
			}
			memset( p, 0, sizeof(int)*(1<<22) );
			a[0] = 0;
			for(int i=1; i<=n; i++){
				a[i] ^= a[i-1];
				if( mx < a[i] ){ mx = a[i]; len = i; }
				Insert( 1, p, a[i], i, 21 );
				int id = Query( 1, p, a[i], 21 );
				if( mx == (a[id]^a[i]) ) len = min( len, i-id );
				else if( mx < (a[id]^a[i]) ){
					mx = (a[id]^a[i]);
					len = i-id;
				}
			}
			for(int i=len; i<=n; i++){
				if( mx == (a[i]^a[i-len]) ){
					printf("%d %d %d\n", mx, i-len+1, i );
					break;
				}
			}
			//printf("%d %d\n", mx, len );
		}printf("%d\n", clock()-st);
		delete []p;
	}
};


int main(){
	//PopKart::ans();
	//Interval::ans();
	//Star::ans();
	//Horizon::ans();
	//Gourmet::ans();
	Cow::ans();
	return 0;
}