#include<iostream>
#include<cstring>
#include<cstdio>
#include<algorithm>
#include<cmath>
using namespace std;

const double PI = 3.1415926535897932384626433832795;
const double EPS = 1e-6;

struct Complex{ double a, b; };
/*inline Complex CMul( Complex &x, Complex &y ){
	Complex s;
	s.a = x.a * y.a - x.b * y.b;
	s.b = x.a * y.b + x.b * y.a;
	return s;
}
inline Complex CAdd( Complex &x, Complex &y ){
	Complex s;
	s.a = x.a + y.a;
	s.b = x.b + y.b;
	return s;
}
inline Complex CSub( Complex &x, Complex &y ){
	Complex s;
	s.a = x.a - y.a;
	s.b = x.b - y.b;
	return s;
}*/
inline void CMul( Complex &x, Complex &y ){
	/*double a = 0, b = 0;
	if( x.a<-EPS || x.a>EPS ){
		a = x.a*y.a-x.b*y.b;
		b = x.a*y.b+x.b*y.a;
	}
	else{ a = -x.b*y.b; b = x.b*y.a; }*/
	double a = x.a * y.a - x.b * y.b;
	double b = x.a * y.b + x.b * y.a;
	x.a = a; x.b = b;
}
inline void CAdd( Complex &x, Complex &y ){
	x.a += y.a;
	x.b += y.b;
}
inline void CSub( Complex &x, Complex &y ){
	x.a -= y.a;
	x.b -= y.b;
}
inline void C2Sub( Complex &x, Complex &y ){ // x = -x-x+y
	x.a = -x.a-x.a+y.a;
	x.b = -x.b-x.b+y.b;
}

int Dig( int n ){ int s = 0; while( n ){ n>>=1; ++s; } return s; }
int Inv( int k, int len ){
	int s = 0;
	for(int i=0; i<len; i++){ s <<= 1; s |= (k&1); k >>= 1; }
	return s;
}
void FFT( int n, Complex *a, Complex *out, bool flag = false ){
	Complex w, wt; // tmp1, tmp2, tmpmul;
	int dig = Dig( n )-1;
	for(int i=0; i<n; i++) out[Inv(i,dig)] = a[i];
	for(int s=2; s<=n; s<<=1){
		w.a = cos( 2*PI/s );
		w.b = sin( 2*PI/s );
		if( flag ) w.b = -w.b;
		for(int i=0; i<n; i+=s){
			wt.a = 1.0; wt.b = 0.0;
			for(int j=0; j<s/2; j++){
				CMul( out[i+j+s/2], wt );
				CAdd( out[i+j], out[i+j+s/2] );
				C2Sub( out[i+j+s/2], out[i+j] );
				CMul( wt, w );
				/*tmp1 = CAdd( out[i+j], (tmpmul=CMul(wt, out[i+j+s/2])) );
				tmp2 = CSub( out[i+j], tmpmul );
				out[i+j] = tmp1;
				out[i+j+s/2] = tmp2;
				wt = CMul( wt, w );*/
			}
		}
	}
}

class CircularShifts{
public:
	static int maxScore( int n, int Z0, int A, int B, int M ){ // SRM 436 DIV 1 1000
		const int MaxN = 70008*2;
		int z[MaxN];
		z[0] = Z0%M;
		for(int i=1; i<2*n; i++) z[i] = (int)(((long long)z[i-1]*A+B)%M);
		for(int i=0; i<2*n; i++) z[i] %= 100;
		
		Complex a[MaxN], b[MaxN], out1[MaxN], out2[MaxN];
		int nn = (1<<(Dig(n-1)+1));
		for(int i=0; i<nn; i++){
			a[i].b = b[i].b = 0;
			if( i>=n ){ a[i].a = 0; b[i].a = 0; }
			else{
				a[i].a = (double)z[i];
				b[i].a = (double)z[2*n-1-i];
			}
		}
		FFT( nn, a, out1 );
		FFT( nn, b, out2 );
		for(int i=0; i<nn; i++) CMul( out1[i], out2[i] );
		FFT( nn, out1, out2, true );
		int res = (int)floor(out2[n-1].a/nn+out2[n-1+n].a/nn+0.1);
		for(int i=0; i<n-1; i++){
			res = max( res, (int)floor(out2[i].a/nn+out2[i+n].a/nn+0.1) );
		}
		return res;
	}
};


class Substring{
public:
	// n: the length of s; m: the max value of s[]
	static void Sort( bool *s, int n, int m, int *pm, int *ps ){
		int i, j, k, *nm = new int[max(m,n)], *pmn = new int[n];
		memset( nm, 0, sizeof(int)*m );
		for(i=0; i<n; i++) nm[s[i]]++;
		for(i=1; i<m; i++) nm[i] += nm[i-1];
		for(i=0; i<n; i++) ps[--nm[s[i]]] = i;
		for(pm[ps[0]]=0, i=1; i<n; i++){
			j = ps[i];	k = ps[i-1];
			pm[j] = pm[k];
			if( s[j] != s[k] ) pm[j]++;
		}
		for(k=1; pm[ps[n-1]] < n-1; k*=2){
			for(i=0, j=pm[ps[n-1]]; i<=j; i++) nm[i] = 0;
			for(i=0; i<n; i++) nm[pm[i]]++;
			for(i=1; i<=j; i++) nm[i] += nm[i-1];
			for(i=n-1; i>=0; i--){
				if( ps[i] < k ) continue;
				pmn[ps[i]-k] = --nm[pm[ps[i]-k]];
			}
			for(i=n-k; i<n; i++) pmn[i] = --nm[pm[i]];
			for(i=0; i<n; i++) ps[pmn[i]] = i;
			for(pmn[ps[0]]=0, i=1; i<n; i++){
				int u = ps[i], v = ps[i-1];
				pmn[u] = pmn[v];
				if( pm[u]!=pm[v] || u>=n-k || v>=n-k || (pm[u]==pm[v]&&pm[u+k]!=pm[v+k]) )
					pmn[u]++;
			}
			memcpy( pm, pmn, sizeof(int)*n );
		}
		delete []nm;
		delete []pmn;
	}

