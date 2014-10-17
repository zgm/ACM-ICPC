#include<iostream>
#include<algorithm>
using namespace std;

class Tree{
	struct Node{
		int l, r, cover, e; double len; bool lc, rc;
		void set( int L, int R ){ l=L; r=R; cover=0; e=0; len=0; lc=rc=false; }
	};
	struct Line{
		double x, d, u; bool r;
		void set( double X, double D, double U, bool R ){ x=X; d=D; u=U; r=R; }
	};
public:
	static bool cmp( Line s, Line t ){ return ( s.x<t.x || (s.x==t.x&&!s.r&&t.r) ); }
	static void Init( int t, int l, int r, Node *P ){
		P[t].set( l, r );
		if( l+1 >= r ) return;
		Init( t<<1, l, (l+r)>>1, P );
		Init( (t<<1)+1, (l+r)>>1, r, P );
	}
	static void Insert( int t, int l, int r, Node *P, double *y ){
		if( l<=P[t].l && P[t].r<=r ) P[t].cover++;
		else{
			int mid = (P[t].l+P[t].r)/2;
			if( l < mid ) Insert( t<<1, l, r, P, y );
			if( r > mid ) Insert( (t<<1)+1, l, r, P, y );
		}
		Update( t, P, y );
	}
	static void Delete( int t, int l, int r, Node *P, double *y ){
		if( l<=P[t].l && P[t].r<=r ) P[t].cover--;
		else{
			int mid = (P[t].l+P[t].r)/2;
			if( l < mid ) Delete( t<<1, l, r, P, y );
			if( r > mid ) Delete( (t<<1)+1, l, r, P, y );
		}
		Update( t, P, y );
	}
	static void Update( int t, Node *P, double *y ){
		if( P[t].cover > 0 ) P[t].len = y[P[t].r]-y[P[t].l];
		else if( P[t].l+1 == P[t].r ) P[t].len = 0;
		else P[t].len = P[t<<1].len+P[(t<<1)+1].len;
		
		if( P[t].cover > 0 ){ P[t].lc = P[t].rc = true; P[t].e = 2; }
		else if( P[t].l+1 == P[t].r ){ P[t].lc = P[t].rc = false; P[t].e = 0; }
		else{
			P[t].lc = P[(t<<1)].lc;
			P[t].rc = P[(t<<1)+1].rc;
			P[t].e = P[t<<1].e+P[(t<<1)+1].e-2*(P[t<<1].rc*P[(t<<1)+1].lc);
		}
	}
	static int BSearch( int n, double *a, double v, double eps=1e-8 ){
		int l = 0, r = n-1, mid, res;
		while( l<=r ){
			mid = (l+r)/2;
			if( a[mid]>=v-eps ){ res = mid; r = mid-1; }
			else l = mid+1;
		}
		return res;
	}
	static double Perimeter( int n, double *x1, double *y1, double *x2, double *y2, Node *P, Line *Q ){
		double *y = new double[2*n]; int k = 0;
		memcpy( y, y1, sizeof(double)*n );
		memcpy( y+n, y2, sizeof(double)*n );
		sort( y, y+2*n );
		for(int i=1; i<2*n; i++) if( y[i]!=y[k] ) y[++k] = y[i];
		Init( 1, 0, k, P );
		
		for(int i=0; i<n; i++) Q[i].set( x1[i], y1[i], y2[i], 0 );
		for(int i=0; i<n; i++) Q[n+i].set( x2[i], y1[i], y2[i], 1 );
		sort( Q, Q+2*n, cmp );
		double s = 0, len = 0;
		for(int i=0; i<2*n; i++){
			int l = BSearch( k+1, y, Q[i].d );
			int r = BSearch( k+1, y, Q[i].u );
			if( Q[i].r ) Delete( 1, l, r, P, y );
			else Insert( 1, l, r, P, y );
			if( i != 2*n-1 ) s += P[1].e*(Q[i+1].x-Q[i].x);
			s += abs( P[1].len-len );
			len = P[1].len;
		}
		delete []y;
		return s;
	}
	static void ans(){ // poj 1177  Picture
		const int N = 5008;
		int T = 0, n;
		double x1[N], y1[N], x2[N], y2[N];
		Node P[3*N]; Line Q[2*N];
		while( scanf("%d",&n)==1 && n ){
			for(int i=0; i<n; i++) scanf("%lf%lf%lf%lf",&x1[i],&y1[i],&x2[i],&y2[i]);
			printf("%.0lf\n", Perimeter( n, x1, y1, x2, y2, P, Q ) );
		}
	}
};

int main(){
	Tree::ans();
	return 0;
}