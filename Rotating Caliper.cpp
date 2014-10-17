//#define _CRT_SECURE_NO_DEPRECATE
//#pragma warning(disable:4996)
#include<cstdio>
#include<cmath>
#include<cstring>
#include<vector>
#include<set>
#include<algorithm>
#include<iostream>
using namespace std;

const double eps = 1e-6, PI = 3.1415926535897932384626433832795, INF = 1e30;


struct Link{
	int u, v, next;
	void set( int U, int V, int Next){ u=U; v=V; next=Next; }
};
class List{
public:
	int end; Link *V;
	List( int size = 1024 ){ V = new Link[size]; }
	~List(){ delete []V; }
	void init( int n ){ end = n; for(int i=0; i<end; i++) V[i].next = -1; }
	void insert( int u, int v ){
	    V[end].set( u, v, V[u].next );
		V[u].next = end++;
	}
};

class Point{
public:
	double x, y, z;
	Point( double X=0, double Y=0, double Z=0 ){ Set(X,Y,Z); }
	void Set( double X, double Y, double Z=0 ){ x = X; y = Y; z = Z; }
	void Set( Point t){ Set( t.x, t.y, t.z ); }
	double Distance( Point t){ return sqrt( (x-t.x)*(x-t.x)+(y-t.y)*(y-t.y) ); }
	double Distance2( Point t){ return (x-t.x)*(x-t.x)+(y-t.y)*(y-t.y); }

	friend Point operator -( Point s, Point t){ return Point(s.x-t.x,s.y-t.y,s.z-t.z); }
	friend Point operator +( Point s, Point t){ return Point(s.x+t.x,s.y+t.y,s.z+t.z); }
	friend double operator^( Point s, Point t){ return s.x*t.x+s.y*t.y; }
    friend double operator*( Point s, Point t){ return s.x*t.y-s.y*t.x; }
	friend double CrossPlot( Point a, Point b, Point c ){
		return (b.x-a.x)*(c.y-a.y)-(b.y-a.y)*(c.x-a.x);
	}
}first;

