#include<iostream>
#include<algorithm>
using namespace std;

class Merge{
public:
	static void BuildTree( int l, int r, int *a, int deep, int *q, int n ){
		if( l == r ){ q[deep*n+l] = a[l]; return; }
		int mid = ((l+r)>>1);
		BuildTree( l, mid, a, deep+1, q, n );
		BuildTree( mid+1, r, a, deep+1, q, n );
		for(int i=l, j=mid+1, k=l; i<=mid || j<=r; ){
			if( j>r || (i<=mid && q[(deep+1)*n+i]<q[(deep+1)*n+j]) ){
				q[deep*n+k++] = q[(deep+1)*n+i++];
			}
			else q[deep*n+k++] = q[(deep+1)*n+j++];
		}
	}
	static int Query( int s, int t, int l, int r, int v, int deep, int *q, int n ){
		if( s<=l && r<=t ){
			return lower_bound( &q[deep*n+l], &q[deep*n+r]+1, v )-&q[deep*n+l];
		}
		int res = 0, mid = (l+r)/2;
		if( s<=mid ) res += Query( s, t, l, mid, v, deep+1, q, n );
		if( t>=mid+1 ) res += Query( s, t, mid+1, r, v, deep+1, q, n );
		return res;
	}
	static void ans(){ // poj 2104
		const int N = 100008;
		int n, m, *a = new int[N], *q = new int[18*N];
		while( scanf("%d%d",&n,&m)==2 ){
			for(int i=0; i<n; i++) scanf("%d",&a[i]);
			BuildTree( 0, n-1, a, 0, q, N );
			int s, t, rank;
			while( m-- ){
				scanf("%d%d%d", &s, &t, &rank );
				s--; t--; rank--;
				int l = 0, r = n-1, mid, k, res;
				while( l<=r ){
					mid = (l+r)/2;
					k = Query( s, t, 0, n-1, q[mid], 0, q, N );
					if( k <= rank ){ l = mid+1; res = mid; }
					else r = mid-1;
				}
				printf("%d\n", q[res] );
			}
		}
		delete []a;
		delete []q;
	}
};

int main(){
	Merge::ans();
	return 0;
}