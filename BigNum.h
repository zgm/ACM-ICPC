#include<iostream>
#include<cmath>
#include<algorithm>
#include<math.h>
#include<time.h>
#include<stdlib.h>
#include<stdio.h>
#include<cstring>
#include<string.h>
#include<vector>
using namespace std;

const int N = 1000008, M = 10;

class BigNum{
public:
    bool neg; int size, *a;
	BigNum( int m=0, int n=N ){ a = new int[n]; Set( m, n ); }
    BigNum( const BigNum& x, int n=N ){ a = new int[n]; Set( x, n ); }
    ~BigNum( ){ delete []a; }
    
	void Set( int m, int n=N ){
		memset( a, 0, sizeof(int)*n );
		if( m < 0 ){ neg = true; m = -m; }
		else neg = false;
		for(size=1, a[0]=m%M, m/=M; m>0; m/=M) a[size++] = m%M;
	}
	void Set( char *s ){ // need change while M != 10
		memset( a, 0, sizeof(int)*N );
		size = strlen(s);
		for(int i=0; i<size; i++) a[i] = s[size-1-i]-'0';
	}
	void Set( const BigNum& x, int n=N ){
		size = x.size;
		memset( a, 0, sizeof(int)*n );
		memcpy( a, x.a, sizeof(int)*size );
		neg = x.neg;
	}
	int Get( int *a, int k ){ // 将(*this)转化为k进制存到a数组, 返回a数组的长度
		BigNum t( *(this) );
		int s = 0;
		while( t.Cmp( 0 ) == 1 ){ a[s++] = t%k; t = t/k; }
		return s;
	}
	int Cmp( int m ){ BigNum t( m ); return Cmp( t ); }
	int Cmp( const BigNum& x ){
		if( size!=x.size ) return ( size<x.size ? -1:1 );
		for(int i=size-1; i>=0; i--)
			if( a[i]!=x.a[i] ) return ( a[i]<x.a[i] ? -1:1 );
		return 0;
	}
	friend void Product( BigNum& s, const BigNum& x, int m){
		s.Set( x );
		int i, k = s.size;
		if( m == 0 ){ s.Set( 0 ); return; }
		if( m < 0 ){ s.neg = !s.neg; m = -m; }
		for(i=0; i<k; i++) s.a[i] *= m;
		for(i=0; i<k; s.a[i]%=M,i++) s.a[i+1] += s.a[i]/M;
		for(i=s.size; s.a[i]>0; s.a[i]%=M,i++) s.a[i+1] = s.a[i]/M;
		s.size = i;
	}
	friend BigNum operator*( const BigNum& x, int m){
		BigNum s; Product( s, x, m ); return s;
	}
	friend void Div( BigNum& s, const BigNum& x, int m){
		s.Set( x );
		int i, k = s.size;
		for(i=k-1; i>=1; s.a[i]/=m,i--) s.a[i-1] += (s.a[i]%m)*M;
		for(s.a[0]/=m, i=k-1; i>0 && s.a[i]==0; i--);
		s.size = i+1;
	}
	friend BigNum operator/( const BigNum& x, int m){
		BigNum s; Div( s, x, m ); return s;
	}
	friend int operator%( const BigNum& x, int m){
		int s = 0; // may need  long long
		for(int i=x.size-1; i>=0; i--) s = (s*M+x.a[i])%m;
		return s;
	}
	friend int sub( const BigNum& x, const BigNum& y){
		int sum = 0;
		for(int i=max(x.size,y.size)-1; i>=0; i--) sum = M*sum+x.a[i]-y.a[i];
		return sum;
	}
	BigNum& operator=( const BigNum& s){
		size = s.size;
		memset( a, 0, sizeof(int)*N );
		for(int i=0; i<size; i++) a[i] = s.a[i];
		neg = s.neg;
		return (*this);
	}
	BigNum& Copy(){ BigNum t( *this ); return t; }
	BigNum& Neg(){ BigNum t( *this ); t.neg = !neg; return t; }
	friend void Add( BigNum& s, const BigNum& x ){ // s += x, 不考虑正负号
		int i, k = max( s.size, x.size );
		for(i=0; i<k; i++) s.a[i] += x.a[i];
		for(i=0; i<k; s.a[i]%=M,i++) s.a[i+1] += s.a[i]/M;
		for(i=k; s.a[i]==0 && i>0; i--);
		s.size = i+1;
	}
	friend void Add( BigNum& s, const BigNum& x, const BigNum& y){ // s = x+y, 考虑正负号
		s.Set( x );
		if( x.neg == y.neg ) Add( s, y );
		else{
			int cmp = s.Cmp( y );
			if( cmp == 0 ) s.Set( 0 ); 
			else if( cmp == 1 ) Sub( s, y );
			else{ s.Set( y ); Sub( s, x ); }
		}
	}
	friend BigNum operator+( const BigNum& x, const BigNum& y){ // return x+y, 考虑正负号
		BigNum s; Add( s, x, y ); return s;
	}
	friend BigNum operator+( const BigNum& x, int m){
		BigNum s, y( m ); Add( s, x, y ); return s;
	}
	friend void Sub( BigNum& s, const BigNum& x ){ // s -= x, 不考虑正负号, just for s >= x
		int i, k = s.size;
		for(i=0; i<k; s.a[i]-=x.a[i],i++)
			if( s.a[i] < x.a[i] ){ s.a[i] += M; --s.a[i+1]; }
		for(i=k-1; i>0 && s.a[i]==0; i--);
		s.size = i+1;
	}
	friend void Sub( BigNum& s, const BigNum& x, const BigNum& y ){ // s = x-y, 考虑正负号
		s.Set( x );
		if( x.neg != y.neg ) Add( s, y );
		else{
			int cmp = s.Cmp( y );
			if( cmp == 0 ) s.Set( 0 );
			else if( cmp == 1 ) Sub( s, y );
			else{ s.Set( y ); Sub( s, x ); }
		}
	}
	friend BigNum operator-( const BigNum& x, const BigNum& y){ // return x-y, 考虑正负号
		BigNum s; Sub( s, x, y ); return s;
	}
	friend BigNum operator-( const BigNum& x, int m){
		BigNum s, y( m ); Sub( s, x, y ); return s;
	}
	friend void Product( BigNum& s, const BigNum& x, const BigNum& y ){ // s = x*y
		memset( s.a, 0, sizeof(int)*N );
		int i, j, k = x.size+y.size;
		for(i=0; i<x.size; i++){
			for(j=0; j<y.size; j++) s.a[i+j] += x.a[i]*y.a[j];
		}
		for(i=0; i<k; s.a[i]%=M,i++) s.a[i+1] += s.a[i]/M;
		for(i=k; i>0 && s.a[i]==0; i--);
		s.size = i+1;
		s.neg = (x.neg!=y.neg);
	}
	friend BigNum operator*( const BigNum& x, const BigNum& y){
		BigNum s; Product( s, x, y ); return s;
	}
    friend BigNum operator/( const BigNum& x, const BigNum& y );
	// s = x^p
	friend void Pow( BigNum& s, const BigNum& x, int p, BigNum& t1, BigNum& t2 ){
		if( p == 1 ) s = x;
		else{
			Pow( s, x, p>>1, t1, t2 );
			Product( t1, s, s );
			if( p&1 ){ Product( t2, t1, x ); s = t2; }
			else s = t1;
		}
	}
	void print( ){
		if( neg ) printf("-");
		printf("%d",a[size-1]);
		for(int i=size-2; i>=0; i--) printf("%d", a[i]); //printf("\n");
	}
};


