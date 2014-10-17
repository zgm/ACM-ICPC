#include<iostream>
using namespace std;

class DLX{
public:
	struct Node{ int r, c; Node *left, *right, *up, *down; };
	static void Link( int r, int c, int &pd, int *cnt, Node *p, Node *row, Node *col ){
		cnt[c]++;
		Node *t = &p[pd++];
		t->r = r;
		t->c = c;
		t->left = &row[r];
		t->right = row[r].right;
		t->left->right = t;
		t->right->left = t;
		t->up = &col[c];
		t->down = col[c].down;
		t->up->down = t;
		t->down->up = t;
	}
	static void remove( int c, int *cnt, Node *row, Node *col ){
		col[c].right->left = col[c].left;
		col[c].left->right = col[c].right;
		for(Node *s=col[c].down; s!=&col[c]; s=s->down){
			for(Node *t=s->right; t!=s; t=t->right){
				cnt[t->c]--;
				t->up->down = t->down;
				t->down->up = t->up;
			}
			s->left->right = s->right;
			s->right->left = s->left;
		}
	}
	static void resume( int c, int *cnt, Node *row, Node *col ){
		for(Node *s=col[c].down; s!=&col[c]; s=s->down){
			s->right->left = s;
			s->left->right = s;
			for(Node *t=s->left; t!=s; t=t->left){
				cnt[t->c]++;
				t->down->up = t;
				t->up->down = t;
			}
		}
		col[c].left->right = &col[c];
		col[c].right->left = &col[c];
	}
	static bool solve( Node &head, int k, int *mem, int *cnt, Node *row, Node *col ){
		if( head.right == &head ) return true;
		int mn = (1<<20), tc;
		for(Node *t=head.right; t!=&head; t=t->right){
			if( cnt[t->c] < mn ){
				mn = cnt[t->c];
				tc = t->c;
				if( mn <= 1 ) break;
			}
		}
		remove( tc, cnt, row, col );
		for(Node *s=col[tc].down; s!=&col[tc]; s=s->down){
			mem[k] = s->r;
			s->left->right = s;
			for(Node *t=s->right; t!=s; t=t->right) remove( t->c, cnt, row, col );
			s->left->right = s->right;
			if( solve( head, k+1, mem, cnt, row, col ) ) return true;
			s->right->left = s;
			for(Node *t=s->left; t!=s; t=t->left) resume( t->c, cnt, row, col );
			s->right->left = s->left;
		}
		resume( tc, cnt, row, col );
		return false;
	}
	//static void ans(){ // poj 3076 Sudoku ( nkoj 1634 Sudoku )
	//	const int n = 9, m = 3;
	//	//const int n = 16, m = 4;
	//	const int R = n*n*n, C = n*n*4;
	//	Node head, *p = new Node[4*R], row[R], col[C];
	//	char s[n*n+1];
	//	int k = 0, mem[n*n], ans[n*n], cnt[C+1];
	//	while( scanf("%s",s)!=EOF && strcmp( s,"end")!=0 ){
	//		//for(int i=1; i<n; i++) scanf("%s",s+i*n);
	//		head.r = R; head.c = C;
	//		head.left = &head;
	//		head.right = &head;
	//		head.up = &head;
	//		head.down = &head;
	//		for(int i=0; i<C; i++){
	//			col[i].r = R;
	//			col[i].c = i;
	//			col[i].up = &col[i];
	//			col[i].down = &col[i];
	//			col[i].left = &head;
	//			col[i].right = head.right;
	//			col[i].left->right = &col[i];
	//			col[i].right->left = &col[i];
	//		}
	//		for(int i=0; i<R; i++){
	//			row[i].r = i;
	//			row[i].c = C;
	//			row[i].left = &row[i];
	//			row[i].right = &row[i];
	//			row[i].up = &head;
	//			row[i].down = head.down;
	//			row[i].up->down = &row[i];
	//			row[i].down->up = &row[i];
	//		}
	//		int pn = 1;
	//		memset( cnt, 0, sizeof(cnt) );
	//		for(int i=0; i<R; i++){
	//			int r = i/n/n%n, c = i/n%n, val = i%n+1;
	//			//if( s[r*n+c]=='-' || s[r*n+c]==val+'A'-1 ){
	//			if( s[r*n+c]=='.' || s[r*n+c]==val+'0' ){
	//				Link( i, r*n+val-1, pn, cnt, p, row, col );
	//				Link( i, n*n+c*n+val-1, pn, cnt, p, row, col );
	//				int tr = r/m, tc = c/m;
	//				Link( i, 2*n*n+(tr*m+tc)*n+val-1, pn, cnt, p, row, col );
	//				Link( i, 3*n*n+r*n+c, pn, cnt, p, row, col );
	//			}
	//		}
	//		for(int i=0; i<R; i++){
	//			row[i].left->right = row[i].right;
	//			row[i].right->left = row[i].left;
	//		}
	//		solve( head, 1, mem, cnt, row, col );
	//		for(int i=1; i<=n*n; i++) ans[mem[i]/n%(n*n)] = mem[i]%n+1;
	//		/*if( k!=0 ) printf("\n");
	//		k++;
	//		for(int i=0; i<n; i++){
	//			for(int j=0; j<n; j++) printf("%c",'A'-1+ans[i*n+j]);
	//			printf("\n");
	//		}*/
	//		for(int i=0; i<n*n; i++) printf("%d",ans[i]); printf("\n");
	//	}
	//	delete []p;
	//}
	static void ans(){ // poj 3740 Easy Finding
		Node head, *p = new Node[16*300], row[16], col[300];
		int n, m, mem[301], cnt[301];
		while( scanf("%d%d",&n,&m)==2 ){
			const int R = n, C = m;
			head.r = R; head.c = C;
			head.left = &head;
			head.right = &head;
			head.up = &head;
			head.down = &head;
			for(int i=0; i<C; i++){
				col[i].r = R;
				col[i].c = i;
				col[i].up = &col[i];
				col[i].down = &col[i];
				col[i].left = &head;
				col[i].right = head.right;
				col[i].left->right = &col[i];
				col[i].right->left = &col[i];
			}
			for(int i=0; i<R; i++){
				row[i].r = i;
				row[i].c = C;
				row[i].left = &row[i];
				row[i].right = &row[i];
				row[i].up = &head;
				row[i].down = head.down;
				row[i].up->down = &row[i];
				row[i].down->up = &row[i];
			}
			int pn = 1;
			memset( cnt, 0, sizeof(cnt) );
			for(int i=0; i<R; i++){
				for(int j=0,k; j<C; j++){
					scanf("%d",&k);
					if( k ) Link( i, j, pn, cnt, p, row, col );
				}
			}
			for(int i=0; i<R; i++){
				row[i].left->right = row[i].right;
				row[i].right->left = row[i].left;
			}
			if( solve( head, 1, mem, cnt, row, col ) ){
				printf("Yes, I found it\n");
			}
			else printf("It is impossible\n");
		}
		delete []p;
	}
	// wuhan online contest;  nkoj  2062 FreeOpen
};

int main(){
	DLX::ans();
	return 0;
}