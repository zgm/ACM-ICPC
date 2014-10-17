//#define _CRT_SECURE_NO_DEPRECATE
//#pragma warning(disable:4996)
#include<cstdio>
#include<cmath>
#include<vector>
#include<set>
#include<algorithm>
#include<iostream>
using namespace std;

const double eps = 1e-8, PI = 3.1415926535897932384626433832795, INF = 1e30;

bool Equal(double s, double t){ return fabs(s-t)<eps; }
bool Between(double x, double l, double r){ return min(l,r)-eps<=x && x<=max(l,r)+eps; }
bool Equation(double a, double b, double c, double &x1, double &x2){
	double dt = b*b-4*a*c;
	if( dt < -eps ) return false;
	dt = sqrt( max( 0.0, dt ) );
	x1 = (-b+dt)/(2*a); x2 = (-b-dt)/(2*a);
	return true;
}
// 求解 a1*x+b1*y = c1, a2*x+b2*y = c2
bool Equation( double a1, double b1, double c1, double a2, double b2, double c2,
			  double &x, double &y ){
	if( Equal( a2*b1, a1*b2 ) ) return false; // 无解或无穷解
	y = (a1*c2-a2*c1)/(a2*b1-a1*b2);
	x = (-c1-b1*y)/a1;
	return true;
}

class Point{
public:
	double x, y, z;
	Point( double X=0, double Y=0, double Z=0 ){ Set(X,Y,Z); }
	void Set( double X, double Y, double Z=0 ){ x = X; y = Y; z = Z; }
	void Set( Point t){ Set( t.x, t.y, t.z ); }
	double PointPlot( Point t ){ return x*t.x+y*t.y; }
	double CrossPlot( Point t ){ return x*t.y-y*t.x; }
	double Norm(){ return sqrt(x*x+y*y); }
	double Norm2(){ return x*x+y*y; }
	double Distance( Point t){ return sqrt( (x-t.x)*(x-t.x)+(y-t.y)*(y-t.y) ); }
	double Distance2( Point t){ return (x-t.x)*(x-t.x)+(y-t.y)*(y-t.y); }
	bool Equal( Point t ){ return ::Equal(x,t.x) && ::Equal(y,t.y); }
	double Slope( Point t ){ return (fabs(t.x-x)<eps ? INF:(t.y-y)/(t.x-x) ); }
	bool Between( Point s, Point t ){ return ::Between(x,s.x,t.x) && ::Between(y,s.y,t.y);}
	
	friend Point operator -( Point s, Point t){ return Point(s.x-t.x,s.y-t.y); }
	friend Point operator +( Point s, Point t){ return Point(s.x+t.x,s.y+t.y); }
	friend double operator*( Point s, Point t){ return s.x*t.y-s.y*t.x; }
	friend double CrossPlot( Point a, Point b, Point c ){
		return (b.x-a.x)*(c.y-a.y)-(b.y-a.y)*(c.x-a.x);
	}
}first;

class Line{
public:
	Point a, b;
	Line( double x=0, double y=0, double z=0, double w=0 ){ Set(x,y,z,w ); }
	Line( Point s, Point t ){ Set( s, t ); }
	void Set( double x, double y, double z, double w){ a.Set(x,y); b.Set(z,w); }
	void Set( Point s, Point t ){ a = s; b = t; }
	double Slope(){ return a.Slope( b ); }
	double Distance( Point t ){ return 0.5*fabs(CrossPlot(a,t,b))/a.Distance(b); }
	bool PointOn( Point t ){ return t.Between(a,b) && Equal(CrossPlot(a,t,b),0); }
	void SymPoint( Point t, Point &p ){
		double dx = b.x-a.x, dy = b.y-a.y;
		double d = (dx*(t.x-a.x)+dy*(t.y-a.y))/(dx*dx+dy*dy);
		double mx = a.x+d*dx, my = a.y+d*dy;
		p.Set( 2*mx-t.x, 2*my-t.y );
	}
	double NearPoint( Point t, Point &p ){
		SymPoint( t, p );
		p.Set( (t.x+p.x)/2, (t.y+p.y)/2 );
		if( p.Between(a,b) ) return t.Distance( p );
		p = ( fabs(p.x-a.x)+fabs(p.y-a.y)<fabs(p.x-b.x)+fabs(p.y-b.y) ) ? a:b;
		return t.Distance( p );
	}
	double MinX(){ return min( a.x, b.x ); }
	double MaxX(){ return max( a.x, b.x ); }
	double MinY(){ return min( a.y, b.y ); }
	double MaxY(){ return max( a.y, b.y ); }
	bool Exclude( Line t ){
		return MaxX()>=t.MinX() && MinX()<=t.MaxX() && MaxY()>=t.MinY() && MinY()<=t.MaxY();
	}
	bool CrossLi( Line t ){
		Point p1q1( a.x-t.a.x, a.y-t.a.y ), p2q1( b.x-t.a.x, b.y-t.a.y );
		Point q2q1( t.b.x-t.a.x, t.b.y-t.a.y );
		return ( p1q1.CrossPlot( q2q1) * p2q1.CrossPlot( q2q1 ) <= eps );
	}
	bool Intersect( Line t ){ return Exclude(t) && CrossLi(t) && t.CrossLi(*this); }
	bool Intersect( Point s, Point t, Point &p ){ // // 求两直线交点
		double d1 = CrossPlot( s, b, a ), d2 = CrossPlot( b, t, a );
		if( fabs(d1+d2)<eps ) return false;
		p.Set( (s.x*d2+t.x*d1)/(d1+d2), (s.y*d2+t.y*d1)/(d1+d2) );
		return true;
	}
	bool Intersect( Line t, Point &p ){ return Intersect( t.a, t.b, p ); }
};

