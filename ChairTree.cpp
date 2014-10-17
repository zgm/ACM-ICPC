#include<cstdio>
#include<algorithm>
#include<iostream>
#include<map>
#include<cstring>
#include<iostream>
using namespace std;


class ChairTree{
	struct Node{
		int l, r, s;
		void set(int L, int R, int S){ l=L; r=R; s=S; }
		//void set(Node x){ set(x.l,x.r,x.s); }
	};
public:
    static int BuildTree( int l, int r, Node node[], int &pn ){
        int ps = pn++, mid = (l+r)/2;
		node[ps].set( -1, -1, 0 );
		if( l!=r ){
			node[ps].l = BuildTree( l, mid, node, pn );
			node[ps].r = BuildTree( mid+1, r, node, pn );
		}
		return ps;
    }
	static int Insert( int l, int r, int root, int x, Node node[], int &pn, int val=1 ){
		int ps = pn++, mid = (l+r)/2;
		node[ps].set( node[root].l, node[root].r, node[root].s+val );
		if( l!=r ){
			if( x<=mid ) node[ps].l = Insert( l, mid, node[root].l, x, node, pn, val );
			else node[ps].r = Insert( mid+1, r, node[root].r, x, node, pn, val );
		}
		return ps;
	}
	static int Query( int l, int r, int s, int t, int k, Node node[] ){
		if( l==r ) return r;
		int Ls = node[s].l, Lt = node[t].l;
		int Rs = node[s].r, Rt = node[t].r;
		if( node[Lt].s-node[Ls].s>=k ){
			return Query( l, (l+r)/2, Ls, Lt, k, node );
		}else{
			k -= node[Lt].s-node[Ls].s;
			return Query( (l+r)/2+1, r, Rs, Rt, k, node );
		}
	}

	static int Query( int l, int r, int L, int R, int k, const int N, int *cur, Node node[] ){
		if( l==r ) return r;
		int size = node[node[cur[R]].l].s - node[node[cur[L-1]].l].s;
		for(int i=R; i>0; i-=Lowbit(i)) size += node[node[cur[N+i]].l].s;
		for(int i=L-1; i>0; i-=Lowbit(i)) size -= node[node[cur[N+i]].l].s;
		
		for(int i=R,j=L-1; i>0 || j>0; ){
			if( i<j ) swap( i, j );
			cur[N+i] = (size>=k ? node[cur[N+i]].l:node[cur[N+i]].r);
			if( i==j ) j -= Lowbit(j);
			i -= Lowbit(i);
		}

		if( size>=k ){
			cur[R] = node[cur[R]].l;
			cur[L-1] = node[cur[L-1]].l;
			return Query( l, (l+r)/2, L, R, k, N, cur, node );
		}else{
			cur[R] = node[cur[R]].r;
			cur[L-1] = node[cur[L-1]].r;
			return Query( (l+r)/2+1, r, L, R, k-size, N, cur, node );
		}
	}
	static int BinarySearch( int n, int *x, int val ){
		int L = 0, R = n-1, mid;
		while( L<=R ){
			mid = (L+R)/2;
			if( x[mid]==val ) return mid;
			if( x[mid]<val ) L = mid+1;
			else R = mid-1;
		}
	}
    /*static void ans(){ // poj 2104  K-th Number   // poj 2761
        const int N = 100008;
        int n, m, v[N], s[N], root[N];
		Node *node = new Node[N*20];
        while( scanf("%d%d",&n,&m)==2 ){
            for(int i=0; i<n; i++){
                scanf("%d",&v[i]);
                s[i] = v[i];
            }
            sort( s, s+n );
			//int sn = unique( s, s+n )-s;
			int pn = 0;
			root[0] = BuildTree( 0, n-1, node, pn );
			for(int i=0; i<n; i++){
				//int id = lower_bound(s,s+n,v[i])-s;
				int id = BinarySearch( n, s, v[i] );//cout<<id<<" ";
				root[i+1] = Insert( 0, n-1, root[i], id, node, pn );
			}//cout<<endl;
            for(int i=0,L,R,K; i<m; i++){
                scanf("%d%d%d",&L,&R,&K);
				//cout<<Query(0,n-1,root[L-1],root[R],K,node)<<"    ";
                printf("%d\n",s[Query(0,n-1,root[L-1],root[R],K,node)]);
            }
        }
    }*/

    static int Lowbit( int t ){ return ( t&(t^(t-1)) ); }
	static void ans(){ // bzoj 1901   http://www.lydsy.com/JudgeOnline/problem.php?id=1901
		const int N = 10008;
		int T, n, m, a[N], cmd[N][4], s[2*N], root[2*N], cur[2*N];
		char c[10];
		Node *node = new Node[N*200];
		scanf("%d",&T);
		//while( T-- ){ scanf("%d%d",&n,&m); // zoj 2112
		while( scanf("%d%d",&n,&m)==2 ){
			int sn = 0;
			for(int i=0; i<n; i++){ scanf("%d",&a[i]); s[sn++] = a[i]; }
			for(int i=0; i<m; i++){
				scanf("%s",c);
				if( c[0]=='Q' ){
					cmd[i][0] = 0;
					scanf("%d%d%d",&cmd[i][1],&cmd[i][2],&cmd[i][3]);
				}else{
					cmd[i][0] = 1;
					scanf("%d%d",&cmd[i][1],&cmd[i][2]);
					s[sn++] = cmd[i][2];
				}
			}
			sort(s,s+sn);
			sn = unique(s,s+sn) - s;

			int pn = 0;
			root[0] = BuildTree( 0, sn-1, node, pn );
			root[N] = BuildTree( 0, sn-1, node, pn );
			for(int i=0; i<n; i++){
				int id = BinarySearch( sn, s, a[i] );
				root[i+1] = Insert( 0, sn-1, root[i], id, node, pn );
				root[N+i+1] = root[N];
			}

			for(int i=0,L,R,K; i<m; i++){
				L = cmd[i][1]; R = cmd[i][2];
				if( cmd[i][0]==0 ){
					cur[L-1] = root[L-1]; cur[R] = root[R];
					for(int j=L-1; j>0; j-=Lowbit(j)) cur[N+j] = root[N+j];
					for(int j=R; j>0; j-=Lowbit(j)) cur[N+j] = root[N+j];
					printf("%d\n",s[Query(0,sn-1,L,R,cmd[i][3],N,cur,node)]);
				}else{
					int id1 = BinarySearch( sn, s, a[L-1] );
					int id2 = BinarySearch( sn, s, R );
					for(int j=L; j<=n; j+=Lowbit(j)){
						root[N+j] = Insert(0,sn-1,root[N+j],id1,node,pn,-1);
						root[N+j] = Insert(0,sn-1,root[N+j],id2,node,pn,1);
					}
					a[L-1] = R;
				}
			}
		}
	}
};

int main(){
    ChairTree::ans();
    return 0;
}

