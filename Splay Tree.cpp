#include<iostream>
#include<stdlib.h>
#include<stdio.h>
#include<algorithm>
#include<time.h>
#include<string.h>
#include<cstring>
using namespace std;

const int INF = (1<<28);

class SplayTree{
	struct Node{
		int key, size, delta, min; bool reverse;
		Node *ch[2], *p;
		//Node():key(-1),size(0){ ch[0] = ch[1] = p =NULL; }
		void set( int Key ){ key=min=Key; size=1; delta=0; reverse=0; ch[0]=ch[1]=p=NULL; }
	};
public:
	static int Size( Node *t ){ return (t ? t->size:0); }
	static void Down( Node *t ){
		if( !t ) return;
		if( t->reverse ){
			if( t->ch[0] ) t->ch[0]->reverse ^= 1;
			if( t->ch[1] ) t->ch[1]->reverse ^= 1;
			swap( t->ch[0], t->ch[1] );
			t->reverse = false;
		}
		if( t->delta ){
			if( t->ch[0] ) t->ch[0]->delta += t->delta;
			if( t->ch[1] ) t->ch[1]->delta += t->delta;
			t->key += t->delta; t->delta = 0;
		}
	}
	static void Update( Node *t ){
		if( !t ) return;
		t->size = Size(t->ch[0])+Size(t->ch[1])+1;
		t->min = t->key;
		if( t->ch[0] ) t->min = min( t->min, t->ch[0]->min+t->ch[0]->delta );
		if( t->ch[1] ) t->min = min( t->min, t->ch[1]->min+t->ch[1]->delta );
	}
	static void Rotate( Node *t, bool r ){ // false: left  true: right
		Node *p = t->p;
		p->ch[!r] = t->ch[r];
		if( t->ch[r] ) t->ch[r]->p = p;
		t->ch[r] = p; t->p = p->p; p->p = t;

		Update( p ); Update( t );
		if( ! t->p ) return;
		t->p->ch[t->p->ch[1]==p] = t;
	}
	static Node* Splay( Node *t ){
		Down( t );
		for(Node *p,*pp; t->p; ){
			p = t->p;
			Down( p->p ); Down( p ); Down( t );
			if( ! p->p ){ Rotate( t, p->ch[0]==t ); continue; }

			pp = p->p;
			if( p->ch[0]==t ){
				if( pp->ch[0]==p ){ Rotate( p, 1 ); Rotate( t, 1 ); }
				else{ Rotate( t, 1 ); Rotate( t, 0 ); }
			}
			else{
				if( pp->ch[0]==p ){ Rotate( t, 0 ); Rotate( t, 1 ); }
				else{ Rotate( p, 0 ); Rotate( t, 0 ); }
			}
		}
		return t;
	}
	static Node* Search( Node *t, int k ){
		while( true ){
			Down( t );
			int size = Size(t->ch[0]);
			if( size==k-1 ) return t;
			if( size>=k ) t = t->ch[0];
			else{ t = t->ch[1]; k = k-size-1; }
		}
	}

	/*static Node* Find( Node *t, int key ){
		while( t ){
			if( t->key == key ) return t;
			t = ( t->key<key ? t->ch[1]:t->ch[0] );
		}
		return NULL;
	}
	static Node* Insert( Node *t, Node *r ){
		if( t->key == -1 ){ t->key = r->key; return t; }
		while( true ){
			if( t->key < r->key ){
				if( ! t->ch[1] ){ t->ch[1] = r; r->p = t; break; }
				else t = t->ch[1];
			}else{
				if( ! t->ch[0] ){ t->ch[0] = r; r->p = t; break; }
				else t = t->ch[0];
			}
		}
		Splay( t );
		return t;
	}
	static Node* Insert( Node *t, int key, int &n, Node *p ){
		p[n++].set( key );
		return Insert( t, &p[n-1] );
	}
	static Node* Delete( Node *t, int key ){
		Node *s = Find( t, key );
		Splay( s );
		if( s->ch[0] ) s->ch[0]->p = NULL;
		if( s->ch[1] ) s->ch[1]->p = NULL;
		return Join( s->ch[0], s->ch[1] );
	}*/
	static Node* Insert( Node *t, int k, Node *r ){
		while( true ){
			Down( t );
			int size = Size( t->ch[0] );
			if( size==0 && k==0 ){ t->ch[0] = r; r->p = t; break; }
			else if( size>=k ) t = t->ch[0];
			else if( !t->ch[1] ){ t->ch[1] = r; r->p = t; break; }
			else{ t = t->ch[1]; k = k-size-1; }
		}
		return Splay( r );
	}
	static Node* Maximum( Node *t, bool reverse=false ){
		bool r = (t->reverse==reverse);
		return t->ch[r] ? Maximum( t->ch[r], !r ):t;
	}
	/*static Node* Minimum( Node *t, bool reverse=false ){
		bool r = (t->reverse!=reverse);
		return t->ch[r] ? Maximum( t->ch[r], !r ):t;
	}
	static Node *Predecessor( Node *t ){
		if( t->ch[0] ) return Maximum( t->ch[0] );
		Node *r = t->p;
		for( ; r && t==r->ch[0]; t=r,r=r->p);
		return r;
	}
	static Node *Successor( Node *t ){
		if( t->ch[1] ) return Minimum( t->ch[1] );
		Node *r = t->p;
		for( ; r && t==r->ch[1]; t=r,r=r->p);
		return r;
	}*/
	static Node *Join( Node *t1, Node *t2 ){
		if( !t1 ) return t2;
		if( !t2 ) return t1; 
		Node *t = Splay( Maximum(t1) );
		t->ch[1] = t2; t->size += t2->size; t2->p = t;
		Update( t );
		return t;
	}
	/*static void Split( int key, Node *t, Node *&t1, Node *&t2 ){
		Node *s = Find( t, key );
		Splay( s );
		t1 = s->ch[0];
		if( t1 ) t1->p = NULL;
		t2 = s->ch[1];
		if( t2 ) t2->p = NULL;
	}*/
	static void Show( int k, Node *t ){
		Down( t );
		if( t->ch[1] ) Show( k+1, t->ch[1] );
		for(int i=0; i<k; i++) printf("  "); printf("%d\n",t->key);
		if( t->ch[0] ) Show( k+1, t->ch[0] );
	}
	static void Show( int &num, int n, Node *t ){
		Down( t );
		if( t->ch[0] ) Show( num, n, t->ch[0] );
		if( num!=0 && num!=n ){
			if( num!=1 ) printf(" ");
			printf("%d",t->key);
		}
		num++;
		if( t->ch[1] ) Show( num, n, t->ch[1] );
	}