class Circle{
public:
	Point a; double r;
	Circle( double X=0, double Y=0, double Z=0, double R=0 ){ a.Set(X,Y,Z); r=R; }
	Circle( Point s, double R ){ a = s; r = R; }
	void Set( double x, double y, double R ){ a.Set( x, y ); r = R; }
	double Intersect( Point t, Point &p ){ // 距离最近点
		double d = a.Distance(t);
		if( Equal(d,0) ) p.Set( a.x+r, a.y );
		else p.Set( a.x+r/d*(t.x-a.x), a.y+r/d*(t.y-a.y) );
		return d;
	}
	bool Intersect( Line s, Point &p1, Point &p2 ){
		if( s.Distance(a) > r+eps ) return false;
		if( Equal( s.a.x, s.b.x)){
			double tmp = sqrt( max( 0.0, r*r-(s.a.x-a.x)*(s.a.x-a.x) ) );
			p1.Set( s.a.x, a.y+tmp );
			p2.Set( s.a.x, a.y-tmp );
		}
		else{
			double k = s.Slope(), b = s.a.y-k*s.a.x;
			Equation( 1+k*k, 2*k*(b-a.y)-2*a.x, (b-a.y)*(b-a.y)+a.x*a.x-r*r, p1.x, p2.x );
			p1.y = k*p1.x+b; p2.y = k*p2.x+b;
		}
		return true;
	}
	bool Intersect( Circle t, Point &p1, Point &p2 ){
		if( a.Distance(t.a) > r+t.r+eps ) return false; // 外围，内部没考虑
		Line s;
		double X, Y, d = (r*r-a.x*a.x-a.y*a.y)-(t.r*t.r-t.a.x*t.a.x-t.a.y*t.a.y);
		if( Equal( a.x, t.a.x )){
			Y = d / (2*t.a.y-2*a.y);
			s.Set( -1, Y, 1, Y );
		}
		else if( Equal( a.y, t.a.y )){
			X = d / (2*t.a.x-2*a.x);
			s.Set( X, -1, X, 1 );
		}
		else{
			if( d<eps ) s.Set( 0, 0, 1, (a.x-t.a.x)/(t.a.y-a.y) );
			else s.Set( 0, d/(2*t.a.y-2*a.y), d/(2*t.a.x-2*a.x), 0 );
		}
		return Intersect( s, p1, p2 );
	}
	bool Intersect( Circle t, double &angle1, double &angle2 ){ // 余弦定理求角度
		double d = a.Distance( t.a );
		if( d>r+t.r-eps ) return false; // 排除相切
		if( d<r-t.r+eps ){ angle1 = angle2 = 0; return true; }
		if( d<t.r-r+eps ){ angle1 = 0; angle2 = 2*PI; return true; }
		double angle = atan2( t.a.y-a.y, t.a.x-a.x );
		double delta = acos( (r*r+d*d-t.r*t.r)/(2*r*d) );
		angle1 = angle-delta; angle2 = angle+delta;
		if( angle1<-eps ) angle1 += 2*PI;
		if( angle2<-eps ) angle2 += 2*PI;
		return true;
	}
	struct Node{
		double u, v;
		void set( double U, double V ){ u = U; v = V; }
	};
	static inline bool cmp( Node s, Node t ){ return s.u<t.u; }
	friend bool operator==(Circle s, Circle t){ return s.a.Equal(t.a) && Equal(s.r,t.r); }
	static void Area( Circle s, double angle1, double angle2, double &area1, double &area2 ){
		double angle = angle2-angle1;
		area1 += 0.5*s.r*s.r*( angle-sin(angle));
		Point a( s.a.x+s.r*cos(angle1), s.a.y+s.r*sin(angle1) );
		Point b( s.a.x+s.r*cos(angle2), s.a.y+s.r*sin(angle2) );
		area2 += a.CrossPlot( b );
	}
	static double AreaUnion( int n, Circle *p, Node *s, Node *t ){
		for(int i=0; i<n; i++){
			for(int j=0; j<n; j++){
				if( i==j ) continue;
				if( p[i]==p[j] || p[i].a.Distance(p[j].a)<=p[j].r-p[i].r+eps ){
					swap( p[i--], p[--n] ); break;
				}
			}
		}
		double angle1, angle2, area1 = 0, area2 = 0;
		for(int i=0; i<n; i++){
			int sn = 0, tn = 0;
			for(int j=0; j<n; j++){
				if( i!=j && p[i].Intersect( p[j], angle1, angle2 ) ){
					if( angle1>angle2 ){
						s[sn++].set( angle1, 2*PI );
						s[sn++].set( 0, angle2 );
					}
					else s[sn++].set( angle1, angle2 );
				}
			}
			sort( s, s+sn, cmp );

			double st = 0, end = 0;
			for(int j=0; j<sn; j++){
				if( end+eps<s[j].u ) t[tn++].set( end, s[j].u );
				end = max( end, s[j].v );
			}
			if( end<2*PI-eps ) t[tn++].set( end, 2*PI );
			if( t[tn-1].v>2*PI-eps && t[0].u<eps ){
				t[0].u = t[tn-1].u;
				if( t[0].u>eps ) t[0].u -= 2*PI;
				tn = max( tn-1, 1 );
			}
			for(int j=0; j<tn; j++) Area( p[i], t[j].u, t[j].v, area1, area2 );
		}
		return area1+fabs(area2)/2;
	}
	static double AreaIntersection( int n, Circle *p, Node *s, Node *t ){
		for(int i=0; i<n; i++){
			for(int j=0; j<n; j++){
				if( i==j ) continue;
				if( p[i].a.Distance(p[j].a)>p[i].r+p[j].r-eps ) return 0;
				if( p[i]==p[j] || p[i].a.Distance(p[j].a)<=p[i].r-p[j].r+eps ){
					swap( p[i--], p[--n] ); break;
				}
			}
		}
		double angle1, angle2, area1 = 0, area2 = 0;
		for(int i=0; i<n; i++){
			int sn = 0; s[sn++].set( 0, 2*PI );
			for(int j=0; j<n; j++){
				if( i==j ) continue;
				p[i].Intersect( p[j], angle1, angle2 );
				int tn = 0;
				for(int k=0; k<sn; k++){
					if( angle1>angle2 ){
						t[tn].u = max( s[k].u, angle1 );
						t[tn++].v = s[k].v;
						t[tn].u = s[k].u;
						t[tn++].v = min( s[k].v, angle2 );
					}
					else{
						t[tn].u = max( s[k].u, angle1 );
						t[tn++].v = min( s[k].v, angle2 );
					}
				}
				swap( s, t ); sn = tn;
			}
			for(int j=0; j<sn; j++) if( s[j].u>s[j].v ) swap( s[j--], s[--sn] );
			if( s[sn-1].v>2*PI-eps && s[0].u<eps ){
				s[0].u = s[sn-1].u;
				if( s[0].u>eps ) s[0].u -= 2*PI;
				sn = max( sn-1, 1 );
			}
			for(int j=0; j<sn; j++) Area( p[i], s[j].u, s[j].v, area1, area2 );
		}
		return area1+fabs(area2)/2;
	}
	// hdu 3467  Song of the Siren
	static void ans(){ // hdu 3239  Jiajia's Robot
		const int N = 308;
		Point a, b, c, d;
		int cs = 0, n = 4; Circle p1[N], p2[N]; Node s[N], t[N];
		freopen("E:\\Program\\contest\\09 武汉\\J\\TestData\\J.in","r",stdin);
		freopen("E:\\Program\\contest\\09 武汉\\J\\TestData\\J.txt","w",stdout);
		while( scanf("%lf%lf%lf%lf",&a.x,&a.y,&b.x,&b.y)==4 ){
			scanf("%lf%lf%lf%lf",&c.x,&c.y,&d.x,&d.y);
			if( a.x==0 && a.y==0 && b.x==0 && b.y==0 ) break;
			p1[0].Set( (a.x+c.x)/2, (a.y+c.y)/2, a.Distance(c)/2 );
			p1[1].Set( (a.x+d.x)/2, (a.y+d.y)/2, a.Distance(d)/2 );
			p1[2].Set( (b.x+c.x)/2, (b.y+c.y)/2, b.Distance(c)/2 );
			p1[3].Set( (b.x+d.x)/2, (b.y+d.y)/2, b.Distance(d)/2 );
			
			p2[0].Set( (a.x+c.x)/2, (a.y+c.y)/2, a.Distance(c)/2 );
			p2[1].Set( (a.x+d.x)/2, (a.y+d.y)/2, a.Distance(d)/2 );
			p2[2].Set( (b.x+c.x)/2, (b.y+c.y)/2, b.Distance(c)/2 );
			p2[3].Set( (b.x+d.x)/2, (b.y+d.y)/2, b.Distance(d)/2 );
			printf("Case %d: %.3f\n\n",++cs,AreaUnion(n,p1,s,t)-AreaIntersection(n,p2,s,t));
		}
	}
	/*static bool Solve( int n, Circle *p, Node *P, double *ok ){
		double angle1, angle2;
		for(int i=0; i<=n; i++){
			if( ok[i]<=p[i].r ) continue;
			int pn = 0;
			for(int j=0; j<=n; j++){
				if( i!=j && p[i].Intersect( p[j], angle1, angle2 ) ){
					if( j==0 ){
						if( angle1!=angle2 ) swap( angle1, angle2 );
						else{ P[pn++].set( 0, 2*PI ); continue; }
					}
					if( angle1>angle2+eps ){
						P[pn++].set( angle1, 2*PI );
						P[pn++].set( 0, angle2 );
					}
					else P[pn++].set( angle1, angle2 );
				}
			}
			double s = 2*PI, t = 0;
			if( pn>0 ) sort( P, P+pn, cmp );
			for(int j=0; j<pn; j++){
				if( t+eps < P[j].u ) return false;
				s = min( s, P[j].u );
				t = max( t, P[j].v );
			}
			if( (eps<s || t<2*PI-eps) ) return false;
			ok[i] = min( ok[i], p[i].r );
		}
		return true;
	}
	static void ans(){ // ural 1520  Empire strikes back
		const int N = 308;
		int n, R; Circle p[N]; Node P[2*N]; double ok[N];
		while( scanf("%d%d",&n,&R)==2 ){
			p[0].a.Set( 0, 0 ); p[0].r = R;
			double l = 0, r = 2*R, mid;
			for(int i=1; i<=n; i++){
				scanf("%lf%lf",&p[i].a.x,&p[i].a.y);
				mid = p[i].a.Distance( p[0].a );
				l = min( l, R-mid );
				r = min( r, R+mid );
			}
			for(int i=0; i<=n; i++) ok[i] = r;
			while( l+eps<r ){
				mid = (l+r)/2;
				for(int i=1; i<=n; i++) p[i].r = mid;
				if( Solve( n, p, P, ok ) ) r = mid;
				else l = mid;
			}
			printf("%.8f\n",r);
		}
	}*/
	/*struct Node{
		double y; int id; bool flag;
		bool operator<(const Node &t)const{ return y<t.y; }
	};
	static void Solve( double x, int n, Circle *p, bool *vis ){
		vector<Node> v; Node t;
		for(int i=0; i<n; i++){
			double d = fabs(p[i].a.x-x);
			if( d >= p[i].r ) continue;
			t.y = p[i].a.y-sqrt(p[i].r*p[i].r-d*d); t.id = -i; t.flag = 0;
			v.push_back( t );
			t.y = p[i].a.y+sqrt(p[i].r*p[i].r-d*d); t.id = -i; t.flag = 1;
			v.push_back( t );
		}
		sort( v.begin(), v.end() );
		set<int> ID;
		for(int i=0; i<v.size(); i++){
			if( v[i].flag==0 ) ID.insert( v[i].id );
			else ID.erase( v[i].id );
			if( ID.size() ) vis[-*ID.begin()] = true;
		}
	}
	static void ans(){ // poj 1418  Viva Confetti
		int n; Circle p[101]; double x[30000]; bool vis[101];
		while( scanf("%d",&n)==1 && n ){
			for(int i=0; i<n; i++) scanf("%lf%lf%lf",&p[i].a.x,&p[i].a.y,&p[i].r);
			int xn = 0;
			Point p1, p2;
			for(int i=0; i<n; i++){
				x[xn++] = p[i].a.x-p[i].r;
				x[xn++] = p[i].a.x+p[i].r;
				for(int j=i+1; j<n; j++){
					if( p[i].Intersect(p[j],p1,p2) ){
						x[xn++] = p1.x; x[xn++] = p2.x;
					}
				}
			}
			sort( x, x+xn );
			memset( vis, 0, sizeof(bool)*n );
			for(int i=1; i<xn; i++){
				if( x[i]==x[i-1] ) continue;
				Solve( (x[i-1]+x[i])/2, n, p, vis );
			}
			int num = 0;
			for(int i=0; i<n; i++) if( vis[i] ) num++;
			printf("%d\n",num);
		}
	}*/
};

