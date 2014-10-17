#include<cstdio>
#include<iostream>
#include<time.h>
#include<algorithm>
using namespace std;

class Treap{
    struct Node{
        int key, py, size, value;
        Node *ch[2];
        void set( int Key, int Py, int Value ){
            key=Key; py=Py; size=1; value=Value; ch[0]=NULL; ch[1]=NULL;
        }
    };
public:
    static void Rotate( Node *&p, bool r ){ // false:left  true:right
        Node *t = p->ch[!r];
        p->ch[!r] = t->ch[r];
        t->ch[r] = p;
        t->size = p->size;
        p->size = ( p->ch[0] ? p->ch[0]->size:0 )+( p->ch[1] ? p->ch[1]->size:0 )+1;
        p = t;
    }
    static void Insert( Node *&t, Node *x ){
        if( !t ) t = x;
        else{
            t->size++;
            Insert( t->ch[(t->key<=x->key)], x );
            if( t->key>x->key ){
                if( t->ch[0]->py > t->py ) Rotate( t, true );
            }
            else{
                if( t->ch[1]->py > t->py ) Rotate( t, false );
            }
        }
    }
    static void Delete( Node *&t, int key ){
        t->size--;
        if( t->key == key ){
            if( !t->ch[0] ){ t = t->ch[1]; return; }
            if( !t->ch[1] ){ t = t->ch[0]; return; }
            if( t->ch[0]->py>t->ch[1]->py ){
                Rotate( t, true );
                Delete( t->ch[1], key );
            }else{
                Rotate( t, false );
                Delete( t->ch[0], key );
            }
        }
        else{ Delete( t->ch[t->key<=key], key ); }
    }
    static Node* Minimum( Node *t ){ while( t && t->ch[0] ) t = t->ch[0]; return t; }
    static Node* Maximum( Node *t ){ while( t && t->ch[1] ) t = t->ch[1]; return t; }
    static int Select( Node *t, int k ){
        while( t ){
			int s = ( t->ch[0] ? t->ch[0]->size:0 )+1;
			if( k == s ) return t->key;
			if( k < s ) t = t->ch[0];
			else{ t=t->ch[1]; k -= s; }
		}
		return -1;
    }
    static void Show( Node *t, int deep ){
        if( !t ){ printf("\n"); return; }
        if( t->ch[1] ) Show( t->ch[1], deep+1 );
        for(int i=0; i<deep; i++) printf("  "); printf("%d\n",t->key);
        if( t->ch[0] ) Show( t->ch[0], deep+1 );
    }
    /*static void ans(){ // poj 3481  Double Queue
        int cmd, value, key;
        int pn = 0; Node *p = new Node[1000008], *root = NULL, *t;
        srand((unsigned)time(NULL));
        while( scanf("%d",&cmd)==1 && cmd ){
            if( cmd==1 ){
                scanf("%d%d",&value,&key);
                p[pn++].set( key, rand(), value );
                Insert( root, &p[pn-1] );
            }else if( cmd==2 ){
                t = Maximum( root );
                printf("%d\n",t?t->value:0);
                if( t ) Delete( root, t->key );
            }else{
                t = Minimum( root );
                printf("%d\n",t?t->value:0);
                if( t ) Delete( root, t->key );
            }
        }
    }*/
	struct Point{ int l, r, k, id; };
	static bool cmp( Point s, Point t ){ return s.l<t.l; }
	static void ans(){ // poj 2761 Feed the dogs
		int n, m, a[100008], res[50008];
		Point *p = new Point[50008];
		Node *root, *sp = new Node[100008];
        srand((unsigned)time(NULL));
		while( scanf("%d%d",&n,&m)==2 ){
			for(int i=1; i<=n; i++) scanf("%d",&a[i]);
			for(int i=1; i<=m; p[i].id=i, i++){
				scanf("%d%d%d",&p[i].l,&p[i].r,&p[i].k);
			}
			sort( p+1, p+m+1, cmp );
			p[0].r = 0;
			for(int i=1, pn=0; i<=m; i++){
				if( p[i].l>p[i-1].r ){
					pn = 0;
					root = NULL;
					for(int j=p[i].l; j<=p[i].r; j++){
						sp[++pn].set( a[j], rand(), -1 );
						Insert( root, &sp[pn] );
					}
				}
				else{
					for(int j=p[i-1].l; j<p[i].l; j++){
						Delete( root, a[j] );
					}
					for(int j=p[i-1].r+1; j<=p[i].r; j++){
						sp[++pn].set( a[j], rand(), -1 );
						Insert( root, &sp[pn] );
					}
				}
				//Show( 0, root );printf("-------------------\n");
				res[p[i].id] = Select( root, p[i].k );
			}
			for(int i=1; i<=m; i++) printf("%d\n",res[i]);
		}
		delete []p;
	}
};

int main(){
	Treap::ans();
	return 0;
}
