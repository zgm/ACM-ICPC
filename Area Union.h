#include<iostream>
#include<algorithm>

using namespace std;


class Area{
	struct Node{
		int l, r, cover; double len;
		void set( int L, int R, int C, double Len ){ l=L; r=R; cover=C; len=Len; }
	};
	struct Line{
		double x, d, u; bool r;
		void set( double X, double D, double U, bool R ){ x=X; d=D; u=U; r=R; }
	};
public:
	static bool cmp( Line s, Line t ){ return s.x<t.x; }
	static void Init( int t, int l, int r, Node *P ){
		P[t].set( l, r, 0, 0 );
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
	static double area( int n, double *x1, double *y1, double *x2, double *y2, Node *P, Line *Q ){
		double *y = new double[2*n]; int k = 0;
		memcpy( y, y1, sizeof(double)*n );
		memcpy( y+n, y2, sizeof(double)*n );
		sort( y, y+2*n );
		for(int i=1; i<2*n; i++) if( y[i]!=y[k] ) y[++k] = y[i];
		Init( 1, 0, k, P );
		
		for(int i=0; i<n; i++) Q[i].set( x1[i], y1[i], y2[i], 0 );
		for(int i=0; i<n; i++) Q[n+i].set( x2[i], y1[i], y2[i], 1 );
		sort( Q, Q+2*n, cmp );
		double s = 0;
		for(int i=0; i<2*n; i++){
			if( i>0 ) s += P[1].len*(Q[i].x-Q[i-1].x);
			int l = BSearch( k+1, y, Q[i].d );
			int r = BSearch( k+1, y, Q[i].u );
			if( Q[i].r ) Delete( 1, l, r, P, y );
			else Insert( 1, l, r, P, y );
		}
		delete []y;
		return s;
	}
	/*static void ans(){ // poj 2760 End of Windless Days
		const int N = 508;
		int n;
		double x1[N], y1[N], x2[N], y2[N];
		Node P[3*N]; Line Q[2*N];
		while( scanf("%d",&n)==1 ){
			double mnx, mny, mx, my, lx, ly, lh;
			scanf("%lf%lf%lf%lf",&mnx,&mny,&mx,&my);
			scanf("%lf%lf%lf",&lx,&ly,&lh);
			double a1, b1, a2, b2, h;
			for(int i=0; i<n; i++){
				scanf("%lf%lf%lf%lf%lf",&a1,&b1,&a2,&b2,&h);
				a1 = lx+lh*(a1-lx)/(lh-h);
				b1 = ly+lh*(b1-ly)/(lh-h);
				a2 = lx+lh*(a2-lx)/(lh-h);
				b2 = ly+lh*(b2-ly)/(lh-h);
				if( a1>=mx || b1>=my ){ i--; n--; continue; }
				if( a2<=mnx || b2<=mny ){ i--; n--; continue; }

				x1[i] = min( max( a1, mnx ), mx );
				x2[i] = min( max( a2, mnx ), mx );
				y1[i] = min( max( b1, mny ), my );
				y2[i] = min( max( b2, mny ), my );
			}
			printf("%.4lf\n", (mx-mnx)*(my-mny)-area( n, x1, y1, x2, y2, P, Q ) );
		}
	}*/
	static void ans(){ // poj 1151 Atlantis
		const int N = 508;
		int T = 0, n;
		double x1[N], y1[N], x2[N], y2[N];
		Node P[3*N]; Line Q[2*N];
		while( scanf("%d",&n)==1 && n ){
			for(int i=0; i<n; i++) scanf("%lf%lf%lf%lf",&x1[i],&y1[i],&x2[i],&y2[i]);
			printf("Test case #%d\n", ++T );
			printf("Total explored area: %.2lf\n\n", area( n, x1, y1, x2, y2, P, Q ) );
		}
	}
	// other problem:  poj 3695 Rectangles
};

int main(){
	Area::ans();
	return 0;
}