class Oval{
public:
	Point f1, f2; double r; // 焦点、距离（长轴2倍）
	Oval( Point F1, Point F2, double R ){ Set( F1, F2, R ); }
	void Set( Point F1, Point F2, double R ){ f1 = F1; f2 = F2; r = R; }
	// 求椭圆 x^2/a^2+y^2/b^2=1 与直线 y=kx+d 的交点
	static bool Intersect( double a2, double b2, Line s, Point &p1, Point &p2 ){
		if( Equal( s.a.x, s.b.x ) ){
			double y = b2*(1-s.a.x*s.a.x/a2);
			if( y < -eps ) return false;
			y = sqrt( max( 0.0, y ) );
			p1.Set( s.a.x, y ); p2.Set( s.a.x, -y);
		}else{
			double k = s.Slope(), d = s.a.y-k*s.a.x;
			if( !Equation( b2+a2*k*k, 2*k*d*a2, a2*(d*d-b2), p1.x, p2.x ) ) return false;
			if( !Equation( b2+a2*k*k, -2*d*b2, b2*(d*d-a2*k*k), p1.y, p2.y ) ) return false;
			if( fabs(p2.y-k*p1.x-d)<fabs(p1.y-k*p1.x-d) ) swap( p1.y, p2.y );
			//p1.y = k*p1.x+d; p2.y = k*p2.x+d;
		}
		return true;
	}
	bool Intersect( Line s, Point &p1, Point &p2 ){
		double d = f1.Distance(f2)/2, X, Y;
		double R11, R12, R13 = (f1.x+f2.x)/2, R21, R22, R23 = (f1.y+f2.y)/2;
		R11 = (f1.x-R13)/(-d); R21 = (f1.y-R23)/(-d);
		R12 = -R21; R22 = R11;
		// 转置
		swap( R12, R21 );
		X = -(R13*R11+R23*R12); Y = -(R13*R21+R23*R22);
		R13 = X; R23 = Y;

		Point s1( s.a.x*R11+s.a.y*R12+R13, s.a.x*R21+s.a.y*R22+R23 );
		Point s2( s.b.x*R11+s.b.y*R12+R13, s.b.x*R21+s.b.y*R22+R23 );
		if( !Intersect( r*r/4, r*r/4-d*d, Line(s1,s2), p1, p2 ) ) return false;

		swap( R12, R21 );
		X = -(R13*R11+R23*R12); Y = -(R13*R21+R23*R22);
		p1.Set( p1.x*R11+p1.y*R12+X, p1.x*R21+p1.y*R22+Y );
		p2.Set( p2.x*R11+p2.y*R12+X, p2.x*R21+p2.y*R22+Y );
		return true;
	}
	static void ans(){  // sgu 233  The Greatest Angle
		int T; Circle s; Point a, b, m, c;
		scanf("%d",&T);
		while( T-- ){
			scanf("%lf%lf%lf",&s.a.x,&s.a.y,&s.r);
			scanf("%lf%lf%lf%lf",&a.x,&a.y,&b.x,&b.y);
			double x0 = s.a.x, y0 = s.a.y;
			s.a.x = s.a.y = 0; a.x -= x0; b.x -= x0; a.y -= y0; b.y -= y0;
			
			m.Set( (a.x+b.x)/2, (a.y+b.y)/2 );
			c.Set( m.x+b.y-a.y, m.y+a.x-b.x );
			
			Oval t( (s.a.Equal(a) ? b:a), s.a, s.r );
			Point q1, q2, result;
			t.Intersect( Line(m,c), q1, q2 );

			if( q1.Distance2(m)<q2.Distance2(m) ) result = q1;
			else result = q2;
			s.Intersect( Line(s.a,result), q1, q2 );
			if( q1.Distance2(m)<q2.Distance2(m) ) result = q1;
			else result = q2;

			printf("%.6f %.6f\n",result.x+x0,result.y+y0);
		}
	}
};

