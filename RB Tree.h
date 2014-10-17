#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<iostream>
#include<set>
#include<queue>
#include<windows.h>
using namespace std;

typedef enum{ Red, Black } Color;

struct RBNode{ // 红黑树节点
	int key;
	Color color;
	RBNode *left, *right, *p;
	RBNode(){ key = 0; color = Black; }
	void set( RBNode *NIL ){ left = right = p = NIL; }
	void set( int Key, RBNode *NIL ){ key = Key; color = Red; left = right = p = NIL; }
}*NIL;

class RBTree{ // 红黑树
public:
	static void LeftRotate( RBNode *&root, RBNode *x ){ // 左旋
		RBNode *y = x->right;
		x->right = y->left;
		if( y->left!=NIL ) y->left->p = x;
		y->p = x->p;
		if( x->p == NIL ) root = y;
		else{
			if( x->p->left==x ) x->p->left = y;
			else x->p->right = y;
		}
		y->left = x;
		x->p = y;
	}
	static void RightRotate( RBNode *&root, RBNode *x ){ // 右旋
		RBNode *y = x->left;
		x->left = y->right;
		if( y->right!=NIL ) y->right->p = x;
		y->p = x->p;
		if( x->p == NIL ) root = y;
		else{
			if( x->p->left==x ) x->p->left = y;
			else x->p->right = y;
		}
		y->right = x;
		x->p = y;
	}
	static void Insert( RBNode *&root, RBNode *z ){  // 插入节点z
		RBNode *y = NIL, *x = root;
		while( x!=NIL ){
			y = x;
			if( z->key<x->key ) x = x->left;
			else x = x->right;
		}
		z->p = y;
		if( y==NIL ) root = z;
		else if( z->key<y->key ) y->left = z;
		else y->right = z;
		z->left = z->right = NIL; z->color = Red;
		InsertFixUp( root, z );
	}
	static void InsertFixUp( RBNode *&root, RBNode *z ){ // 进行插入操作后，调用该函数保持红黑树性质
		for(RBNode *y; z->p->color==Red; ){
			if( z->p == z->p->p->left ){
				y = z->p->p->right;
				if( y->color==Red ){
					z->p->color = Black;
					y->color = Black;
					z->p->p->color = Red;
					z = z->p->p;
				}
				else{
					if( z==z->p->right ){ z = z->p; LeftRotate( root, z ); }
					z->p->color = Black;
					z->p->p->color = Red;
					RightRotate( root, z->p->p );
				}
			}
			else{
				y = z->p->p->left;
				if( y->color==Red ){
					z->p->color = Black;
					y->color = Black;
					z->p->p->color = Red;
					z = z->p->p;
				}
				else{
					if( z==z->p->left ){ z = z->p; RightRotate( root, z ); }
					z->p->color = Black;
					z->p->p->color = Red;
					LeftRotate( root, z->p->p );
				}
			}
		}
		root->color = Black;
	}
	static RBNode* Find( RBNode *x, int key ){ // 返回关键字为key的节点
		while( x!=NIL && x->key!=key ){
			x = ( x->key>key ? x->left:x->right );
		}
		return x;
	}
	static RBNode* Min( RBNode *x ){ // 最小关键字
		while( x->left!=NIL ) x = x->left;
		return x;
	}
	static RBNode* Max( RBNode *x ){ // 最大关键字
		while( x->right!=NIL ) x = x->right;
		return x;
	}
	static RBNode* Successor( RBNode *x ){ // 返回后继
		if( x->right!=NIL ) return Min( x->right );
		RBNode *y = x->p;
		while( y!=NIL && x==y->right ){
			x = y;  y = y->p;
		}
		return y;
	}
	static void Delete( RBNode *&root, RBNode *z ){ // 删除节点z
		RBNode *y, *x;
		if( z->left==NIL || z->right==NIL ) y = z;
		else y = Successor( z );
		if( y->left!=NIL ) x = y->left;
		else x = y->right;
		x->p = y->p;
		if( y->p==NIL ) root = x;
		else if( y==y->p->left ) y->p->left = x;
		else y->p->right = x;
		if( y!=z ) z->key = y->key;
		if( y->color==Black ) DeleteFixUp( root, x );
	}
	static void DeleteFixUp( RBNode *&root, RBNode *x ){  // 进行删除操作后，调用该函数保持红黑树性质
		for(RBNode *w ; x!=root && x->color==Black; ){
			if( x==x->p->left ){
				w = x->p->right;
				if( w->color==Red ){
					w->color = Black;
					x->p->color = Red;
					LeftRotate( root, x->p );
					w = x->p->right;
				}
				if( w->left->color==Black && w->right->color==Black ){
					w->color = Red;
					x = x->p;
				}
				else{
					if( w->right->color==Black ){
						w->left->color = Black;
						w->color = Red;
						RightRotate( root, w );
						w = x->p->right;
					}
					w->color = x->p->color;
					x->p->color = Black;
					w->right->color = Black;
					LeftRotate( root, x->p );
					x = root;
				}
			}
			else{
				w = x->p->left;
				if( w->color==Red ){
					w->color = Black;
					x->p->color = Red;
					RightRotate( root, x->p );
					w = x->p->left;
				}
				if( w->right->color==Black && w->left->color==Black ){
					w->color = Red;
					x = x->p;
				}
				else{
					if( w->left->color==Black ){
						w->right->color = Black;
						w->color = Red;
						LeftRotate( root, w );
						w = x->p->left;
					}
					w->color = x->p->color;
					x->p->color = Black;
					w->left->color = Black;
					RightRotate( root, x->p );
					x = root;
				}
			}
		}
		x->color = Black;
	}
	static void show( RBNode *root, int deep, HANDLE hd ){
		if( root->right!=NIL ) show( root->right, deep+1, hd );
		for(int i=0; i<deep; i++) printf("    ");
		if( root->color==Red ) SetConsoleTextAttribute( hd, FOREGROUND_RED );
		else SetConsoleTextAttribute( hd, FOREGROUND_INTENSITY );
		printf("%d\n",root->key);
		//printf("%d %s\n",root->key,root->color ?"Black":"Red");
		if( root->left!=NIL ) show( root->left, deep+1, hd );
	}
	static void show( RBNode *root, HANDLE hd ){ // 按宽度优先顺序逐行输出树的各节点
		if( root==NIL ){ printf("the tree is empty!\n"); return; }
		queue<RBNode*> q[2];
		q[0].push( root );
		RBNode *node;
		for(int i=1,r=0,k=8; ;i++,r=1-r,k/=2){
			bool sun = false;
			while( !q[r].empty() ){
				node = q[r].front();
				q[r].pop();
				for(int j=1; j<k; j++) printf("    ");
				if( k ) printf("  ");
				if( node!=NIL ){
					if( node->color==Red ) SetConsoleTextAttribute( hd, FOREGROUND_RED );
					else SetConsoleTextAttribute( hd, FOREGROUND_INTENSITY );
					printf("%4d",node->key);
					q[1-r].push( node->left );
					q[1-r].push( node->right );
					if( node->left!=NIL || node->right!=NIL ) sun = true;
				}
				else{
					printf("    ");
					q[1-r].push( NIL );
					q[1-r].push( NIL );
				}
				for(int j=1; j<k; j++) printf("    ");
				if( k ) printf("  ");
			}
			printf("\n");
			if( !sun ) break;
		}
	}
	static void test(){ // 红黑树测试程序
		const int N = 10000;
		NIL = new RBNode(); NIL->set( NIL );
		RBNode p[N], *root = NIL;
		char cmd[100], s[100]; int n, pn = 0; set<int> st;
		srand((unsigned)time(NULL));
		while( true ){
			gets( cmd );
			int num = sscanf( cmd, "%s%d",s,&n);
			if( strcmp(s,"help")==0 ){
				printf("    new n       : build a tree with n rand number (0<n<20)\n");
				printf("    insert key  : insert key into the tree\n");
				printf("    delete key  : delete key from the tree\n");
				printf("    help        : show the commond information\n");
				printf("    exit        : exit the program\n");
				continue;
			}
			else if( strcmp(s,"exit")==0 ){
				return;
			}
			else if( num!=2 ){
				printf("the input information is not enough!\n");
				continue;
			}
			else if( strcmp(s,"new")==0 ){
				pn = 0;
				root = NIL;
				st.clear();
				for(int i=0; i<n; i++){
					int key = rand()%100;
					if( st.find(key)!=st.end() ){ i--; continue; }
					st.insert( key );
					p[pn].set( key, NIL );
					Insert( root, &p[pn] );
					pn++;
				}
			}
			else if( strcmp(s,"insert")==0 ){
				if( st.find(n)!=st.end() ){
					printf("the number %d has been found in the tree!\n",n);
					continue;
				}
				else{
					st.insert( n );
					p[pn].set( n, NIL );
					Insert( root, &p[pn] );
					pn++;
				}
			}
			else if( strcmp(s,"delete")==0 ){
				if( st.find(n)==st.end() ){
					printf("the number %d is not found in the tree!\n",n);
					continue;
				}
				else{
					st.erase( st.find(n) );
					RBNode *z = Find( root, n );
					Delete( root, z );
				}
			}
			else{ printf("commond not found!\n"); continue; }


			HANDLE hd = GetStdHandle( STD_OUTPUT_HANDLE );
			printf("================================================================\n");
			//show( root, 0, hd );
			show( root, hd );
			SetConsoleTextAttribute( hd, FOREGROUND_INTENSITY );
			printf("================================================================\n");
		}
	}
};


int main(){
	RBTree::test();
	return 0;
}
