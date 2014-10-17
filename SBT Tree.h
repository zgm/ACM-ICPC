#include<iostream>
#include<time.h>
#include<algorithm>
using namespace std;

class SBT{
	struct SBTNode{
		int v, size;
		SBTNode *left, *right;
		SBTNode( int V=-1 ){ v=V; size=0; left=NULL; right=NULL; }
		void set( int V=-1 ){ v=V; size=1; left=NULL; right=NULL; }
	};
public:
	static void LeftRotate( SBTNode *&p ){
		SBTNode *t = p->right;
		p->right = t->left;
		t->left = p;
		t->size = p->size;
		p->size = ( p->left ? p->left->size:0 )+( p->right ? p->right->size:0 )+1;
		p = t;
	}
	static void RightRotate( SBTNode *&p ){
		SBTNode *t = p->left;
		p->left = t->right;
		t->right = p;
		t->size = p->size;
		p->size = ( p->left ? p->left->size:0 )+( p->right ? p->right->size:0 )+1;
		p = t;
	}
	static void Maintain( SBTNode *&t, bool flag ){
		if( !t ) return;
		if( !flag ){
			if( t->left && t->left->left && (!t->right||t->left->left->size > t->right->size) )
				RightRotate( t ); 
			else if( t->left && t->left->right && (!t->right||t->left->right->size > t->right->size) ){
				LeftRotate( t->left );
				RightRotate( t );
			}else return;
		}
		else{
			if( t->right && t->right->right && (!t->left||t->right->right->size > t->left->size) )
				LeftRotate( t );
			else if( t->right && t->right->left && (!t->left||t->right->left->size > t->left->size) ){
				RightRotate( t->right );
				LeftRotate( t );
			}else return;
		}
		Maintain( t->left, false );
		Maintain( t->right, true );
		Maintain( t, false );
		Maintain( t, true );
	}
	static void Insert( SBTNode *&t, SBTNode *x ){
		if( !t ) t = x;
		else{
			t->size++;
			Insert( t->v>=x->v ? t->left:t->right, x );
			Maintain( t, t->v <= x->v );
		}
	}
	static SBTNode* Delete( int v, SBTNode *&t ){
		if( !t ) return NULL;
		t->size--;
		if( t->v==v || (t->v>v && !t->left) || (t->v<v && !t->right) ){
			SBTNode *s;
			if( !t->left || !t->right ){ 
				s = t;
				t = ( t->left ? t->left:t->right );
			}
			else{
				s = Delete( v-1, t->right );
				t->v = s->v;
			}
			return s;
		}
		else return Delete( v, (t->v>v ? t->left:t->right) );
	}
	static SBTNode* Find( int v, SBTNode *t ){
		while( t && t->v != v ){
			if( t->v < v ) t = t->right;
			else t = t->left;
		}
		return t;
	}
	static int Rank( int v, SBTNode *t ){
		int s = 0;
		while( t->v != v ){
			if( t->v > v ) t = t->left;
			else{ s += t->size-t->right->size; t = t->right; }
		}
		return s;
	}
	static int Select( int k, SBTNode *t ){
		while( t ){
			int s = ( t->left ? t->left->size:0 )+1;
			if( k == s ) return t->v;
			if( k < s ) t = t->left;
			else{ t=t->right; k -= s; }
		}
		return -1;
	}
	static SBTNode* Predecessor( int v, SBTNode *t, bool repeat = false ){
		if( !t ) return NULL;
		if( repeat && t->v == v ) return t;
		if( t->v >= v ) return Predecessor( v, t->left, ( (repeat||t->v==v) ? true:false) );
		else{
			SBTNode *s = Predecessor( v, t->right, repeat );
			return ( !s ? t:s );
		}
	}
	static SBTNode* Successor( int v, SBTNode *t, bool repeat = false ){
		if( !t ) return NULL;
		if( repeat && t->v == v ) return t;
		if( t->v <= v ) return Successor( v, t->right, ( (repeat||t->v==v) ? true:false) );
		else{
			SBTNode *s = Successor( v, t->left, repeat );
			return ( !s ? t:s );
		}
	}
	static void Show( int k, SBTNode *t ){
		if( t->right != NULL ) Show( k+1, t->right );
		for(int i=0; i<k; i++) printf("  "); printf("%d\n",t->v);
		if( t->left != NULL ) Show( k+1, t->left );
	}
	/*struct Point{ int l, r, k, id; };
	static bool cmp( Point s, Point t ){ return s.l<t.l; }
	static void ans(){ // poj 2761 Feed the dogs
		int n, m, a[100008], res[50008];
		Point *p = new Point[50008];
		SBTNode *root, *sp = new SBTNode[100008];
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
						sp[++pn].set( a[j] );
						Insert( root, &sp[pn] );
					}
				}
				else{
					for(int j=p[i-1].l; j<p[i].l; j++){
						Delete( a[j], root );
					}
					for(int j=p[i-1].r+1; j<=p[i].r; j++){
						sp[++pn].set( a[j] );
						Insert( root, &sp[pn] );
					}
				}
				//Show( 0, root );printf("-------------------\n");
				res[p[i].id] = Select( p[i].k, root );
			}
			for(int i=1; i<=m; i++) printf("%d\n",res[i]);
		}
		delete []p;
	}*/
	static void ans(){ // Turnover 湖南省队2002年选拔赛
		int n, v;
		SBTNode *p = new SBTNode[100008];
		while( scanf("%d",&n)==1 ){
			SBTNode *root = NULL;
			int res = 0, pn = 0;
			for(int i=0; i<n; i++){
				scanf("%d",&v);
				p[++pn].set( v );
				Insert( root, &p[pn] );
				SBTNode *t1 = Predecessor( v, root );
				SBTNode *t2 = Successor( v, root );
				if( t1 == NULL ){
					if( t2 == NULL ) res += v;
					else res += abs( v-t2->v );
				}
				else{
					if( t2 == NULL ) res += abs( v-t1->v );
					else res += min( abs(v-t1->v), abs(v-t2->v) );
				}
				//Show( 0, t );
				//printf(" ------------   %d\n",res);
			}
			printf("%d\n",res);
		}
		delete []p;
	}
};