class Triangle{
public:
	Point a, b, c;
	Triangle(){ a.x = a.y = b.x = b.y = c.x = c.y = 0; }
	Triangle( Point A, Point B, Point C){ a = A; b = B; c = C; }
	double Area( double bc=-1, double ac=-1, double ab=-1 ){
		double A = ( bc==-1 ? b.Distance(c):bc );
		double B = ( ac==-1 ? a.Distance(c):ac );
		double C = ( ab==-1 ? a.Distance(b):ab );
		double P = (A+B+C)/2;
		return sqrt( P*(P-A)*(P-B)*(P-C) );
	}
	void Plumb( Point &p ){ // ChuiXin
		double pa = (b.x-a.x)*(c.x-b.x)*(c.x-a.x)+b.y*(c.y-a.y)*(c.x-b.x)-a.y*(c.y-b.y)*(c.x-a.x);
		double pb = (c-b)*(c-a);
		p.y = pa/pb;
		if( !Equal(c.x, b.x)) p.x = -(p.y-a.y)*(c.y-b.y)/(c.x-b.x)+a.x;
		else if( !Equal(c.x, a.x)) p.x = -(p.y-b.y)*(c.y-a.y)/(c.x-a.x)+b.x;
	}
	double Incenter( Point &p ){ // NeiXin
		double A = b.Distance( c ), B = a.Distance( c ), C = a.Distance( b );
		p.x = (A*a.x+B*b.x+C*c.x)/(A+B+C);
		p.y = (A*a.y+B*b.y+C*c.y)/(A+B+C);
		double P = (A+B+C)/2;
		return sqrt( (P-A)*(P-B)*(P-C)/P );
	}
	void Barycenter( Point &p ){ // ZhongXin
		p.Set( (a.x+b.x+c.x)/3, (a.y+b.y+c.y)/3 );
	}
	double Circumcenter( Point &p ){ // WaiXin
		double k1 = a.Slope( c ), k2 = a.Slope( b );
		if( Equal(k1,0)) p.Set( (a.x+c.x)/2, (-1/k2)*(p.x-(a.x+b.x)/2)+(a.y+b.y)/2 );
		else if( Equal( k2, 0 )){
			p.Set( (a.x+b.x)/2, (-1/k1)*(p.x-(a.x+c.x)/2)+(a.y+c.y)/2 );
		}
		else{
			p.x = ( (b.y-c.y)/2+(a.x+b.x)/(2*k2)-(a.x+c.x)/(2*k1) )/(1/k2-1/k1);
			p.y = -1/k1*(p.x-(a.x+c.x)/2)+(a.y+c.y)/2;
		}
		double A = b.Distance( c ), B = a.Distance( c ), C = a.Distance( b );
		return A*B*C/(4*Area( A, B, C ));
	}
};

