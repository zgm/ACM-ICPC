#include<iostream>
#include<time.h>
using namespace std;

class KMP{
public:
	static void Prefix( int m, char *p, int *x ){
		x[0] = -1;
		for(int j=1, k=-1; j<m; j++){
			while( k>-1 && p[k+1]!=p[j] ) k = x[k];
			if( p[k+1] == p[j] ) k++;
			x[j] = k;
		}
	}
	static int Kmp( int n, char *t, int m, char *p ){
		int *x = new int[m], num = 0;
		Prefix( m, p, x );
		for(int i=0, k=-1; i<n; i++){
			while( k>-1 && p[k+1]!=t[i] ) k = x[k];
			if( p[k+1] == t[i] ) k++;
			if( k == m-1 ){ num++; k = x[k]; }
		}
		delete []x;
		return num;
	}
	// a[i] = { the longest common prefix of "p and p(i,m)" }
	static void EK( int m, char *p, int *a ){
		int j = 0, k = 1;
		while( j+1<m && p[j]==p[j+1] ) ++j;
		a[0] = m;
		if( m > 1 ) a[1] = j;
		for(int i=2; i<m; i++){
			int Len = k+a[k]-1, L = a[i-k];
			if( L < Len-i+1 ) a[i] = L;
			else{
				j = max( 0, Len-i+1 );
				while( i+j<m && p[i+j]==p[j] ) ++j;
				a[i] = j; k = i;
			}
		}
	}
	// b[i] = { the longest common prefix of "p and t(i,n)" }
	static void EK( int n, char *t, int m, char *p, int *a, int *b ){
		EK( m, p, a );
		int j = 0, k = 0;
		while( j<n && j<m && p[j]==t[j] ) ++j;
		b[0] = j;
		for(int i=1; i<n; i++){
			int Len = k+b[k]-1, L = a[i-k];
			if( L < Len-i+1 ) b[i] = L;
			else{
				j = max( 0, Len-i+1 );
				while( j<m && i+j<n && p[j]==t[i+j] ) ++j;
				b[i] = j; k = i;
			}
		}
	}
	// judge whether t(i,n-1)(0<=i<n) is a palindrome, return ok[]
	static void Palindrome( int n, char *t, bool *ok, char *s, int *a, int *b ){
		for(int i=0; i<n; i++) s[i] = t[n-1-i];
		EK( n, t, n, s, a, b );
		memset( ok, false, sizeof(bool)*n );
		for(int i=0; i<n; i++) if( b[i]==n-i ) ok[i] = true;
	}
	static void ans(){
		int a[1000];
		char t[1000], p[100];
		while( scanf("%s%s",t,p)==2 ){
			printf("%d\n", Kmp( strlen(t), t, strlen(p), p ) );
			EK( strlen(t), t, a );
			for(int i=0; i<strlen(t); i++) printf("%d ",a[i]); printf("\n");
		}
	}
};

class BM{
public:
	static void PreBmBc( int m, char *x, int *bmBc, int Asize = 256 ){
		for(int i=0; i<Asize; i++) bmBc[i] = m;
		for(int i=0; i<m-1; i++) bmBc[x[i]] = m-i-1;
	}
	static void Suffixes( int m, char *x, int *suff ){
		suff[m-1] = m;
		int f, g = m-1;
		for(int i=m-2; i>=0; i--){
			if( i>g && suff[i+m-1-f]<i-g ) suff[i] = suff[i+m-1-f];
			else{
				if( i<g ) g = i;
				f = i;
				while( g>=0 && x[g]==x[g+m-1-f] ) g--;
				suff[i] = f-g;
			}
		}
	}
	//static void Suffixe( int m, char *x, int *suff ){
	//	suff[m-1] = m;
	//	for(int i=m-2, k=m; i>=0; i--){
	//		while( k<m && x[k-1]!=x[i] ) k = suff[k];
	//		if( x[k-1] == x[i] ) k--;
	//		suff[i] = k;
	//	}
	//}
	static void PreBmGs( int m, char *x, int *bmGs ){
		int *suff = new int[m];
		Suffixes( m, x, suff );
		for(int i=0; i<m; i++) bmGs[i] = m;
		for(int i=m-1, j=0; i>=0; i--){
			if( suff[i] == i+1 ){
				for( ; j<m-1-i; j++){
					if( bmGs[j] == m ) bmGs[j] = m-1-i;
				}
			}
		}
		for(int i=0; i<=m-2; i++) bmGs[m-1-suff[i]] = m-1-i;
		delete []suff;
	}
	static int Bm( int n, char *y, int m, char *x, int Asize = 256 ){
		int *bmGs = new int[m], *bmBc = new int[Asize];
		PreBmGs( m, x, bmGs );
		PreBmBc( m, x, bmBc, Asize );

		int i, j = 0, res=0;
		while( j <= n-m ){
			for(i=m-1; i>=0 && x[i]==y[i+j]; i--);
			if( i < 0 ){ res++; j += bmGs[0]; }
			else j += max( bmGs[i], bmBc[y[i+j]]-m+1+i);
		}
		delete []bmGs;
		delete []bmBc;
		return res;
		//printf("bm: %d\n",res);
	}
	static void ans(){
		freopen("C:\\Documents and Settings\\zgmcn\\×ÀÃæ\\acm.out", "w", stdout );
		char s[1008];
		int n, x[1008];
		srand((unsigned)time(NULL));
		while( scanf("%d",&n)==1 ){
			for(int i=0; i<n; i++) s[i] = rand()%10+'a';
			s[n] = '\0';
			printf("%s\n",s);
			Suffixe( n, s, x );
			for(int i=0; i<n; i++) printf(" %d",x[i]); printf("\n");
			Suffixes( n, s, x );
			for(int i=0; i<n; i++) printf(" %d",x[i]); printf("\n");
		}
	}
};

int main(){
	//KMP::ans();
	BM::ans();
	return 0;
}