#include<iostream>
#include<stdlib.h>
#include<stdio.h>
#include<algorithm>
using namespace std;

const int INF = (1<<28);

class AVL{
	struct Value{
		int v, id, mv, md;
		Value( int V=-1, int Id=-1 ){ set( V, Id ); }
		void set( int V=-1, int Id=-1 ){ v = mv = V; id = md =Id; }
	};
	struct Node{
		int key, h; Value w;
		Node *ch[2];
		Node(){ h = 0; key = -1; w.set(); ch[0] = ch[1] = NULL; }
		void set( int Key, Value W, Node *NIL ){
			key = Key; w = W; h = 1;
			ch[0] = ch[1] = NIL;
		}
	};
public:
	static void Update( Node *p ){
		p->h = max( p->ch[0]->h, p->ch[1]->h )+1;
		int s = p->w.v, d = p->w.id;
		for(int i=0; i<2; i++){
			if( p->ch[i]->h && s>p->ch[i]->w.mv ){
				s = p->ch[i]->w.mv;
				d = p->ch[i]->w.md;
			}
		}
		p->w.mv = s;
		p->w.md = d;
	}
	static Node* LL( Node *p, bool d=false ){ // false: LL  true: RR
		Node *t = p->ch[d];
		p->ch[d] = t->ch[!d];
		t->ch[!d] = p;
		Update( p );
		Update( t );
		return t;
	}
	static Node* LR( Node *p, bool d=false ){  // false: LR  true: RL
		Node *t = p->ch[d], *r = t->ch[!d];
		p->ch[d] = r->ch[!d];
		t->ch[!d] = r->ch[d];
		r->ch[d] = t;
		r->ch[!d] = p;
		Update( t );
		Update( p );
		Update( r );
		return r;
	}
	static Node* Insert( Node *p, int key, Value w, int &n, Node *P, Node *NIL ){
		if( p->h == 0 ){ P[n++].set( key, w, NIL ); return &P[n-1]; }
		if( p->key>key ) p->ch[0] = Insert( p->ch[0], key, w, n, P, NIL );
		else p->ch[1] = Insert( p->ch[1], key, w, n, P, NIL );
		Update( p );
		Node *t = p->ch[0], *r = p->ch[1];
		if( t->h-r->h == 2 ){
			if( t->ch[0]->h > t->ch[1]->h ) return LL( p, 0 );
			else return LR( p, 0 );
		}
		else if( t->h-r->h == -2 ){
			if( r->ch[0]->h > r->ch[1]->h ) return LR( p, 1 );
			else return LL( p, 1 );
		}
		return p;
	}
	static void SearchUp( Node *p, int key, int &v, int &id ){
		if( p->h==0 ){ v = INF; id = -1; return; }
		if( p->key<= key ) SearchUp( p->ch[1], key, v, id );
		else{
			int mv = INF, md = -1;
			SearchUp( p->ch[0], key, mv, md );
			if( p->ch[1]->h && mv>p->ch[1]->w.mv ){
				mv = p->ch[1]->w.mv;
				md = p->ch[1]->w.md;
			}
			if( mv>p->w.v ){ mv = p->w.v; md = p->w.id; }
			v = mv; id = md;
		}
	}
	/*static void SearchDown( Node *p, int key, int &v, int &id ){
		if( p->h==0 ){ v = INF; id = -1; return; }
		if( p->key>= key ) SearchDown( p->ch[0], key, v, id );
		else{
			int mv = INF, md = -1;
			SearchUp( p->ch[1], key, mv, md );
			if( p->ch[0]->h && mv>p->ch[0]->w.mv ){
				mv = p->ch[0]->w.mv;
				md = p->ch[0]->w.md;
			}
			if( mv>p->w.v ){ mv = p->w.v; md = p->w.id; }
			v = mv; id = md;
		}
	}*/
	static void Show( Node *p, int deep ){
		if( p->ch[1]->h ) Show( p->ch[1], deep+1 );
		for(int i=0; i<deep; i++) printf(" ");
		printf("%d\n",p->key);
		if( p->ch[0]->h ) Show( p->ch[0], deep+1 );
	}

	struct Point{
		int x, y, w;
		void set( int a, int b, int c ){ x = a; y = b; w = c; }
	};
	static bool cmp1( Point s, Point t ){ return ( s.x<t.x || (s.x==t.x&&s.y<t.y) ); }
	static bool cmp2( Point s, Point t ){
		return ( s.y-s.x>t.y-t.x || (s.y-s.x==t.y-t.x&&s.y<t.y) );
	}
	static bool cmp3( Point s, Point t ){ return ( s.y>t.y || (s.y==t.y&&s.x<t.x) ); }
	static bool cmp4( Point s, Point t ){
		return ( s.x+s.y>t.x+t.y || (s.x+s.y==t.x+t.y&&s.y>t.y) );
	}
	static bool cmpw( Point s, Point t ){ return s.w<t.w; }
	static int Find( int u, int *p ){
		if( p[u] < 0 ) return u;
		else{ p[u] = Find(p[u], p); return p[u]; }
	}
	static void Union( int u, int v, int *p ){
		int pu = Find( u, p ), pv = Find( v, p );
		if( pu != pv ){
			int tmp = p[pu]+p[pv];
			if( p[pu] > p[pv] ){ p[pu] = pv; p[pv] = tmp; }
			else{ p[pv] = pu; p[pu] = tmp; }
		}
	}
	static long long Kruscal( int n, int m, Point *P, int *p ){
		sort( P, P+m, cmpw );
		memset( p, -1, sizeof(int)*(n+1) );
		int k = 0, u, v; long long res = 0;
		for(int i=1; i<n; i++){
			for( ; k<m; k++){
				u = P[k].x; v = P[k].y;
				if( Find(u, p) == Find(v, p) ) continue;
				res += P[k].w;
				Union( u, v, p );
				break;
			}
		}
		return res;
	}
	static void ans(){ // uva 3662  Another Minimum Spanning Tree
		const int N = 100008;
		int cs = 0, n, f[N]; Point t[N], pt[4*N];
		Node *NIL = new Node(), *p = new Node[N], *root;
		while( scanf("%d",&n)==1 && n ){
			for(int i=0; i<n; i++){
				scanf("%d%d",&t[i].x,&t[i].y);
				t[i].w = i;
			}
			int m = 0;
			for(int k=0; k<4; k++){
				switch(k){
					case 0: sort( t, t+n, cmp1 ); break;
					case 1: sort( t, t+n, cmp2 ); break;
					case 2: sort( t, t+n, cmp3 ); break;
					case 3: sort( t, t+n, cmp4 ); break;
					default: break;
				};
				root = NIL;
				for(int i=n-1, v, d, key, value, pn=0; i>=0; i--){
					v = d = -1;
					switch( k ){
						case 0: key = t[i].y-t[i].x; value = t[i].x+t[i].y; break;
						case 1: key = t[i].y; value = t[i].x+t[i].y; break;
						case 2: key = t[i].x+t[i].y; value = t[i].x-t[i].y; break;
						case 3: key = t[i].x; value = t[i].x-t[i].y; break;
						default: break;
					};
					SearchUp( root, key, v, d );
					if( d!=-1 ) pt[m++].set( t[i].w, t[d].w, abs(t[i].x-t[d].x)+abs(t[i].y-t[d].y) );
					root = Insert( root, key, Value(value,i), pn, p, NIL );
				}
			}
			printf("Case %d: Total Weight = %lld\n",++cs,Kruscal( n, m, pt, f ));
		}
	}
};

int main(){
	AVL::ans();
	return 0;
}
