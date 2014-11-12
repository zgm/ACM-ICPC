#include<iostream>
#include<cmath>
#include<math.h>
#include<cstdio>
#include<cstring>
using namespace std;


const int n = 9, R = 3;

class Sudoku{
public:
	static bool single( int k, bool *ok, int *a ){
		if( (a[k]&(a[k]-1))==0 ){ ok[k] = true; return true; }
		return false;
	}
	static bool check( int r, int c, bool *ok, int *a ){
		int t = a[r*n+c];
		for(int i=0; i<n; i++){
			if( i!=c && a[r*n+i]==t ) return false;
			if( ok[r*n+i] || (a[r*n+i]&t)==0 ) continue;
			a[r*n+i] ^= t;
			if( single( r*n+i, ok, a ) && !check( r, i, ok, a ) ) return false;
		}
		for(int i=0; i<n; i++){
			if( i!=r && a[i*n+c]==t ) return false;
			if( ok[i*n+c] || (a[i*n+c]&t)==0 ) continue;
			a[i*n+c] ^= t;
			if( single( i*n+c, ok, a ) && !check( i, c, ok, a ) ) return false;
		}
		int x = r/R, y = c/R;
		for(int i=0; i<n; i++){
			int j = x*R+i/R, k = y*R+i%R;
			if( (j!=r||k!=c) && a[j*n+k]==t ) return false;
			if( ok[j*n+k] || (a[j*n+k]&t)==0 ) continue;
			a[j*n+k] ^= t;
			if( single( j*n+k, ok, a ) && !check( j, k, ok, a ) ) return false;
		}
		return true;
	}
	static bool check( bool *ok, int *a, int t ){
		t--;
		for(int i=0; i<n; i++){
			int k = 0, y = -1;
			for(int j=0; j<n && k<2; j++){
				k += ((a[i*n+j]>>t)&1);
				if( k==1 && y==-1 ) y = j;
			}
			if( k == 0 ) return false;
			if( k>1 || ok[i*n+y] ) continue;
			a[i*n+y] = (1<<t); ok[i*n+y] = true;
			if( !check( i, y, ok, a ) ) return false;
		}
		for(int j=0; j<n; j++){
			int k = 0, x = -1;
			for(int i=0; i<n && k<2; i++){
				k += ((a[i*n+j]>>t)&1);
				if( k==1 && x==-1 ) x = i;
			}
			if( k == 0 ) return false;
			if( k>1 || ok[x*n+j] ) continue;
			a[x*n+j] = (1<<t); ok[x*n+j] = true;
			if( !check( x, j, ok, a ) ) return false;
		}
		for(int i=0; i<n; i++){
			int r = i/R*R, c = i%R*R, k = 0, s = -1, x, y;
			for(int j=0; j<n && k<2; j++){
				x = r+j/R; y = c+j%R;
				k += ((a[x*n+y]>>t)&1);
				if( k==1 && s==-1 ) s = j;
			}
			if( k == 0 ) return false;
			x = r+s/R; y = c+s%R;
			if( k>1 || ok[x*n+y] ) continue;
			a[x*n+y] = (1<<t); ok[x*n+y] = true;
			if( !check( x, y, ok, a ) ) return false;
		}
		return true;
	}
	static bool check( bool *ok, int *a ){
		for(int i=0; i<n; i++) if( !check( ok, a, i+1 ) ) return false;
		return true;
	}
	static bool dfs( int r, int c, bool *ok, int *a ){
		if( ok[r*n+c] ){
			for( ; r<n && ok[r*n+c]; r=r+(c+1)/n, c=(c+1)%n);
			if( r>=n ) return true;
			return dfs( r, c, ok, a );
		}
		int ta[n*n]; bool tk[n*n];
		for(int i=0; i<n; i++){
			if( ((a[r*n+c]>>i)&1) == 0 ) continue;
			memcpy( tk, ok, sizeof(bool)*n*n );
			memcpy( ta, a, sizeof(int)*n*n );
			ok[r*n+c] = true;
			a[r*n+c] = (1<<i);
			if( check( r, c, ok, a ) && check( ok, a ) && dfs( r+(c+1)/n, (c+1)%n, ok, a ) ) return true;
			memcpy( ok, tk, sizeof(bool)*n*n );
			memcpy( a, ta, sizeof(int)*n*n );
		}
		return false;
	}
	static void print( int *a ){
		for(int i=0; i<n; i++){
			for(int j=0; j<n; j++){
				for(int t=a[i*n+j], k=0; k<n; t>>=1, k++)
					if( (t&1)!=0 ){ printf("%d",k+1); break; }
			}
			//printf("\n");
		}
		printf("\n");
	}
	/*static void ans(){
		char s[n*n+1];
		int a[n*n]; bool ok[n*n];
		while( true ){
			for(int i=0; i<n; i++) if( scanf("%s",s+i*n)==EOF ) return;
			for(int i=0; i<n*n; i++) a[i] = (1<<n)-1;
			memset( ok, false, sizeof(bool)*n*n );
			for(int i=0; i<n*n; i++){
				if( s[i] == '-' ) continue;
				a[i] = (1<<(s[i]-'A'));
				ok[i] = true;
				check( i/n, i%n, ok, a );
			}
			check( ok, a );
			dfs( 0, 0, ok, a );
			print( a );
		}
	}*/
	static void ans(){ // poj 3074 Sudoku
		char s[n*n+1];
		int a[n*n]; bool ok[n*n];
		while( scanf("%s",s)!=EOF && strcmp( s, "end")!=0 ){
			for(int i=0; i<n*n; i++) a[i] = (1<<n)-1;
			memset( ok, false, sizeof(bool)*n*n );
			for(int i=0; i<n*n; i++){
				if( s[i] == '.' ) continue;
				a[i] = (1<<(s[i]-'1'));
				ok[i] = true;
				check( i/n, i%n, ok, a );
			}
			check( ok, a );
			dfs( 0, 0, ok, a );
			print( a );
		}
	}
};

int main(){
	Sudoku::ans();
	return 0;
}