class Function{
public:
	static __int64 Gcd( __int64 a, __int64 b ){
		if( a < b ) swap( a, b );
		while( b!= 0 ){ swap( a, b ); b %= a; }
		return a;
	}
	// calculate gcd(a,b) and the integer x and y where gcd(a,b)=a*x+b*y
	static __int64 Ext_Gcd( __int64 a, __int64 b, __int64 &x, __int64 &y ){
		if( b == 0 ){ x = 1; y = 0; return a; }
		__int64 d = Ext_Gcd( b, a%b, x, y );
		__int64 t = x; x = y; y = t-a/b*y;
		return d;
	}
	static __int64 Abs( __int64 a ){ return a>=0 ? a:-a; }
	// get the smallest positive answer x where x % m1 = a1 and x % m2 = a2
	static __int64 Mod_L_EQ( __int64 a1, __int64 m1, __int64 a2, __int64 m2 ){
		__int64 x, y, c = Gcd( m1, m2 );
		if( Abs(a1-a2)%c != 0 ) return -1;
		Ext_Gcd( m2/c, m1/c, x, y );
		x = a2+m2*(x*(a1-a2)/c);
		y = m1*m2/c;
		x %= y;
		return ( x<0 ? x+y:x );
	}
	static void ans(){ // poj 3708 Recurrent Function: N = 2008, M = 10
		int d, a[101], b[101], md[401], kd[401];
		char s[100]; bool vis[101];
		__int64 t[101], c[401], w[401];
		BigNum m, k;
		while( scanf("%d",&d)==1 && d!=-1 ){
			a[0] = 0;
			for(int i=1; i<d; i++) scanf("%d",&a[i]);
			for(int i=0; i<d; i++) scanf("%d",&b[i]);
			scanf("%s", s ); m.Set( s );
			scanf("%s", s ); k.Set( s );
			int mn = m.Get( md, d ), kn = k.Get( kd, d );
			if( kn > mn ){ printf("NO\n"); continue; }
			
			int i = mn-1;
			for( ; i>=0; i--){
				memset( vis, 0, sizeof(bool)*d );
				int q = md[i], r = 0;
				while( !vis[q] ){
					vis[q] = true;
					t[q] = r++;
					q = ( (i!=mn-1) ? b[q]:a[q] );
				}
				if( !vis[kd[i]] ) break;
				w[i] = r-t[q];
				c[i] = (t[kd[i]]-t[md[i]])%w[i];
			}
			if( i>=0 ){ printf("NO\n"); continue; }
			for(i=mn-2; i>=0; i--){
				__int64 s = Mod_L_EQ( c[i], w[i], c[i+1], w[i+1] );
				if( (s%w[i])!=c[i] || (s%w[i+1])!=c[i+1] ) break;
				c[i] = s;
				w[i] = w[i]*w[i+1]/Gcd( w[i], w[i+1] );
				if( i==0 ) printf("%I64d\n", s );
			}
			if( i>=0 ){ printf("NO\n"); continue; }
		}
	}
};


int main(){
	Function::ans();
	// nuaa 1113 Mysterious nick: N = 2008, M = 10
	// poj 3742 Equivalent Polynomial: N = 28, M = 100000000
	// poj 1965  Cube Root
	// poj 2757 Beautiful Numbers: N = 2008, M = 10
	return 0;
}