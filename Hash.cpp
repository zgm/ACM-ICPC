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

/*int main(){
	Hash::ans();
	return 0;
}*/


//      https://code.google.com/p/smhasher/
//-----------------------------------------------------------------------------
// MurmurHash3 was written by Austin Appleby, and is placed in the public
// domain. The author hereby disclaims copyright to this source code.

// Note - The x86 and x64 versions do _not_ produce the same results, as the
// algorithms are optimized for their respective platforms. You can still
// compile and run any of them on any platform, but your performance with the
// non-native version will be less than optimal.
//-----------------------------------------------------------------------------
#if defined(_MSC_VER)
    typedef unsigned char uint8_t;
    typedef unsigned long uint32_t;
    typedef unsigned __int64 uint64_t;
// Other compilers
#else   // defined(_MSC_VER)
    #include <stdint.h> 
#endif // !defined(_MSC_VER) 


// Platform-specific functions and macros

// Microsoft Visual Studio

#if defined(_MSC_VER)
    #define FORCE_INLINE	__forceinline
    #include <stdlib.h>
    
    #define ROTL32(x,y)	_rotl(x,y)
    #define ROTL64(x,y)	_rotl64(x,y)

    #define BIG_CONSTANT(x) (x)
// Other compilers
#else	// defined(_MSC_VER)
    #define	FORCE_INLINE inline __attribute__((always_inline))
    
    inline uint32_t rotl32 ( uint32_t x, int8_t r ){
        return (x << r) | (x >> (32 - r));
    }
    inline uint64_t rotl64 ( uint64_t x, int8_t r ){
        return (x << r) | (x >> (64 - r));
    }

    #define	ROTL32(x,y)	rotl32(x,y)
    #define ROTL64(x,y)	rotl64(x,y)

    #define BIG_CONSTANT(x) (x##LLU)
#endif // !defined(_MSC_VER)

//-----------------------------------------------------------------------------
// Block read - if your platform needs to do endian-swapping or can only
// handle aligned reads, do the conversion here

FORCE_INLINE uint32_t getblock32 ( const uint32_t * p, int i ){ return p[i]; }
FORCE_INLINE uint64_t getblock64 ( const uint64_t * p, int i ){ return p[i]; }

//-----------------------------------------------------------------------------
// Finalization mix - force all bits of a hash block to avalanche
FORCE_INLINE uint32_t fmix32 ( uint32_t h ){
  h ^= h >> 16;
  h *= 0x85ebca6b;
  h ^= h >> 13;
  h *= 0xc2b2ae35;
  h ^= h >> 16;
  return h;
}

//----------
FORCE_INLINE uint64_t fmix64 ( uint64_t k ){
  k ^= k >> 33;
  k *= BIG_CONSTANT(0xff51afd7ed558ccd);
  k ^= k >> 33;
  k *= BIG_CONSTANT(0xc4ceb9fe1a85ec53);
  k ^= k >> 33;
  return k;
}

//-----------------------------------------------------------------------------
// len: char length
void MurmurHash3_x86_32 ( const void * key, int len, uint32_t seed, void * out ){
  const uint8_t * data = (const uint8_t*)key;
  const int nblocks = len / 4;

  uint32_t h1 = seed;cout<<" <"<<h1<<"> ";

  const uint32_t c1 = 0xcc9e2d51;
  const uint32_t c2 = 0x1b873593;

  //----------
  // body
  const uint32_t * blocks = (const uint32_t *)(data + nblocks*4);
  for(int i = -nblocks; i; i++){
    uint32_t k1 = getblock32(blocks,i);

    k1 *= c1;
    k1 = ROTL32(k1,15);if(i==-2) cout<<k1<<endl;
    k1 *= c2;
    
    h1 ^= k1;
    h1 = ROTL32(h1,13); 
    h1 = h1*5+0xe6546b64;
    cout<<" <"<<h1<<"> ";
  }cout<<" <"<<h1<<"> ";

  //----------
  // tail
  const uint8_t * tail = (const uint8_t*)(data + nblocks*4);
  uint32_t k1 = 0;
  switch(len & 3){
  case 3: k1 ^= tail[2] << 16;
  case 2: k1 ^= tail[1] << 8;
  case 1: k1 ^= tail[0];
          k1 *= c1; k1 = ROTL32(k1,15); k1 *= c2; h1 ^= k1;
  };

  //----------
  // finalization
  h1 ^= len;
  h1 = fmix32(h1);
  *(uint32_t*)out = h1;
} 

