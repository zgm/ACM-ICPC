#include<iostream>
#include<cmath>
using namespace std;

class Sequence{
public:
	static void Drs_Point( int n, int *a, int *p, int *d, double *w ){
		for(int i=n; i>0; i--){
			p[i] = i; w[i] = a[i]; d[i] = 1;
			while( p[i]<n && w[i]/d[i]<=w[p[i]+1]/d[p[i]+1] ){
				w[i] = w[i]+w[p[i]+1];
				d[i] = d[i]+d[p[i]+1];
				p[i] = p[p[i]+1];
			}
		}
	}
	static int Locate( int i, int j, int n, int L, double *s, int *p ){
		int k = (int)(log((double)L)/log(2.0));
		double s1, s2;
		for( ; k>=0; k--){
			s1 = (s[j]-s[i-1])/(j-i+1);
			s2 = (s[p[j+1]]-s[j])/(p[j+1]-j);
			if( j>=n || s1>=s2 ) return j;
			if( p[j+1]<n ){
				s1 = (s[p[j+1]]-s[i-1])/(p[j+1]-i+1);
				s2 = (s[p[p[j+1]+1]]-s[p[j+1]])/(p[p[j+1]+1]-p[j+1]);
				if( s1<s2 ) j = p[j+1];
			}
		}
		if( j<n ){
			s1 = (s[j]-s[i-1])/(j-i+1);
			s2 = (s[p[j+1]]-s[j])/(p[j+1]-j);
			if( s1<s2 ) j = p[j+1];
		}
		return j;
	}
	static void MaxAvg( int n, int L, int *a ){
		double *s = new double[n+1];
		int res = 0, *p = new int[n+1], *d = new int[n+1];
		Drs_Point( n, a, p, d, s );
		s[0] = 0;
		for(int i=1; i<=n; i++) s[i] = s[i-1]+a[i];
		for(int i=1; i<=n-L+1; i++){
			int j = i+L-1;
			if( j+1<=n ){
				double s1 = (s[j]-s[i-1])/(j-i+1);
				double s2 = (s[p[j+1]]-s[j])/(p[j+1]-j);
				if( s1 < s2 ) j = Locate( i, j, n, L, s, p );
			}
			res = max( res, (int)(s[j]-s[i-1])/(j-i+1) );
		}
		printf("%d\n", res );
		delete []s;
		delete []p;
		delete []d;
	}
	static void ans(){ // poj 2018 Best Cow Fences
		const int N = 10008;
		int n, L, a[N];
		while(scanf("%d%d",&n,&L)==2 ){
			for(int i=1; i<=n; i++){
				scanf("%d",&a[i]);
				a[i] *= 1000;
			}
			MaxAvg( n, L, a );
		}
	}
};

int main(){
	Sequence::ans();
	return 0;
}