class Polygon{
public:
	static bool cmp( Point s, Point t ){ return (s.x<t.x || (s.x==t.x&&s.y<t.y) ); }
	static bool cmpy( Point s, Point t ){ return s.y<t.y; }
	static bool cmpd( Point s, Point t){
		double r = (t-first)*(s-first);
		return ( r+eps<0 || ( fabs(r)<eps && first.Distance2(s)<first.Distance2(t)) );
	}
    static Point Barycenter( int n, Point *p ){ // ZhongXin
        Point s, t; double A = 0, a;
        p[n] = p[0];
        for(int i=0; i<n; i++){
            t = p[i]+p[i+1];
            a = p[i].CrossPlot( p[i+1] );
            s.x += t.x*a/3;
            s.y += t.y*a/3;
            A += a;
        }
        s.x /= A; s.y /= A;
        return s;
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
    struct Node{
        double angle; int u, v;
        void set( double Angle, int U, int V ){ angle = Angle; u = U; v = V; }
    };
    static bool cmpN( Node s, Node t ){ return s.angle<t.angle; }
    static int getint() {
    	int ch, ret = 0, mark = 0;
	    while (((ch = getchar()) < '0' || ch > '9') && ch != '-');
	    if (ch == '-') {
		    mark = true;
		    ch = getchar();
	    }
	    while (ch >= '0' && ch <= '9') {
		    ret = ret * 10 + ch - '0';
		    ch = getchar();
	    }
	    return mark ? -ret : ret;
    }
    static void ans(){ // hdu 3702  The Covering Polygon   tju 3248  Ginkgo
        const int N = 508, M = 200008;
        int T, n, m, d[N][N], dis[N], q[10*N];
        Point x[N], *y = new Point[2*M], *p = y+M;
        Node *s = new Node[N*N]; List st( 300000 );
        scanf("%d",&T);
        while( T-- ){
            scanf("%d%d",&n,&m);
            for(int i=0; i<n; i++){
                x[i].x = getint(); x[i].y = getint();
                //scanf("%lf%lf",&x[i].x,&x[i].y);
            }
            for(int i=0; i<m; i++){
                y[i].x = getint(); y[i].y = getint();
                //scanf("%lf%lf",&y[i].x,&y[i].y);
            }
            //for(int i=0; i<n; i++) scanf("%lf%lf",&x[i].x,&x[i].y);
            int nv = Graham( m, y, p ); p[nv] = p[0];
            //for(int i=0; i<nv; i++) printf("%.2f %.2f\n",p[i].x,p[i].y);
            memset( d, -1, sizeof(d) );
            /*for(int i=0; i<n; i++){
                for(int j=0; j<n; j++){
                    if( i==j ) continue;
                    bool ok = true;
                    for(int k=0; k<nv; k++){
                        double val = (x[j]-x[i])*(p[k]-x[i]);
                        if( (fabs(val)<eps && !p[k].Between(x[i],x[j])) || val<0 ){
                            ok = false; break;
                        }
                    }
                    if( ok ) d[i][j] = 1;
                }
            }*/
            int sn = 0;
            for(int i=0; i<n; i++){
                for(int j=i+1; j<n; j++){
                    double angle = atan2( x[j].y-x[i].y, x[j].x-x[i].x );
                    if( angle<0 ) angle += 2*PI;
                    s[sn++].set( angle, i, j );
                    angle += PI;
                    if( angle>=2*PI ) angle -= 2*PI;
                    s[sn++].set( angle, j, i );
                }
            }
            sort( s, s+sn, cmpN );
            double angle = atan2( p[1].y-p[0].y, p[1].x-p[0].x );
            st.init( n );
            for(int i=0,j=0; j<sn; ){
                int u = s[j].u, v = s[j].v;
                if( i==nv ){
                    if( (x[v]-x[u])*(p[i]-x[u])>=0 ) d[u][v] = 1;
                    j++;
                }else{
                    if( angle<s[j].angle-eps ){
                        i++;
                        if( i<nv ) angle = atan2( p[i+1].y-p[i].y, p[i+1].x-p[i].x );
                        if( angle<0 ) angle += 2*PI;
                    }
                    else{
                        if( (x[v]-x[u])*(p[i]-x[u])>=0 ) st.insert( u, v );
                        j++;
                    }
                }
            }
            int result = n+1;
            for(int k=0; k<n; k++){
                memset( dis, -1, sizeof(int)*n );
                int head = 0, tail = 0;
                for(int i=st.V[k].next; i!=-1; i=st.V[i].next){
                    q[tail++] = st.V[i].v;
                    dis[st.V[i].v] = 1;
                }
                while( head<tail ){
                    int u = q[head++];
                    if( dis[u]>=result ) continue;
                    for(int i=st.V[u].next; i!=-1; i=st.V[i].next){
                        int v = st.V[i].v;
                        if( dis[v]==-1 || dis[v]>dis[u]+1 ){
                            dis[v] = dis[u]+1;
                            q[tail++] = v;
                        }
                    }
                }
                if( dis[k]!=-1 ) result = min( result, dis[k] );
            }
            /*for(int k=0; k<n; k++){
                for(int i=0; i<n; i++){
                    if( d[i][k]==-1 || d[i][k]>=result ) continue;
                    for(int j=0; j<n; j++){
                        if( d[k][j]==-1 ) continue;
                        if( d[i][j]==-1 || d[i][j]>d[i][k]+d[k][j] ){
                            d[i][j] = d[i][k]+d[k][j];
                        }
                    }
                }
                for(int i=0; i<n; i++)
                    if( d[i][i]!=-1 ) result = min( result, d[i][i] );
            }*/
            /*if( result==2 ){
                bool ok = false;
                for(int i=2; i<n; i++){
                    if( (x[i]-x[0])*(x[1]-x[0])!=0 ){ ok = true; break; }
                }
                if( ok ) result = 3;
                else result = n+1;
            }*/
            if( result>=n+1 ) printf("-1\n");
            else printf("%d\n",result);
        }
    }
};

int main(){
	Polygon::ans();
	return 0;
}