	static void Lcp( bool *s, int n, int *pm, int *ps, int *lcp, int len ){
		int i, j, k = 0;
		for(i=0; i<n; i++){
			if( pm[i] == n-1 ){ lcp[n-1] = k = 0; continue; }
			if( k > 0 ) k--;
			for(j=ps[pm[i]+1]; i+k<n && j+k<n && s[i+k]==s[j+k]; k++);
			lcp[pm[i]] = k;
		}
		if( len == -1 ) len = n;
		for(i=1, k=2; k<=len-1; i++, k*=2){
			for(j=0; j<n-k; j++){
				lcp[i*n+j] = min( lcp[(i-1)*n+j], lcp[(i-1)*n+j+k/2] );
			}
		}
	}
	static void ans(){ // 2009 hefei F: K-neighbor substrings
	// http://acm.ustc.edu.cn/ustcoj/problem.php?id=1129
		const int N = 100008*3;
		int K, d[N], *pm = new int[N], *ps = pm+N/3, *lcp = ps+N/3;
		bool *A = new bool[N], *B = new bool[N];
		Complex *a = new Complex[N], *b = new Complex[N];
		Complex *out1 = new Complex[N], *out2 = new Complex[N];
		for(int i=1; ;i++){
			scanf("%d",&K);
			if( K==-1 ) break;
			int n = 0, m = 0; char c;
			getchar();
			while( (c=getchar())!='\n' ) A[n++] = (c=='a');
			while( (c=getchar())!='\n' ) B[m++] = (c=='a');

			//scanf("%s%s",A,B);
			//int n = strlen(A), m = strlen(B);
			printf("Case %d: ",i);
			if( n<m ){ printf("0\n"); continue; }

			int nn = (1<<(Dig(n-1)+0)); // ? maybe have some problem
			for(int i=0; i<nn; i++){
				if( i>=n ){ a[i].a = 0; a[i].b = 0; }
				else{ a[i].a = 1; a[i].b = (A[i] ? 1:-1); }
				if( i>=m ){ b[i].a = 0; b[i].b = 0; }
				else{ b[i].a = 1; b[i].b = (B[m-1-i] ? 1:-1); }
			}
			FFT( nn, a, out1 );
			FFT( nn, b, out2 );
			for(int i=0; i<nn; i++) CMul( out1[i], out2[i] );
			FFT( nn, out1, out2, true );
			for(int i=m-1; i<=n-1; i++){
				d[i] = (int)floor(out2[i].a/nn+0.1);//cout<<d[i]<<" ";
				d[i] /= 2;
			}//cout<<endl;

			Sort( A, n, 2, pm, ps );
			Lcp( A, n, pm, ps, lcp, 2 );
			
			int res = 0;
			for(int i=m-1; i<=n-1; i++){
				if( d[i]<=K ){
					if( pm[i-m+1]==0 || lcp[pm[i-m+1]-1]<m ) res++;
				}
			}
			printf("%d\n",res);
		}
	}
};

void ans(){ // spoj 31 Fast Multiplication
	const int N = 10008*4;
	Complex *a = new Complex[N], *b = new Complex[N];
	Complex *out1 = new Complex[N], *out2 = new Complex[N];
	int T, n1, n2, s[N]; char x[N];
	scanf("%d",&T);
	while( T-- ){
		scanf("%s",x);
		n1 = strlen(x);
		for(int i=0; i<n1; i+=3){
			a[i/3].a = (double)(x[n1-1-i]-'0');
			if( i+1<n1 ) a[i/3].a += 10.0*(x[n1-2-i]-'0');
			if( i+2<n1 ) a[i/3].a += 100.0*(x[n1-3-i]-'0');
			a[i/3].b = 0;
		}
		scanf("%s",x);
		n2 = strlen(x);
		for(int i=0; i<n2; i+=3){
			b[i/3].a = (double)(x[n2-1-i]-'0');
			if( i+1<n2 ) b[i/3].a += 10.0*(x[n2-2-i]-'0');
			if( i+2<n2 ) b[i/3].a += 100.0*(x[n2-3-i]-'0');
			b[i/3].b = 0;
		}
		n1 = (n1+2)/3; n2 = (n2+2)/3;
		
		int nn = (1<<(Dig(max(n1,n2)-1)+1));
		for(int i=n1; i<nn; i++){ a[i].a = 0; a[i].b = 0; }
		for(int i=n2; i<nn; i++){ b[i].a = 0; b[i].b = 0; }
		FFT( nn, a, out1 );
		FFT( nn, b, out2 );
		for(int i=0; i<nn; i++) CMul( out1[i], out2[i] );
		FFT( nn, out1, out2, true );
		int len = 0;
		memset( s, 0, sizeof(s) );
		for(int i=0; i<n1+n2; i++){
			cout<<floor(out2[i].a/nn+0.1)<<endl;
			s[i] += (int)floor(out2[i].a/nn+0.1);
			if( s[i]!=0 ) len = i;
			if( s[i]>=1000 ){ s[i+1] += s[i]/1000; len = i+1; }
			s[i] %= 1000;
		}
		printf("%d",s[len]);
		for(int i=len-1; i>=0; i--) printf("%03d",s[i]); printf("\n");
	}
}

int main(){
	//Substring::ans();
	ans();
	return 0;
}