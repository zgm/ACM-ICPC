#include<iostream>
#include<algorithm>
#include<set>
#include<cmath>
using namespace std;
const double eps = 1e-10;
const int N = 50008;

struct Point{ int x, y, r; }p[N];
bool cmpy( int a, int b ){ return (p[a].y==p[b].y&&p[a].x<p[b].x) || p[a].y<p[b].y; }
bool cmpl( int a, int b ){ return p[a].x-p[a].r<p[b].x-p[b].r; }
bool cmpr( int a, int b ){ return p[a].x+p[a].r<p[b].x+p[b].r; }

int n, L[N], R[N], ID[N], tmp[N];
double mid;
set<int> tree;
set<int>::iterator it;

#define Square(v) ((double)(v)*(v))
bool Collid( int a, int b ){
	a = tmp[a]; b = tmp[b];
	if( Square(p[a].x-p[b].x)+Square(p[a].y-p[b].y)<=Square(p[a].r+p[b].r+mid+mid) ) return true;
	return false;
}
bool Insert( int v ){
	it = tree.insert(v).first;
	if( it!=tree.begin() ){
		if( Collid( v, *--it) ) return false;
		++it;
	}
	if( ++it!=tree.end() && Collid(v,*it) ) return false;
	return true;
}
bool Remove( int v ){
	it = tree.find(v);
	if( it!=tree.begin() && it!=--tree.end() ){
		int a = *--it;
		++it;
		int b = *++it;
		if( Collid(a,b) ) return false;
	}
	tree.erase(v);
	return true;
}
bool Ok( ){
	tree.clear();
	int l = 0, r = 0;
	while( l<n || r<n ){
		if( l==n ){
			if( !Remove( ID[R[r++]] ) ) return false;
		}
		else if( r==n ){
			if( !Insert( ID[L[l++]] ) ) return false;
		}
		else if( p[L[l]].x-p[L[l]].r-mid-p[R[r]].x+p[R[r]].r-mid < eps ){
			if( !Insert( ID[L[l++]] ) ) return false;
		}
		else if( !Remove( ID[R[r++]] ) ) return false;
	}
	return true;
}

double Solve(){
	for(int i=0; i<n; i++) tmp[i]=L[i]=R[i]=i;
	sort( tmp, tmp+n, cmpy );
	for(int i=0; i<n; i++) ID[tmp[i]] = i;
	sort( L, L+n, cmpl );
	sort( R, R+n, cmpr );
	double s = 0, t = sqrt(Square(p[0].x-p[1].x)+Square(p[0].y-p[1].y))-p[0].r-p[1].r;
	while( s+1e-8<t ){
		mid = (s+t)/2;
		if( Ok() ) s = mid;
		else t = mid;
	}
	return s+t;
}
void ans(){ // hdu 3124 Moonmist
	int T;
	scanf("%d",&T);
	while( T-- ){
		scanf("%d",&n);
		for(int i=0; i<n; i++) scanf("%d%d%d",&p[i].x,&p[i].y,&p[i].r);
		printf("%.6f\n", Solve());
	}
}

int main(){
	ans();
	return 0;
}