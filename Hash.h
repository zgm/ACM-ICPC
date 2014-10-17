#include<iostream>
#include<vector>
#include<queue>
#include<algorithm>
#include<stdlib.h>
#include<stdio.h>
#include<cstring>
#include<map>
using namespace std;

const int N = 100008, M = 99991;

struct Link{
	int v, w, next;
	void set( int V=-1, int W=-1, int Next=-1){ v=V; w=W; next=Next; }
};
class List{
public:
	int end; Link *V;
	List( int size = 1024 ){ V = new Link[size]; }
	~List(){ delete []V; }	
	void init( int n ){ end = n; for(int i=0; i<n; i++) V[i].next = -1; }
	void insert( int u,int v,int w ){ V[end].set(v,w,V[u].next ); V[u].next=end++; }
};

class Hash{
public:
	static unsigned int ELFHash( char *key ){
		unsigned int g, h = 0;
		while( *key ){
			h = (h<<4)+(*key++);
			g = h & 0xf0000000L;
			if( g ) h ^= g>>24;
			h &= ~g;
		}
		return ( h & 0x7FFFFFFF );
	}
	
	#define get8bits( d ) (*((const unsigned char*)(d)))
	static unsigned int SuperFastHash( char *key ){
		int len = strlen( key );
		unsigned int h = len, tmp;
		if( len <= 0 || key == NULL ) return 0;
		int rem = len&3;
		for( len>>=2; len>0; len-- ){
			h += get8bits( key );
			tmp = (get8bits(key+2)<<11)^h;
			h = (h<<16)^tmp;
			key += 4;
			h += h>>11;
		}
		switch( rem ){
			case 3:
				h += get8bits( key );
				h ^= h<<16;
				h ^= key[1]<<18;
				h += h>>11;
				break;
			case 2:
				h += get8bits( key );
				h ^= h<<11;
				h += h>>17;
				break;
			case 1:
				h += *key;
				h ^= h<<10;
				h += h>>1;
		}
		h ^= h<<3;
		h += h>>5;
		h ^= h<<4;
		h += h>>17;
		h ^= h<<25;
		h += h>>6;
		return ( h & 0x7FFFFFFF );
	}
	static unsigned int RSHash( char *key ){
		unsigned int a = 63689, b = 378551, h = 0;
		while( *key ){
			h = h*a+(*key++);
			a *= b;
		}
		return ( h & 0x7FFFFFFF );
	}
	static unsigned int JSHash( char *key ){
		unsigned int h = 1315423911;
		while( *key ) h ^= ((h<<5)+(*key++)+(h>>2));
		return ( h & 0x7FFFFFFF );
	}
	static unsigned int BKDRHash( char *key ){
		unsigned int seed = 131; // 31 131 1313 13131 131313 etc...
		unsigned int h = 0;
		while( *key ) h = h*seed+(*key++);
		return ( h & 0x7FFFFFFF );
	}
	static unsigned int SDBMHash( char *key ){
		unsigned int h = 0;
		while( *key ) h = (*key)+(h<<6)+(h<<16)-h;
		return ( h & 0x7FFFFFFF );
	}
	static unsigned int DJBHash( char *key ){
		unsigned int h = 5381;
		while( *key ) h += (h<<5)+(*key++);
		return ( h & 0x7FFFFFFF );
	}
	static unsigned int APHash( char *key ){
		unsigned int h = 0;
		for(int i=0; *key; i++){
			if( (i&1) == 0 ) h ^= ((h<<7)^(*key++)^(h>>3));
			else h ^= (~((h<<11)^(*key++)^(h>>5)));
		}
		return ( h & 0x7FFFFFFF );
	}

	static int hash( int key ){ return key%M; }
	static bool has( int key, int M, int *h ){
		for(int id=hash(key); h[id]!=-1; ){
			if( h[id]==key ) return true;
			id = ( id==M-1 ? 0:id+1 );
		}
		return false;
	}
	static void add( int key, int M, int *h ){
		int id = hash( key );
		for( ; h[id]!=-1; ){
			if( h[id]==key ) return;
			id = ( id==M-1 ? 0:id+1 );
		}
		h[id] = key;
	}
	static void ans(){ // poj 3764  The xor-longest Path
		int n, u, v, w, a[N], head, tail, q[N], h[M];
		List s( 300000 );
		while( scanf("%d",&n)==1 ){
			s.init( n );
			for(int i=1; i<n; i++){
				scanf("%d%d%d",&u,&v,&w);
				s.insert( u, v, w );
				s.insert( v, u, w );
			}
			memset( a, -1, sizeof(int)*n );
			for(head=tail=0,q[tail++]=0,a[0]=0; head!=tail; ){
				u = q[head++];
				for(int i=s.V[u].next; i!=-1; i=s.V[i].next){
					v = s.V[i].v;
					if( a[v]!=-1 ) continue;
					a[v] = ( a[u]^s.V[i].w );
					q[tail++] = v;
				}
			}
			int result = 0, num = 0, value = a[0];
			for(int i=0; i<n; i++) value = max( value, a[i] );
			while( num<=30 && (1<<num)<=value ) num++;
			if( num<=0 ){ printf("0\n"); continue; }
			
			--num;  result = (1<<num);
			for(int i=0,j=n-1; i<j; i++){
				if( a[i]>=result ){
					while( j>i && a[j]>=result ) j--;
					swap( a[i], a[j] );
				}
			}
			
			for(int k=num-1; k>=0; k--){
				result ^= (1<<k);
				if( result<=value ) continue;
				memset( h, -1, sizeof(h) );
				add( 0, M, h );
				for(int i=1; i<n; i++){
					if( a[i]==a[i-1] ) continue;
					v = a[i];
					if( k>0 ) v = ( (v>>k)<<k );
					if( a[i]>=(1<<num) ){
						if( has( result^v, M, h ) ) break;
					}
					else Hash::add( v, M, h );
					if( i==n-1 ) result ^= (1<<k);
				}
			}
			printf("%d\n",result);
		}
	}
};

int main(){
	Hash::ans();
	return 0;
}