//-----------------------------------------------------------------------------
// len: char length
void MurmurHash3_x86_128 ( const void * key, const int len, uint32_t seed, void * out ){
  const uint8_t * data = (const uint8_t*)key;
  const int nblocks = len / 16;

  uint32_t h1 = seed;
  uint32_t h2 = seed;
  uint32_t h3 = seed;
  uint32_t h4 = seed;

  const uint32_t c1 = 0x239b961b; 
  const uint32_t c2 = 0xab0e9789;
  const uint32_t c3 = 0x38b34ae5; 
  const uint32_t c4 = 0xa1e38b93;

  //----------
  // body
  const uint32_t * blocks = (const uint32_t *)(data + nblocks*16);
  for(int i = -nblocks; i; i++){
    uint32_t k1 = getblock32(blocks,i*4+0);
    uint32_t k2 = getblock32(blocks,i*4+1);
    uint32_t k3 = getblock32(blocks,i*4+2);
    uint32_t k4 = getblock32(blocks,i*4+3);

    k1 *= c1; k1  = ROTL32(k1,15); k1 *= c2; h1 ^= k1;

    h1 = ROTL32(h1,19); h1 += h2; h1 = h1*5+0x561ccd1b;

    k2 *= c2; k2  = ROTL32(k2,16); k2 *= c3; h2 ^= k2;

    h2 = ROTL32(h2,17); h2 += h3; h2 = h2*5+0x0bcaa747;

    k3 *= c3; k3  = ROTL32(k3,17); k3 *= c4; h3 ^= k3;

    h3 = ROTL32(h3,15); h3 += h4; h3 = h3*5+0x96cd1c35;

    k4 *= c4; k4  = ROTL32(k4,18); k4 *= c1; h4 ^= k4;

    h4 = ROTL32(h4,13); h4 += h1; h4 = h4*5+0x32ac3b17;
  }

  //----------
  // tail
  const uint8_t * tail = (const uint8_t*)(data + nblocks*16);

  uint32_t k1 = 0;
  uint32_t k2 = 0;
  uint32_t k3 = 0;
  uint32_t k4 = 0;

  switch(len & 15){
  case 15: k4 ^= tail[14] << 16;
  case 14: k4 ^= tail[13] << 8;
  case 13: k4 ^= tail[12] << 0;
           k4 *= c4; k4  = ROTL32(k4,18); k4 *= c1; h4 ^= k4;

  case 12: k3 ^= tail[11] << 24;
  case 11: k3 ^= tail[10] << 16;
  case 10: k3 ^= tail[ 9] << 8;
  case  9: k3 ^= tail[ 8] << 0;
           k3 *= c3; k3  = ROTL32(k3,17); k3 *= c4; h3 ^= k3;

  case  8: k2 ^= tail[ 7] << 24;
  case  7: k2 ^= tail[ 6] << 16;
  case  6: k2 ^= tail[ 5] << 8;
  case  5: k2 ^= tail[ 4] << 0;
           k2 *= c2; k2  = ROTL32(k2,16); k2 *= c3; h2 ^= k2;

  case  4: k1 ^= tail[ 3] << 24;
  case  3: k1 ^= tail[ 2] << 16;
  case  2: k1 ^= tail[ 1] << 8;
  case  1: k1 ^= tail[ 0] << 0;
           k1 *= c1; k1  = ROTL32(k1,15); k1 *= c2; h1 ^= k1;
  };

  //----------
  // finalization
  h1 ^= len; h2 ^= len; h3 ^= len; h4 ^= len;

  h1 += h2; h1 += h3; h1 += h4;
  h2 += h1; h3 += h1; h4 += h1;

  h1 = fmix32(h1);
  h2 = fmix32(h2);
  h3 = fmix32(h3);
  h4 = fmix32(h4);

  h1 += h2; h1 += h3; h1 += h4;
  h2 += h1; h3 += h1; h4 += h1;

  ((uint32_t*)out)[0] = h1;
  ((uint32_t*)out)[1] = h2;
  ((uint32_t*)out)[2] = h3;
  ((uint32_t*)out)[3] = h4;
}

