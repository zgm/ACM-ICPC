#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<cmath>
#include<math.h>
#include<xutility>
//#include<iostream>
using namespace std;

class SAA{ // Simulated annealing algorithm
public:
	/*struct Point{
		double x, y;
		double distance( double s, double t ){
			return sqrt((x-s)*(x-s)+(y-t)*(y-t));
		}
		double distance( Point s ){ return distance(s.x,s.y); }
	};
	// a simple way to generate [0,1]
	static double Rand(){ return rand()/(double)RAND_MAX; }
	static void ans(){ // poj 1379 Run Away
		const int N = 1008, NP = 40, L = 30;
		int t, n, m, k;
		Point p[N], q[NP]; double s[NP];
		scanf("%d",&t);
		//srand( 1234567 );
		srand( (unsigned)time(NULL) );
		while( t-- ){
			scanf("%d%d%d",&n,&m,&k);
			for(int i=0; i<k; i++) scanf("%lf%lf",&p[i].x,&p[i].y);
			for(int i=0; i<NP; i++){
				q[i].x = Rand()*n;
				q[i].y = Rand()*m;
				s[i] = 1e10;
				for(int j=0; j<k; j++) s[i] = min( s[i], q[i].distance(p[j]) );
			}

			double delta = max(n,m)/sqrt((double)k);
			double d = 0; int num = 0;
			while( delta>1e-2 && num<20 ){
				double dd = 0;
				for(int i=0; i<NP; i++){
					for(int j=0; j<L; j++){
						double dx = Rand()*delta;
						double dy = sqrt(delta*delta-dx*dx);
						double x = q[i].x+(rand()%2 ? dx:-dx );
						double y = q[i].y+(rand()%2 ? dy:-dy );
						if( 0<=x && x<=n && 0<=y && y<=m ){
							double d = 1e10;
							for(int r=0; r<k; r++) d = min( d, p[r].distance(x,y) );
							if( d>s[i] ){ q[i].x = x; q[i].y = y; s[i] = d; }
						}
					}
					dd = max( dd, s[i] );
				}
				if( d<dd ){ d = dd; num = 0; }
				else num++;
				delta *= 0.8;
			}

			for(int i=0; i<NP; i++){
				if( d!=s[i] ) continue;
				printf("The safest point is (%.1f, %.1f).\n",q[i].x,q[i].y);
				break;
			}
		}
	}*/
	struct Point{
		double x, y, z;
		double distance( double s, double t, double r ){
			return sqrt((x-s)*(x-s)+(y-t)*(y-t)+(z-r)*(z-r));
		}
		double distance( Point s ){ return distance(s.x,s.y,s.z); }
	};
	static void ans(){ // poj 2069  Super Star
		int n; Point q, p[100];
		while( scanf("%d",&n)==1 && n ){
			for(int i=0; i<n; i++) scanf("%lf%lf%lf",&p[i].x,&p[i].y,&p[i].z);
			double res = 1e20;
			q.x = q.y = q.z = 0;
			for(double step=40,eps=1e-6; step>eps; step*=0.98){
				double md = -1; int id;
				for(int i=0; i<n; i++){
					double d = q.distance(p[i]);
					if( md<d ){ md = d; id = i; }
				}
				if( res>md ) res = md;
				q.x += (p[id].x-q.x)/md*step;
				q.y += (p[id].y-q.y)/md*step;
				q.z += (p[id].z-q.z)/md*step;
			}
			printf("%.5f\n",res);
		}
	}
};

int main(){
	SAA::ans();
	return 0;
}