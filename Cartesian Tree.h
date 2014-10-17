#include<iostream>
#include<stdlib.h>
#include<stdio.h>
#include<algorithm>
#include<time.h>
#include<string.h>
#include<cstring>
using namespace std;

class Tree{
	struct Point{ int key, value, id; };
	struct Node{
		Point a;
		Node *ch[2], *p;
		void set( Point s ){ a = s; ch[0] = ch[1] = p = NULL; }
	};
public:
	static void Rotate( Node *t, bool r ){ // false: left  true: right
		Node *p = t->p;
		p->ch[!r] = t->ch[r];
		if( t->ch[r] ) t->ch[r]->p = p;
		t->ch[r] = p; t->p = p->p; p->p = t;

		if( ! t->p ) return;
		t->p->ch[t->p->ch[1]==p] = t;
	}
	static void Show( int k, Node *t ){
		if( t->ch[1] ) Show( k+1, t->ch[1] );
		for(int i=0; i<k; i++) printf("  ");
		printf("%d %d\n",t->a.key,t->a.value);
		if( t->ch[0] ) Show( k+1, t->ch[0] );
	}
	static bool cmp( Point s, Point t ){ return s.key<t.key; }
	static void ans(){ // sgu 155  Cartesian Tree
		const int N = 50008;
		int n, f[N], l[N], r[N]; Point p[N]; Node P[N];
		while( scanf("%d",&n)==1 ){
			for(int i=0; i<n; i++){
				scanf("%d%d",&p[i].key,&p[i].value);
				p[i].id = i+1;
			}
			sort( p, p+n, cmp );
			P[0].set( p[0] );
			for(int i=1; i<n; i++){
				P[i].set( p[i] );
				P[i-1].ch[1] = &P[i];
				P[i].p = &P[i-1];
				while( P[i].p!=NULL && P[i].p->a.value>P[i].a.value ){
					Rotate( &P[i], 0 );
				}
			}
			
			memset( f, 0, sizeof(f) );
			memset( l, 0, sizeof(l) );
			memset( r, 0, sizeof(r) );
			for(int i=0; i<n; i++){
				int j = P[i].a.id;
				if( P[i].p ) f[j] = P[i].p->a.id;
				if( P[i].ch[0] ) l[j] = P[i].ch[0]->a.id;
				if( P[i].ch[1] ) r[j] = P[i].ch[1]->a.id;
			}
			printf("YES\n");
			for(int i=1; i<=n; i++) printf("%d %d %d\n",f[i],l[i],r[i]);
		}
	}
	// zoj 2243  Binary Search Heap Construction
};

int main(){
	Tree::ans();
	return 0;
}