//-----------------------------------------------------------------------------
void MurmurHash3_x64_128 ( const void * key, const int len, const uint32_t seed, void * out ){
  const uint8_t * data = (const uint8_t*)key;
  const int nblocks = len / 16;

  uint64_t h1 = seed;
  uint64_t h2 = seed;

  const uint64_t c1 = BIG_CONSTANT(0x87c37b91114253d5);
  const uint64_t c2 = BIG_CONSTANT(0x4cf5ad432745937f);

  //----------
  // body

  const uint64_t * blocks = (const uint64_t *)(data);

  for(int i = 0; i < nblocks; i++){
    uint64_t k1 = getblock64(blocks,i*2+0);
    uint64_t k2 = getblock64(blocks,i*2+1);

    k1 *= c1; k1  = ROTL64(k1,31); k1 *= c2; h1 ^= k1;

    h1 = ROTL64(h1,27); h1 += h2; h1 = h1*5+0x52dce729;

    k2 *= c2; k2  = ROTL64(k2,33); k2 *= c1; h2 ^= k2;

    h2 = ROTL64(h2,31); h2 += h1; h2 = h2*5+0x38495ab5;
  }

  //----------
  // tail
  const uint8_t * tail = (const uint8_t*)(data + nblocks*16);

  uint64_t k1 = 0;
  uint64_t k2 = 0;
  switch(len & 15){
  case 15: k2 ^= ((uint64_t)tail[14]) << 48;
  case 14: k2 ^= ((uint64_t)tail[13]) << 40;
  case 13: k2 ^= ((uint64_t)tail[12]) << 32;
  case 12: k2 ^= ((uint64_t)tail[11]) << 24;
  case 11: k2 ^= ((uint64_t)tail[10]) << 16;
  case 10: k2 ^= ((uint64_t)tail[ 9]) << 8;
  case  9: k2 ^= ((uint64_t)tail[ 8]) << 0;
           k2 *= c2; k2  = ROTL64(k2,33); k2 *= c1; h2 ^= k2;

  case  8: k1 ^= ((uint64_t)tail[ 7]) << 56;
  case  7: k1 ^= ((uint64_t)tail[ 6]) << 48;
  case  6: k1 ^= ((uint64_t)tail[ 5]) << 40;
  case  5: k1 ^= ((uint64_t)tail[ 4]) << 32;
  case  4: k1 ^= ((uint64_t)tail[ 3]) << 24;
  case  3: k1 ^= ((uint64_t)tail[ 2]) << 16;
  case  2: k1 ^= ((uint64_t)tail[ 1]) << 8;
  case  1: k1 ^= ((uint64_t)tail[ 0]) << 0;
           k1 *= c1; k1  = ROTL64(k1,31); k1 *= c2; h1 ^= k1;
  };

  //----------
  // finalization
  h1 ^= len; h2 ^= len;

  h1 += h2;
  h2 += h1;

  h1 = fmix64(h1);
  h2 = fmix64(h2);

  h1 += h2;
  h2 += h1;

  ((uint64_t*)out)[0] = h1;
  ((uint64_t*)out)[1] = h2;
}

//-----------------------------------------------------------------------------



int main(){
    unsigned int value, key;
    while( cin>>value ){
        key = 0;
        MurmurHash3_x86_32 ( (const void *)&value, 4, (uint32_t)1, (void *)&key );
        //cout<<key<<"     ";
        //MurmurHash3_x86_32 ( (const uint32_t *)&value, 32, (uint32_t)1, (uint32_t*)&key );
        cout<<key<<endl;
    }
    return 0;
}//*(uint32_t*)out = h1;


