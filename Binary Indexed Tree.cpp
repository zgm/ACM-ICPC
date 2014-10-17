#include<iostream>
#include<cstdio>
#include<cstring>
#include<algorithm>
using namespace std;

class BinaryIndexedTree{
public:
    // return the lowest bit with value=1
	static int Lowbit( int t ){ return ( t&(t^(t-1)) ); } // t&(-t)
	static void Plus( int x, int delta, int *a, int N ){
		for( ; x<N; x+=Lowbit(x) ) a[x] += delta;
	}
    /*static void Plus( int x, int delta, int *a ){
        for( ; x>0; x-=Lowbit(x) ) a[x] += delta;
    }*/
	static void Plus( int x, int y, int delta, int *a, int N ){
		for( ; x<N; x+=Lowbit(x) ){
			int k = max(delta,-a[x*N+y]); // keep the final value non-negative
			for(int j=y; j<N; j+=Lowbit(j) ) a[x*N+j] += k;
		}
	}
	static int Sum( int x, int *a, int s=0 ){
		for( ; x>0; x-=Lowbit(x) ) s += a[x];
		return s;
	}
    /*static int Sum( int x, int *a, int N, int s=0 ){
        for( ; x<N; x+=Lowbit(x) ) s += a[x];
        return s;
    }*/
	static int Sum( int x, int y, int *a, int N, int s=0 ){
		for( ; x>0; x-=Lowbit(x) ){
			for(int j=y; j>0; j-=Lowbit(j) ) s += a[x*N+j];
		}
		return s;
	}
    // update [i,j], sum [i,j]
    static void ans(){
        int n, a[N], x, y;
        while( scanf("%d",&n)==1 && n ){
            memset( a, 0, sizeof(int)*(n+2) );
            for(int i=0; i<n; i++){
                scanf("%d%d",&x,&y);
                Add( x, 1, n+2, a );
                Add( y+1, -1, n+2, a );
            }
            for(int i=1; i<=n; i++){
                if( i>1 ) printf(" ");
                printf("%d",Sum(i,a));
            }
            printf("\n");
        }
    }
    static void ans(){ // same with the above one  hdu 1556
        int n, x, y, b[N], c[N];
        while( scanf("%d",&n)==1 && n ){
            memset( b, 0, sizeof(int)*(n+1) );
            memset( c, 0, sizeof(int)*(n+2) );
            for(int i=0; i<n; i++){
                scanf("%d%d",&x,&y);
                Add( y, 1*y, n+1, b );
                Add( 1, 1, n+2, c );
                Add( y, -1, n+2, c );
                if( x>1 ){
                    Add( x-1, -1*(x-1), n+1, b );
                    Add( 1, -1, n+2, c );
                    Add( x-1, 1, n+2, c );
                }
            }
            for(int i=1; i<=n; i++){
                if( i>1 ) printf(" ");
                printf("%d",Sum(i,b)+Sum(i,c)*i,-Sum(i-1,b)-Sum(i-1,c)*(i-1));
            }
            printf("\n");
        }
    }
    // need to make sure that the Kth number exists
	static int Find( int k, int *a, int N, int res=0 ){
		int i, cnt = 0;
		for(i=0; (1<<i)<N; i++);
		for(i-=1; i>=0; i--){
			res += (1<<i);
			if( res>=N || cnt+a[res]>=k ) res -= (1<<i);
			else cnt += a[res];
		}
		return ++res;
	}
	static int Query( int x, int all, int res, int delta, int *a ){
		int rv = 0, step = (1<<18), nowv = 0, last;
		if( all-res < x ) return -1;
		x = all-res+1-x;
		while( nowv!=x && step!=0 ){
			while( a[rv+step]+nowv>=x && step>0 ){
				last = rv+step;
				step >>= 1;
			}
			if( step==0 ) continue;
			rv += step;
			nowv += a[rv];
			step >>= 1;
		}
		return last;
	}
	static void ans(){ // nkoj 1248 ÓôÃÆµÄ³öÄÉÔ±
		const int N = 300002;
		int all = 0, res = 0, delta = 100001, a[N];
		int n, mn, k; char c;
		scanf("%d%d",&n,&mn);
		memset( a, 0, sizeof(int)*N );
		for(int i=0; i<n; i++){
			getchar();
			c = getchar();
			scanf("%d",&k);
			if( c=='I' ){
				if( k>=mn ){ all++; Plus( k+delta, 1, a, N ); }
				continue;
			}
			if( c=='A' ){ delta -= k; continue; }
			if( c=='S' ){
				for(int j=mn+delta; j<=mn+delta+k-1; j++){
					while( a[j]>0 ){ res++; Plus( j, -1, a, N ); }
				}
				delta += k;
				continue;
			}
			if( c=='F' ){ printf("%d\n",Query( k, all, res, delta,  a ) ); continue; }
		}
		printf("%d\n", res);
	}
	/*static void ans(){ // poj 2155 Matrix
		int t, n, m, a[1000*1008];
		scanf("%d",&t);
		while( t-- ){
			scanf("%d%d",&n,&m);
			char c;
			int x1, y1, x2, y2;
			memset( a, 0, sizeof(int)*(n+1)*(n+1) );
			while( m-- ){
				getchar();
				c = getchar();
				scanf("%d%d",&x1,&y1);
				if( c == 'C' ){
					scanf("%d%d",&x2,&y2);
					Plus( x1, y1, 1, a, n+1 );
					if( y2<n ) Plus( x1, y2+1, 1, a, n+1 );
					if( x2<n ) Plus( x2+1, y1, 1, a, n+1 );
					if( x2<n && y2<n ) Plus( x2+1, y2+1, 1, a, n+1 );
				}
				else printf("%d\n",Sum( x1, y1, a, n+1)%2 );
			}
			printf("\n");
		}
	}*/
	/*struct Point{ int v, id; };
	static bool cmp( Point s, Point t){ return s.v<t.v; }
	static int NewSort( int n, int *s, Point *p, int *key ){
		for(int i=0; i<n; i++){ p[i].v = s[i]; p[i].id = i; }
		sort( p, p+n, cmp );
		int k = 0;
		for(int i=0, pre; i<n; i++){
			if( i!=0 && s[p[i].id]!=pre ) k++;
			key[k] = s[p[i].id];
			pre = key[k];
			s[p[i].id] = k;
		}
		return k+1;
	}
	// find min{ res | a[res]>val }(0<=res<n), if a[n-1]<val return n
	static int BSearch( int n, int *a, int val ){
		if( a[n-1]<val ) return n;
		int l = 0, r = n-1, mid, res = -1;
		while( l<=r ){
			mid = (l+r)/2;
			if( a[mid] < val ) l = mid+1;
			else{ r = mid-1; res = mid; }
		}
		return res;
	}
	static void ans(){ // nuaa 1119 Stones for Amy
		int t = 0, n, k, a[20008], s[20008], key[20008];
		Point p[20008];
		while( scanf("%d%d",&n,&k)==2 ){
			for(int i=0; i<n; i++) scanf("%d",&a[i]);
			s[0] = a[0];
			for(int i=1; i<n; i++) s[i] = s[i-1]+a[i];
			int len = NewSort( n, s, p, key );
			int l = 0, r = 0, mid, res;
			for(int i=0; i<n; i++) if( a[i]<0 ) l += a[i];
			for(int i=0; i<n; i++) if( a[i]>0 ) r += a[i];
			while( l<=r ){
				mid = (l+r)/2;
				int num = 0;
				for(int i=0; i<n; i++) if( key[s[i]]<=mid ) num++;
				memset( a, 0, sizeof(int)*(len+1) );
				for(int i=0; i<n; i++){
					int id = BSearch( len, key, key[s[i]]-mid );
					num += i-Sum( id, a );
					Plus( s[i]+1, 1, a, len+1 );
				}
				if( num>=k ){ r = mid-1; res = mid; }
				else l = mid+1;
			}
			printf("Case %d: %d\n", ++t, res );
		}
	}*/
	/*class BigNum{
	public:
		int n, a[100];
		BigNum(){ n=1; memset( a, 0, sizeof(a) ); }
		BigNum operator+( __int64 s ){
			for(int i=0; s>0; i++){
				a[i] += s%10; s /= 10;
				if( s == 0 ) n = max( n, i+1 );
			}
			for(int i=0; i<n; i++){
				a[i+1] += a[i]/10;
				a[i] %= 10;
			}
			if( a[n] != 0 ) n++;
			return (*this);
		}
		void Print(){
			for(int i=n-1; i>=0; i--) printf("%d",a[i]);
			printf("\n");
		}
	};
	struct Node{ int x, p; };
	static bool cmp( Node s, Node t ){ return s.x<t.x; }
	static void ans(){ // poj 3378 Crazy Thairs
		const int N = 50008;
		int n, t[N];
		__int64 *a = new __int64[N], *b = new __int64[N], *c = new __int64[N];
		Node *p = new Node[N];
		while( scanf("%d",&n)==1 ){
			for(int i=0; i<n; i++){ scanf("%d",&p[i].x); p[i].p = i; }
			sort( p, p+n, cmp );
			t[p[0].p] = 1;
			for(int i=1; i<n; i++){
				t[p[i].p] = t[p[i-1].p];
				if( p[i].x != p[i-1].x ) t[p[i].p]++;
			}
			memset( a, 0, sizeof(__int64)*N );
			for(int i=0; i<n; i++){
				b[i] = Sum( t[i]-1, a );
				Plus( t[i], 1, a, N );
			}
			memset( a, 0, sizeof(__int64)*N );
			for(int i=0; i<n; i++){
				c[i] = Sum( t[i]-1, a );
				Plus( t[i], b[i], a, N );
			}
			memset( a, 0, sizeof(__int64)*N );
			for(int i=0; i<n; i++){
				b[i] = Sum( t[i]-1, a );
				Plus( t[i], c[i], a, N );
			}
			memset( a, 0, sizeof(__int64)*N );
			BigNum s;
			for(int i=0; i<n; i++){
				c[i] = Sum( t[i]-1, a );
				Plus( t[i], b[i], a, N );
				s = s+c[i];
			}
			s.Print();
		}
		delete []p;
	}*/
	/*struct Node{ int x, y, p; };
	static bool cmp( Node s, Node t ){ return ( s.y>t.y || (s.y==t.y&&s.x<t.x) ); }
	static void ans(){ // poj 2481 Cows
		const int N = 100008;
		int n, a[N], res[N];
		Node *p = new Node[N];
		while( scanf("%d",&n)==1 && n ){
			for(int i=0; i<n; i++){
				scanf("%d%d",&p[i].x,&p[i].y);
				p[i].x++; p[i].y++; p[i].p = i;
			}
			sort( p, p+n, cmp );
			memset( a, 0, sizeof(int)*N );
			for(int i=0; i<n; i++){
				if( i>=1 && p[i].x==p[i-1].x && p[i].y==p[i-1].y )
					res[p[i].p] =  res[p[i-1].p];
				else res[p[i].p] = Sum( p[i].x, a );
				Plus( p[i].x, 1, a, N );
			}
			for(int i=0; i<n; i++){
				if( i!=0 ) printf(" ");
				printf("%d",res[i]);
			}
			printf("\n");
		}
		delete []p;
	}*/
	/*struct Node{ int v, x; };
	static bool cmp( Node s, Node t ){ return s.v<t.v; }
	static void ans(){ // poj 1990 MooFest
		const int N = 20008;
		int n, num[N], total[N];
		Node s[N];
		while( scanf("%d",&n)==1 ){
			memset( num, 0, sizeof(num) );
			memset( total, 0, sizeof(total) );
			for(int i=0; i<n; i++) scanf("%d%d",&s[i].v,&s[i].x);
			sort( s, s+n, cmp );
			__int64 res = 0;
			for(int i=0; i<n; i++){
				int r = Sum( s[i].x, num );
				int t1 = Sum( s[i].x, total );
				int t2 = Sum( N-1, total );
				res += s[i].v*( (__int64)s[i].x*(2*r-i)+t2-2*t1 );
				Plus( s[i].x, 1, num, N );
				Plus( s[i].x, s[i].x, total, N );
			}
			printf("%I64d\n", res );
		}
	}*/
	/*static void ans(){ // poj 1195 Mobile phones
		const int N = 1025;
		int n, *a = new int[N*N], cmd, x, y, z, w;
		while( scanf("0 %d",&n)==1 ){
			memset( a, 0, sizeof(int)*N*N );
			while( true ){
				scanf("%d",&cmd);
				if( cmd == 3 ) break;
				if( cmd == 1 ){
					scanf("%d%d%d",&x,&y,&z);
					Plus( x+1, y+1, z, a, N );
				}
				else if( cmd == 2 ){
					scanf("%d%d%d%d",&x,&y,&z,&w);
					x++; y++; z++; w++;
					int x1 = Sum( x-1, y-1, a, N );
					int x2 = Sum( x-1, w, a, N );
					int x3 = Sum( z, y-1, a, N );
					int x4 = Sum( z, w, a, N );
					printf("%d\n",x4+x1-x2-x3);
				}
			}
		}
		delete []a;
	}*/
	/*struct Node{ int x, y, p; };
	static bool cmp( Node s, Node t ){ return s.x<t.x; }
	static void ans(){ // poj 3416 Crossing
		const int N = 500002, M = 50002;
		int t, n, m, *x = new int[N], *y = new int[N], *res = new int[M];
		Node *s = new Node[M], *p = new Node[M];
		scanf("%d",&t);
		for(int i=0; i<t; i++){
			if( i != 0 ) printf("\n");
			scanf("%d%d",&n,&m);

			memset( x, 0, sizeof(int)*N );
			memset( y, 0, sizeof(int)*N );
			for(int j=0; j<n; j++){
				scanf("%d%d", &s[j].x, &s[j].y);
				s[j].x++; s[j].y++;
			}
			for(int j=0; j<m; j++){
				scanf("%d%d", &p[j].x, &p[j].y);
				p[j].x++; p[j].y++; p[j].p = j;
			}
			sort( s, s+n, cmp );
			sort( p, p+m, cmp );
			for(int j=0, k=0; j<n; j++){
				while( k<m && p[k].x<s[j].x ){
					res[p[k].p] = 4*Sum( p[k].y, y )-2*Sum( p[k].x, x )+n;
					k++;
				}
				Plus( s[j].x, 1, x, N );
				Plus( s[j].y, 1, y, N );
			}
			for(int j=0; j<m; j++){
				res[p[j].p] -= 2*Sum( p[j].y, y );
				if( res[p[j].p] < 0 ) res[p[j].p] = -res[p[j].p];
			}
			for(int j=0; j<m; j++) printf("%d\n",res[j]);
		}
		delete []s;
		delete []p;
	}*/
};

int main(){
	BinaryIndexedTree::ans();
	return 0;
}