class Polygon{
public:
	static bool cmp( Point s, Point t ){ return (s.x<t.x || (s.x==t.x&&s.y<t.y) ); }
	static bool cmpy( Point s, Point t ){ return s.y<t.y; }
	static bool cmpd( Point s, Point t){
		double r = (t-first)*(s-first);
		return ( r+eps<0 || ( fabs(r)<eps && first.Distance(s)<first.Distance(t)) );
	}
	static double MinDistance( int l, int r, Point *p, Point *v ){
		double res = 1e40;
		if( r-l < 6 ){
			for(int i=l; i<r; i++){
				for(int j=i+1; j<r; j++){
					//res = min( res, p[i].Distance( p[j] ) );
					if( p[i].z != p[j].z ) res = min( res, p[i].Distance( p[j] ) );
				}
			}
		}
		else{
			int mid = (l+r)/2, len = 0;
			res = min( MinDistance( l, mid, p, v ), MinDistance( mid, r, p, v ) );
			for(int i=mid; i>=l; i--){
				if( p[mid].x-p[i].x<=res ) v[len++] = p[i];
				else break;
			}
			for(int i=mid+1; i<r; i++){
				if( p[i].x-p[mid].x<=res ) v[len++] = p[i];
				else break;
			}
			sort( v, v+len, cmpy );
			for(int i=0; i<len; i++){
				/*for(int j=i+1; j<i+8 && j<len; j++){
					res = min( res, v[i].Distance( v[j] ) );
				}*/
				for(int j=i+1; j<len && v[j].y<=v[i].y+res; j++){
					if( v[i].z!=v[j].z ) res = min( res, v[i].Distance( v[j] ) );
				}
			}
		}
		return res;
	}
	static void ans(){ // poj 3714 Raid
		int t, n;
		scanf("%d",&t);
		const int N = 200008;
		Point *p = new Point[2*N], *v = p+N;
		while( t-- ){
			scanf("%d",&n);
			for(int i=0; i<n; i++){ scanf("%lf%lf",&p[i].x,&p[i].y); p[i].z = 0; }
			for(int i=n; i<2*n; i++){ scanf("%lf%lf",&p[i].x,&p[i].y); p[i].z = 1; }
			n += n;
			sort( p, p+n, cmp );
			printf("%.3lf\n", MinDistance( 0, n, p, v ));
		}
		delete []p;
	}
	static int Graham( int n, Point *p, Point *v, int *id = NULL ){
		int low = 0;
		for(int i=1; i<n; i++){
			double t = p[i].y-p[low].y;
			if( (fabs(t)<eps && p[i].x<p[low].x) || t<0 ) low = i;
		}
		first = p[low]; p[low] = p[0]; p[0] = first;
		sort( p+1, p+n, cmpd );
		int nv = min( n, 2 );
		v[0] = p[0], v[1] = p[1];
		if( id ){ id[0] = 0; id[1] = 1; }
		for(int i=2; i<n; i++){
			while( nv>=2 && (p[i]-v[nv-2])*(v[nv-1]-v[nv-2])>=0 ) nv--;
			if( id ) id[nv] = i;
			v[nv++] = p[i];
		}
		return nv;
	}
	static double Angle( Point s, Point t ){
		double angle = atan2( t.y-s.y, t.x-s.x );
		return angle+eps<0 ? angle+2*PI:angle;
	}
	static double MaxDistance( int n, Point *p, Point *v ){
		n = Graham( n, p, v );
		int k = n-1;
		while( k>1 && v[k].y<=v[k-1].y+eps ) k--;
		double res = -1, alph = 0;
		for(int s=0,t=k; alph<=PI+eps && s<=k && s<n-1; ){
			res = max( res, v[s].Distance( v[t] ) );
			double a1 = fabs( Angle( v[s], v[s+1] )-alph );
			double a2 = fabs( Angle( v[t], v[(t+1)%n] )-alph-PI );
			if( a1 < a2 ){ alph += a1; s++; }
			else{ alph += a2; t=(t+1)%n; }
		}
		return res;
	}
	/*static void ans(){ // poj 2187 Beauty Contest
		const int N = 200008;
		int n; Point *p = new Point[2*N], *v = p+N;
		while( scanf("%d",&n)==1 ){
			for(int i=0; i<n; i++) scanf("%lf%lf",&p[i].x,&p[i].y);
			double d = MaxDistance( n, p, v );
			printf("%.0lf\n",d*d);
		}
		delete []p;
	}*/
	static double MinDistance( int n, Point *p, int m, Point *v ){
		Point *P = new Point[n+m], *V = P+n, pt;  Line st;
		n = Graham( n, p, P ); m = Graham( m, v, V );
		int k = m-1;
		while( k>1 && V[k].y<=V[k-1].y+eps ) k--;
		double res = P[0].Distance( V[k] ), alph = 0;
		for(int s=0,t=k; alph<=2*PI-eps; ){
			double a1 = fabs( Angle( P[s], P[(s+1)%n] )-alph );
			double a2 = Angle( V[t], V[(t+1)%m] );
			if( a2 <PI ) a2 += 2*PI;
			a2 = fabs( a2-alph-PI );

			if( a1 < a2 ){
				st.Set( P[s], P[(s+1)%n] );
				res = min( res, st.NearPoint( V[t], pt ) );
				alph += a1; s=(s+1)%n;
			}
			else{
				st.Set( V[t], V[(t+1)%m] );
				res = min( res, st.NearPoint( P[s], pt ) );
				alph += a2; t=(t+1)%m;
			}
		}
		delete []P;
		return res;
	}
	/*static void ans(){ // poj 3608 Bridge Across Islands
		const int N = 10008;
		Point p[N], v[N];
		int n, m;
		while( scanf("%d%d",&n,&m)==2 && n+m!=0 ){
			for(int i=0; i<n; i++) scanf("%lf%lf",&p[i].x,&p[i].y);
			for(int i=0; i<m; i++) scanf("%lf%lf",&v[i].x,&v[i].y);
			printf("%.5lf\n", MinDistance( n, p, m, v ) );
		}
	}*/
	static double Area( int i, int j, int k, Point *v){
		return fabs( v[i].x*v[j].y+v[j].x*v[k].y+v[k].x*v[i].y
			-v[j].x*v[i].y-v[k].x*v[j].y-v[i].x*v[k].y)/2;
	}
	static double Area( int nv, Point *v ){
		double res = 0;
		for(int i=2; i<nv; i++) res += Area( 0, i-1, i, v );
		return res;
	}
	static double Perimeter( int nv, Point *v ){
		double res = v[0].Distance( v[nv-1] );
		for(int i=1; i<nv; i++) res += v[i-1].Distance( v[i] );
		return res;
	}
	static double Triangle( int nv, Point *v ){
		double res = -1;
		for(int i=0; i<=nv-3; i++){
			for(int j=i+1, k=-1; j<=nv-2; j++){
				if( k<=j ) k = j+1;
				double s = Area( i, j, k, v );
				res = max( res, s );
				while( k+1 <= nv-1 ){
					double t = Area( i, j, k+1, v );
					if( t < s ) break;
					res = max( res, t );
					s = t; k++;
				}
			}
		}
		return res;
	}
	/*static void ans(){ //poj 1228 Grandpa's Estate
		int t, n;
		scanf("%d",&t);
		Point p[1008], v[1008];
		while( t-- ){
			scanf("%d",&n);
			for(int i=0; i<n; i++) scanf("%lf%lf",&p[i].x, &p[i].y);
			if( n <= 5 ){ printf("NO\n"); continue; }
			int nv = Graham( n, p, v );
			if( nv <= 2 ){ printf("NO\n"); continue; }
			int i, *vis = new int[nv];
			memset( vis, 0, sizeof(int)*nv );
			for(i=0; i<nv; i++){
				Line r( v[i].x, v[i].y, v[(i+1)%nv].x, v[(i+1)%nv].y );
				for(int j=0; j<n; j++){
					if( r.PointOn( p[j] ) ) vis[i]++;
				}
				if( vis[i] < 3 ) break;
			}
			delete []vis;
			if( i < nv ) printf("NO\n");
			else printf("YES\n");
		}
	}*/
	/*struct dpoint{ char s[21], t[21]; Point p; };
	static bool dmp( dpoint s, dpoint t){ return cmpd( s.p, t.p ); }
	static void ans(){ // poj 3355 Boundary Points
		Point tp[10008], v[10008];
		dpoint *p = new dpoint[10008];
		while( true ){
			char c, s[51];
			int n = 0, q = 0;
			while( (c=getchar())!='\n' && c!=EOF){
				if( c==')' ){
					s[q] = '\0';
					int i, j = strlen(s);
					for(i=0; i<j; i++) if( s[i]==',') break;
					memcpy( p[n].s, s, sizeof(char)*i );
					p[n].s[i] = '\0';
					memcpy( p[n].t, s+i+1, sizeof(char)*(j-i-1) );
					p[n].t[j-i-1] = '\0';

					p[n].p.x = atof( p[n].s );
					p[n].p.y = atof( p[n].t );
					q = 0; n++;
				}
				else if( (c<='9'&&c>='0') || c=='.' || c==',' || c=='-' ) s[q++] = c;
			}
			if( c == EOF ) break;
			int low = 0;
			for(int i=1; i<n; i++){
				double t = p[i].p.y-p[low].p.y;
				if( (fabs(t)<eps && p[i].p.x<p[low].p.x) || t<0 ) low = i;
			}
			dpoint tmp;
			tmp = p[low]; p[low] = p[0]; p[0] = tmp;
			first = p[0].p;
			sort(p, p+n, dmp );

			for(int i=0; i<n; i++){
				tp[i].x = p[i].p.x;
				tp[i].y = p[i].p.y;
			}
			int *id = new int[n];
			n = Graham( n, tp, v, id );
			for(int i=0; i<n; i++) printf("(%s,%s) ", p[id[i]].s,p[id[i]].t);
			printf("(%s,%s)\n",p[id[0]].s,p[id[0]].t);
			delete []id;
		}
		delete []p;
	}*/
	static bool PointIn( int n, Point *p, Point t ){
		double angle = 0;
		Point a, b;
		for(int i=0; i<n; i++){
			a = p[i]-t; b = p[(i+1)%n]-t;
			if( fabs(a*b)<eps ) continue;
			angle += atan2( a*b, a.PointPlot(b) );
		}
		return fabs(angle)>PI; // should be only 0 or 2*PI ...
	}
	// -1: out  0: On   1: in
	static int PointOn( Point s, int n, Point *p ){
		int low = 0, up = 0, odd = 0;
		for(int i=n-1,j=0; j<n; i=j++){
			low = i, up = j;
			if( p[low].y>p[up].y ) swap( low, up );
			if( p[up].y>s.y && p[low].y<=s.y ){
				double cross = (p[low].x-s.x)*(p[up].y-s.y)
					-(p[up].x-s.x)*(p[low].y-s.y);
				if( cross==0 && p[low].y<=s.y && s.y<=p[up].y ) return 0;
				else if( cross>0 ) odd ^= 1;
			}
			else if( p[up].y==s.y && p[low].y==s.y ){
				if( min(p[low].x,p[up].x)<=s.x && s.x<=max(p[low].x,p[up].x) )
					return 0;
			}
		}
		return odd ? 1:-1;
	}
};

int main(){
	Circle::ans();
	//Oval::ans();
	//Polygon::ans();
	// nkoj 1204 / poj 1066 Treasure Hunt
	// poj 2653 Pick-up sticks
	// poj 3711 Mayan Pyramid
	// poj 3596  Illuminated Planet
	return 0;
}
