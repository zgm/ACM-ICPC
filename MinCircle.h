#include<iostream>
#include<time.h>
#include<cmath>
#include<algorithm>
using namespace std;

class MinCircle{
	struct Point{
		double x, y;
		Point( double a=0, double b=0){ x = a; y = b; }
	};
	struct Circle{
		double x, y, r;
		void set( double a, double b, double c){ x = a; y = b; r = c; }
		void set( Point s, double c ){ set( s.x, s.y, c ); }
	};
public:
	static double Distance( Point s, Point t ){
		return sqrt( (s.x-t.x)*(s.x-t.x)+(s.y-t.y)*(s.y-t.y) );
	}
	static Point Solve( Point *p ){
		double x1 = p[0].x-p[1].x;
		double y1 = p[0].y-p[1].y;
		double x2 = p[0].x-p[2].x;
		double y2 = p[0].y-p[2].y;
		double z1 = (p[0].x*p[0].x+p[0].y*p[0].y-p[1].x*p[1].x-p[1].y*p[1].y)/2;
		double z2 = (p[0].x*p[0].x+p[0].y*p[0].y-p[2].x*p[2].x-p[2].y*p[2].y)/2;
		return Point( (z1*y2-z2*y1)/(x1*y2-x2*y1), (x1*z2-x2*z1)/(x1*y2-x2*y1) );
	}
	static Circle Min( int n, int m, Point *p, Point *q ){
		Circle s;
		if( n==0 || m==3 ){
			if( m==1 ) s.set( q[0].x, q[0].y, 0 );
			else if( m==2 ){
				s.set( (q[0].x+q[1].x)/2, (q[0].y+q[1].y)/2, Distance( q[0], q[1] )/2 );
			}
			else{
				Point t = Solve( q );
				s.set( t, Distance( t, q[0] ) );
			}
			return s;
		}
		if( m==0 && n==1 ) s.set( p[0], 0 );
		else s = Min( n-1, m, p, q );
		if( Distance( Point( s.x, s.y ), p[n-1] ) > s.r+1e-8 ){
			q[m++] = p[n-1];
			s = Min( n-1, m, p, q );
			m--;
		}
		return s;
	}
	static void ans(){ // hdu 3007 Buried memory
		int n;
		Circle s; Point q[3];
		srand( (unsigned)time(NULL) );
		while( scanf("%d",&n)==1 && n ){
			Point *p = new Point[n];
			for(int i=0; i<n; i++) scanf("%lf%lf",&p[i].x,&p[i].y);
			random_shuffle( &p[0], &p[n] );
			s = Min( n, 0, p, q );
			printf("%.2lf %.2lf %.2lf\n",s.x,s.y,s.r);
			delete []p;
		}
	}
};

int main(){
	MinCircle::ans();
	return 0;
}