class SBTArray{
public:
	static void RightRotate( int &t, int *left, int *right, int *s ){
		int k = left[t];
		left[t] = right[k];
		right[k] = t;
		s[k] = s[t];
		s[t] = s[left[t]]+s[right[t]]+1;
		t = k;
	}
	static void LeftRotate( int &t, int *left, int *right, int *s ){
		int k = right[t];
		right[t] = left[k];
		left[k] = t;
		s[k] = s[t];
		s[t] = s[left[t]]+s[right[t]]+1;
		t = k;
	}
	static void Maintain( int &t, bool flag, int *left, int *right, int *s ){
		if( !flag ){
			if( s[left[left[t]]]>s[right[t]] ) RightRotate( t, left, right, s );
			else if( s[right[left[t]]]>s[right[t]] ){
				LeftRotate( left[t], left, right, s );
				RightRotate( t, left, right, s );
			}
			else return;
		}
		else if( s[right[right[t]]]>s[left[t]] ) LeftRotate( t, left, right, s );
		else if( s[left[right[t]]]>s[left[t]] ){
			RightRotate( right[t], left, right, s );
			LeftRotate( t, left, right, s );
		}
		else return;
		Maintain( left[t], false, left, right, s );
		Maintain( right[t], true, left, right, s );
		Maintain( t, true, left, right, s );
		Maintain( t, false, left, right, s );
	}
	static void Insert( int &t, int v, int &n, int *key, int *left, int *right, int *s ){
		if( t == 0 ){
			t = ++n;
			key[t] = v;
			s[t] = 1;
			left[t] = 0;
			right[t] = 0;
		}
		else{
			s[t]++;
			if( v < key[t] ) Insert( left[t], v, n, key, left, right, s );
			else Insert( right[t], v, n, key, left, right, s );
			Maintain( t, v>=key[t], left, right, s );
		}
	}
	static int Delete( int &t, int v, int *key, int *left, int *right, int *s ){
		s[t]--;
		if( v==key[t] || (v<key[t]&&left[t]==0) || (v>key[t]&&right[t]==0) ){
			int del = key[t];
			if( left[t]==0 || right[t]==0) t=left[t]+right[t];
			else key[t] = Delete( left[t], key[t]+1, key, left, right, s );
			return del;
		}
		else{
			if( v<key[t] ) return Delete( left[t], v, key, left, right, s );
			else return Delete( right[t], v, key, left, right, s );
		}
	}
	static int Select( int t, int k, int *key, int *left, int *right, int *s ){
		if( k==s[left[t]]+1 ) return key[t];
		else if( k<=s[left[t]] ) return Select( left[t], k, key, left, right, s );
		else return Select( right[t], k-1-s[left[t]], key, left, right, s );
	}
	struct Node{ int l, r, k, id; };
	static bool cmp( Node s, Node t ){ return s.l<t.l; }
	static void ans(){ // poj 2761
		const int N = 100008;
		int n, m, a[N], res[50008];
		int key[N], left[N], right[N], s[N];
		Node *p = new Node[50008];
		while( scanf("%d%d",&n,&m)==2 ){
			for(int i=1; i<=n; i++) scanf("%d",&a[i]);
			for(int i=1; i<=m; p[i].id=i, i++){
				scanf("%d%d%d",&p[i].l,&p[i].r,&p[i].k);
			}
			sort( p+1, p+m+1, cmp );
			p[0].r = 0;
			for(int i=1, sn, root; i<=m; i++){
				if( p[i].l>p[i-1].r ){
					sn = root = 0;
					s[0] = left[0] = right[0] = 0;
					for(int j=p[i].l; j<=p[i].r; j++)
						Insert( root, a[j], sn, key, left, right, s );
				}
				else{
					for(int j=p[i-1].l; j<p[i].l; j++)
						Delete( root, a[j], key, left, right, s );
					for(int j=p[i-1].r+1; j<=p[i].r; j++)
						Insert( root, a[j], sn, key, left, right, s );
				}
				res[p[i].id] = Select( root, p[i].k, key, left, right, s );
			}
			for(int i=1; i<=m; i++) printf("%d\n", res[i] );
		}
		delete []p;
	}
};

int main(){
	//SBT::ans();
	SBTArray::ans();
	return 0;
}