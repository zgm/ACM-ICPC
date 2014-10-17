#include<cstdio>
#include<iostream>
#include<vector>
#include<cstring>
#include<cmath>
#include<deque>
#include<queue>
#include<algorithm>
using namespace std;

const double eps = 1e-8, small = -1e5, big = 1e5;

class HalfPlane{
	struct Point{ double x, y; };
	struct Line{
		Point p1, p2; double at2;
		void set( double x1, double y1, double x2, double y2 ){
			p1.x = x1; p1.y = y1; p2.x = x2; p2.y = y2;
			at2 = atan2( p2.y-p1.y, p2.x-p1.x );
			//printf("%.6f %.6f %.6f %.6f %.6f\n", x1, y1, x2, y2, at2 );
		}
	};
public:
	// -1: [-oo, -eps);  0:(-eps,eps); 1:(eps,+oo)
	static int sig( double k ){ return ( (k<-eps) ? -1:(k>eps) ); }
	// 判断p2是否在p0->p1的左边，在左边的话 sig(CrossPlot()) >= 0
	static double CrossPlot( Point p0, Point p1, Point p2 ){
		return (p1.x-p0.x)*(p2.y-p0.y)-(p1.y-p0.y)*(p2.x-p0.x);
	}
	// 线段求交点
	static bool Intersect( Point s1, Point t1, Point s2, Point t2, Point &p ){
		double d1 = CrossPlot( s2, t1, s1 );
		double d2 = CrossPlot( t1, t2, s1 );
		if( fabs(d1+d2)<eps ) return false;
		p.x = (s2.x*d2+t2.x*d1)/(d1+d2);
		p.y = (s2.y*d2+t2.y*d1)/(d1+d2);
		return true;
	}
	static bool cmp( Line &s, Line &t ){
		if( sig(s.at2-t.at2)!=0 ) return s.at2<t.at2;
		return CrossPlot( t.p1, t.p2, s.p2 )>0;
	}
	// 判断最近两个半平面的交点在当前半平面外
	static bool JudgeIn( Line &x, Line &y, Line &z ){
		Point p;
		Intersect( x.p1, x.p2, y.p1, y.p2, p );
		return CrossPlot( z.p1, z.p2, p )<0;
	}
	static bool JudgeIn( int x, int y, int z, Line *ln ){
		return JudgeIn( ln[x], ln[y], ln[z] );
	}
	static void Intersection( int &n, Line *ln, Point *p ){
		int *d = new int[n+1];
		sort( ln, ln+n, cmp );
		int tn = 1;
		for(int i=1; i<n; i++){
			if( sig(ln[i-1].at2-ln[i].at2)!=0 ) ln[tn++] = ln[i];
		}
		n = tn;
		int bot = 1, top = 2;
		d[bot] = 0; d[top] = 1;
		for(int i=2; i<n; i++){
			while( bot<top && JudgeIn(d[top-1],d[top],i,ln) ) top--;
			//if( bot==top && JudgeIn(i,d[bot],d[bot+1],ln) ){ n = -1; return; }
			//bool ok = (bot==top);
			while( bot<top && JudgeIn(d[bot+1],d[bot],i,ln) ) bot++;
			//if( !ok && bot==top && JudgeIn(i,d[bot],d[bot-1],ln) ){ n = -1; return; }
			d[++top] = i;
		}
		while( bot<top && JudgeIn(d[top-1],d[top],d[bot],ln) ) top--;
		while( bot<top && JudgeIn(d[bot+1],d[bot],d[top],ln) ) bot++;
		d[--bot] = d[top];
		n = 0;
		for(int i=bot+1; i<=top; i++){
			Intersect( ln[d[i-1]].p1, ln[d[i-1]].p2, ln[d[i]].p1, ln[d[i]].p2, p[n++] );
		}
		delete []d;
	}
	static double Area( int i, int j, int k, Point *v){
		return CrossPlot( v[i], v[j], v[k] )/2;
	}
	static double Area( int n, Point *v ){
		double res = 0;
		for(int i=2; i<n; i++) res += Area( 0, i-1, i, v );
		return res;
	}
	/*static void ans(){ // poj 1279 Art Gallery
		const int N = 20008;
		int T, n; double x[N], y[N];
		Line *ln = new Line[N];
		Point p[N];
		scanf("%d",&T);
		while( T-- ){
			scanf("%d",&n);
			for(int i=0; i<n; i++) scanf("%lf%lf",&x[n-1-i],&y[n-1-i]);
			for(int i=0; i<n; i++){
				ln[i].set( x[i], y[i], x[(i+1)%n], y[(i+1)%n] );
			}
			ln[n++].set( small, small, big, small );
			ln[n++].set( big, small, big, big );
			ln[n++].set( big, big, small, big );
			ln[n++].set( small, big, small, small );
			Intersection( n, ln, p );
			printf("%.2f\n", Area( n, p ) );
		}
	}*/
	/*static void ans(){ // poj 3130 How I Mathematician Wonder What You Are!
		const int N = 20008;
		int n; double x[N], y[N];
		Line *ln = new Line[N];
		Point p[N];
		while( scanf("%d",&n)==1 && n!=0 ){
			for(int i=0; i<n; i++) scanf("%lf%lf",&x[i],&y[i]);
			for(int i=0; i<n; i++){
				ln[i].set( x[i], y[i], x[(i+1)%n], y[(i+1)%n] );
			}
			ln[n++].set( small, small, big, small );
			ln[n++].set( big, small, big, big );
			ln[n++].set( big, big, small, big );
			ln[n++].set( small, big, small, small );
			Intersection( n, ln, p );
			if( Area( n, p )<eps ) printf("0\n");
			else printf("1\n");
		}
	}*/
	/*static void ans(){ // poj 1474 Video Surveillance
		const int N = 20008;
		int cs = 0, n; double x[N], y[N];
		Line *ln = new Line[N];
		Point p[N];
		while( scanf("%d",&n)==1 && n!=0 ){
			for(int i=0; i<n; i++) scanf("%lf%lf",&x[n-1-i],&y[n-1-i]);
			for(int i=0; i<n; i++){
				ln[i].set( x[i], y[i], x[(i+1)%n], y[(i+1)%n] );
			}
			ln[n++].set( small, small, big, small );
			ln[n++].set( big, small, big, big );
			ln[n++].set( big, big, small, big );
			ln[n++].set( small, big, small, small );
			Intersection( n, ln, p );
			//for(int i=0; i<n; i++) printf("%.2f %.2f\n", p[i].x,p[i].y);
			printf("Floor #%d\n", ++cs);
			if( n<3 ) printf("Surveillance is impossible.\n\n");
			else printf("Surveillance is possible.\n\n");
		}
	}*/
	// 根据a*x+b*y<c, 计算两点p1和p2,使得p1->p2的左边是所求平面
	static void GetPoint( double a, double b, double c, Point &p1, Point &p2 ){
		if( fabs(a)<eps ){ p1.x = 0; p1.y = c/b; p2.x = 1; p2.y = c/b; }
		else if( fabs(b)<eps ){ p1.x = c/a; p1.y = 0; p2.x = c/a; p2.y = 1; }
		else{ p1.x = 0; p1.y = c/b; p2.x = 1; p2.y = (c-a)/b; }
		double at2 = atan2( p2.y-p1.y, p2.x-p1.x )+0.1;
		double d = sqrt((p2.x-p1.x)*(p2.x-p1.x)+(p2.y-p1.y)*(p2.y-p1.y));
		Point p;
		p.x = p1.x+d*cos(at2);
		p.y = p1.y+d*sin(at2);
		if( a*p.x+b*p.y>c ) swap( p1, p2 );
	}
	/*static void ans(){ // poj 1755 Triathlon
		const int N = 208;
		int n; double v[N], u[N], w[N];
		Line ln[N]; Point p[N], p1, p2;
		while( scanf("%d",&n)==1 ){
			for(int i=0; i<n; i++) scanf("%lf%lf%lf",&v[i],&u[i],&w[i]);
			for(int i=0; i<n; i++){
				int tn = 0;
				for(int j=0; j<n; j++){
					if( i==j ) continue;
					double c = 1/w[j]-1/w[i];
					double a = 1/v[i]-1/v[j]+c;
					double b = 1/u[i]-1/u[j]+c;
					if( fabs(a)<eps && fabs(b)<eps ){
						if( c<eps ){ tn = -1; break; }
						else continue;
					}
					GetPoint( a, b, c, p1, p2 );
					ln[tn++].set( p1.x, p1.y, p2.x, p2.y );
				}
				if( tn == -1 ){ printf("No\n"); continue; }
				ln[tn++].set( 0, 0, 1, 0 );
				ln[tn++].set( 1, 0, 0, 1 );
				ln[tn++].set( 0, 1, 0, 0 );
				Intersection( tn, ln, p );
				//printf("%d %.12f  ",tn,Area(tn,p) );
				//for(int j=0; j<tn; j++) printf("%.8f %.8f  ",p[j].x,p[j].y);
				if( Area( tn, p )<1e-16 ) printf("No\n");
				else printf("Yes\n");
			}
		}
	}*/
	// 向p1->p2的左侧平移d距离
	static void InwardShift( double x1, double y1, double x2, double y2, double d, Line &t ){
		double kx = y1-y2, ky = x2-x1;
		double kd = sqrt( kx*kx+ky*ky );
		kx /= kd; ky /= kd;
		t.set( x1+kx*d, y1+ky*d, x2+kx*d, y2+ky*d );
	}
	static void ans(){ // sgu 332  Largest Circle;  poj 3525 Most Distant Point from the Sea
		const int N = 10008;
		int n; double x[N], y[N];
		Line ln[N]; Point p[N];
		while( scanf("%d",&n)==1 && n ){
			for(int i=0; i<n; i++) scanf("%lf%lf",&x[i],&y[i]);
			x[n] = x[0]; y[n] = y[0];
			double l = 0, r = 10000000, mid;
			while( l+1e-4<r ){
				mid = (l+r)/2;
				for(int i=1; i<=n; i++){
					InwardShift( x[i-1], y[i-1], x[i], y[i], mid, ln[i-1] );
				}
				int pn = n;
				Intersection( pn, ln, p );
				if( pn>=3 && Area( pn, p )>=0 ) l = mid;
				else r = mid;
			}
			printf("%.4f\n", l);
		}
	}
};

int main(){
	//freopen("G:\\program\\NEERC\\2000 tests\\triath.23","r",stdin);
	//freopen("G:\\program\\NEERC\\2000 tests\\out.txt","w",stdout);
	HalfPlane::ans();
	return 0;
}