	static Node* Init( Node *pp, int &pn, Node *P, int l, int r, int *x ){
		int mid = (l+r)/2, n = pn;
		P[n].set( x[mid] ); P[n].p = pp;
		if( l<mid ) P[n].ch[0] = Init( &P[n], ++pn, P, l, mid-1, x );
		if( mid<r ) P[n].ch[1] = Init( &P[n], ++pn, P, mid+1, r, x );
		//P[n].size = Size(P[n].ch[0])+Size(P[n].ch[1])+1;
		Update( &P[n] );
		return &P[n];
	}
	static Node* Interval( Node *&root, int a, int b, Node *&A, Node *&B ){
		B = Search( root, b+2 );
		root = Splay( B );
		A = Search( root, a );
		root = Splay( A );
		if( A->ch[1]!=B ) Rotate( B, 0 );
		return B->ch[0];
	}
	static void ans(){  //poj 3580 SuperMemo
		const int N = 200008;
		int n, m, *x = new int[N], a, b, c; char s[10];
		Node *p = new Node[N], *root, *A, *B, *AB;
		while( scanf("%d",&n)==1 ){
			x[0] = 0; x[n+1] = n+1;
			for(int i=1; i<=n; i++) scanf("%d",&x[i]);
			int pn = 0, size = n+2;
			root = Init( NULL, pn, p, 0, n+1, x );
			scanf("%d",&m);
			while( m-- ){
				scanf("%s%d",s,&a);
				if( s[0]=='A' ){
					scanf("%d%d",&b,&c);
					AB = Interval( root, a, b, A, B );
					AB->delta += c;
					Update( B ); Update( A );
				}
				else if( strcmp(s,"REVERSE")==0 ){
					scanf("%d",&b);
					AB = Interval( root, a, b, A, B );
					AB->reverse ^= 1;
				}
				else if( strcmp(s,"REVOLVE")==0 ){
					scanf("%d%d",&b,&c);
					c %= (b-a+1);
					if( c<0 ) c += (b-a+1);
					if( c==0 ) continue;
					AB = Interval( root, a, b-c, A, B );
					AB->reverse ^= 1;
					AB = Interval( root, b-c+1, b, A, B );
					AB->reverse ^= 1;
					AB = Interval( root, a, b, A, B );
					AB->reverse ^= 1;
				}
				else if( s[0]=='I' ){
					scanf("%d",&b);
					p[++pn].set( b );
					//AB = Interval( root, a+1, a, A, B );
					//B->ch[0] = &p[pn]; p[pn].p = B;
					//Update( B ); Update( A );
					root = Insert( root, a+1, &p[pn] );
				}
				else if( s[0]=='D' ){
					AB = Interval( root, a, a, A, B );
					B->ch[0] = NULL;
					Update( B ); Update( A );
				}
				else{
					scanf("%d",&b);
					AB = Interval( root, a, b, A, B );
					printf("%d\n",AB->min+AB->delta);
				}
			}
		}
	}
	/*static void ans(){  // hdu 3487  Play with Chain
		const int N = 400000;
		int n, m, *x = new int[N], a, b, c; char s[10];
		Node *p = new Node[N], *root, *A, *B, *AB, *C, *CC;
		while( scanf("%d%d",&n,&m)==2 && n+m>0 ){
			for(int i=0; i<=n+1; i++) x[i] = i;
			int pn = 0;
			root = Init( NULL, pn, p, 0, n+1, x );
			while( m-- ){
				scanf("%s %d%d",s,&a,&b);
				AB = Interval( root, a, b, A, B );
				
				if( s[0]=='C' ){
					scanf("%d",&c);
					AB->p = NULL; B->ch[0] = NULL;
					B->size -= AB->size; A->size -= AB->size;

					C = Search( root, c+1 );
					root = Splay( C );
					CC = C->ch[1]; CC->p = NULL;
					C->size -= Size(CC); C->ch[1] = NULL;
					root = Join( C, Join(AB,CC) );
				}
				else AB->reverse ^= 1;
			}
			int num = 0; Show( num, n+1, root ); printf("\n");
		}
	}*/
	// sgu 187  Twist and whirl - want to cheat
};

int main(){
	SplayTree::ans();
	return 0;
}
