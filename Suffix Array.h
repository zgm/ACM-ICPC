#include<iostream>
#include<math.h>
#include<cmath>
#include<stack>
using namespace std;

class SuffixArray{
public:
	// n: the length of s; m: the max value of s[]
	static void Sort( char *s, int n, int m, int *pm, int *ps ){
		int i, j, k, *nm = new int[max(m,n)], *pmn = new int[n];
		memset( nm, 0, sizeof(int)*m );
		for(i=0; i<n; i++) nm[s[i]]++;
		for(i=1; i<m; i++) nm[i] += nm[i-1];
		for(i=0; i<n; i++) ps[--nm[s[i]]] = i;
		for(pm[ps[0]]=0, i=1; i<n; i++){
			j = ps[i];	k = ps[i-1];
			pm[j] = pm[k];
			if( s[j] != s[k] ) pm[j]++;
		}
		for(k=1; pm[ps[n-1]] < n-1; k*=2){
			for(i=0, j=pm[ps[n-1]]; i<=j; i++) nm[i] = 0;
			for(i=0; i<n; i++) nm[pm[i]]++;
			for(i=1; i<=j; i++) nm[i] += nm[i-1];
			for(i=n-1; i>=0; i--){
				if( ps[i] < k ) continue;
				pmn[ps[i]-k] = --nm[pm[ps[i]-k]];
			}
			for(i=n-k; i<n; i++) pmn[i] = --nm[pm[i]];
			for(i=0; i<n; i++) ps[pmn[i]] = i;
			for(pmn[ps[0]]=0, i=1; i<n; i++){
				int u = ps[i], v = ps[i-1];
				pmn[u] = pmn[v];
				if( pm[u]!=pm[v] || u>=n-k || v>=n-k || (pm[u]==pm[v]&&pm[u+k]!=pm[v+k]) )
					pmn[u]++;
			}
			memcpy( pm, pmn, sizeof(int)*n );
		}
		delete []nm;
		delete []pmn;
	}

	static void Lcp( char *s, int n, int *pm, int *ps, int *lcp, int len ){
		int i, j, k = 0;
		for(i=0; i<n; i++){
			if( pm[i] == n-1 ){ lcp[n-1] = k = 0; continue; }
			if( k > 0 ) k--;
			for(j=ps[pm[i]+1]; i+k<n && j+k<n && s[i+k]==s[j+k]; k++);
			lcp[pm[i]] = k;
		}
		if( len == -1 ) len = n;
		for(i=1, k=2; k<=len-1; i++, k*=2){
			for(j=0; j<n-k; j++){
				lcp[i*n+j] = min( lcp[(i-1)*n+j], lcp[(i-1)*n+j+k/2] );
			}
		}
	}
	static int Descartes( int n, int *h, int *Left, int *Right, int *father ){
		for(int i=1; i<=n; i++){
			int p = i-1;
			father[i] = p; Left[i] = Right[i] = 0;
			Right[p] = i;
			while( father[i]>0 && h[i]<h[father[i]] ){
				Right[p] = Left[i];
				father[Left[i]] = Right[p];
				Left[i] = p;
				p = father[p];
				father[Left[i]] = i;
				Right[p] = i; father[i] = p;
			}
			//printf("%d %d %d %d\n",i,Left[i],Right[i],father[i]);
		}
		for(int i=1; i<=n; i++) if( father[i]==0 ) return i;
	}
	struct Node{
		int l, r, p, in;
		void set( int L, int R, int P, int deep ){ l = L; r = R; p = P; in = deep; }
	};
	static void Kth( long long k, int root,int n, char *s, int *ps, int *height, Node *P,
		int *left, int *right, char *result ){
		int l = 1, r = n, p = root, in = 0, top = 0;
		P[++top].set( l, r, p, in );
		while( top ){
			l = P[top].l; r = P[top].r; p = P[top].p; in = P[top].in;
			int h = ( p==0 ? (n-ps[l-1]):height[p] );
			if( k<= (r-l+1ll)*(h-in) ){
				for(int i=0; i<=(k-1)/(r-l+1); i++) result[i+in] = s[ps[l-1]+i+in];
				result[(k-1)/(r-l+1)+1+in] = 0;
				return;
			}
			k -= (r-l+1ll)*(h-in);
			if( p!=0 ){
				for(int i=in; i<h; i++) result[i] = s[ps[l-1]+i];
				P[top].set( p, r, right[p], height[p] );
				P[++top].set( l, p-1, left[p], height[p] );
			}
			else top--;
		}
	}
	static void ans(){ // hdu 3553  Just a String
	    const int N = 100008;
        int T, pm[N], ps[N], lcp[N], *p = new int[3*N];
        char s[2*N]; long long k;
		Node *P = new Node[N];
        scanf("%d",&T);
        for(int t=1; t<=T; t++){
            scanf("%s%I64d",s,&k);
            int n = strlen(s);
            Sort( s, n, 200, pm, ps );
            Lcp( s, n, pm, ps, lcp+2, 1 ); lcp[1] = 0;
            int root = Descartes( n, lcp, p, p+n+1, p+2*n+2 );
            Kth( k, root, n, s, ps, lcp, P, p, p+n+1, s+n );
            printf("Case %d: %s\n",t,s+n);
        }
	}
	/*static void ans(){ // poj 3729 Facer's string
		const int N = 100008;
		int n, m, k, s[N], pm[N], ps[N], lcp[N];
		while( scanf("%d%d%d",&n,&m,&k)==3 ){
			for(int i=0; i<n; i++) scanf("%d",&s[i]);
			s[n] = 10001;
			for(int i=n+1; i<=n+m; i++) scanf("%d",&s[i]);
			Sort( s, n+m+1, 10008, pm, ps );
			Lcp( s, n+m+1, pm, ps, lcp, 2 );

			__int64 res1 = 0, res2 = 0;			
			int t, p, p1 = 0, p2 = 0;
			for(int i=0; i<n+m+1; i++){
				if( ps[i] < n ) p1 += 1; 
				else if( ps[i] > n ) p2 = 1;
				p = lcp[i]-k+1;
				if( p <= 0 ){ res1 += p1*p2; p1 = p2 = 0; }
			}
			res1 += p1*p2;

			k++;
			p1 = p2 = 0;
			for(int i=0; i<n+m+1; i++){
				if( ps[i] < n ) p1 += 1; 
				else if( ps[i] > n ) p2 = 1;
				p = lcp[i]-k+1;
				if( p <= 0 ){ res2 += p1*p2; p1 = p2 = 0; }
			}
			res2 += p1*p2;

			printf("%I64d\n", res1-res2 );
		}
	}*/
	/*struct Node{ int h, v; Node(int H=0,int V=0){ h=H; v=V; } };
	static void ans(){ // poj 3080 Blue Jeans
		int t, n, pm[800], ps[800], lcp[800];
		char s[800];
		scanf("%d",&t);
		while( t-- ){
			scanf("%d",&n);
			int k = 0;
			for(int i=0; i<n; i++){ scanf("%s",s+k); s[k+60] = 'a'+i; k += 61; }
			s[k] = '\0';
			
			Sort( s, k, 200, pm, ps );
			Lcp( s, k, pm, ps, lcp, 2 );
			stack<Node> st;
			int res = 0, pos;
			for(int i=0; i<k; i++){
				int h = lcp[i], v = 0;
				while( !st.empty() && st.top().h>h){
					v = (v|st.top().v);
					st.pop();
					if( !st.empty() ) st.top().v = (st.top().v|v);
				}
				v = (v|(1<<(ps[i]/61)));
				if( i < k-1 ) v = (v|(1<<(ps[i+1]/61)));
				if( st.empty() || st.top().h<h ) st.push(Node(h,v));
				else if( st.top().h==h ) st.top().v = (st.top().v|v);
				if( st.top().v == (1<<n)-1 && res<st.top().h ){
					res = st.top().h;
					pos = ps[i];
				}
			}
			if( res < 3 ) printf("no significant commonalities\n");
			else{
				for(int i=pos; i<pos+res; i++) printf("%c",s[i]); printf("\n");
			}
		}
	}*/
	/*struct Node{ int p1, p2, p; Node( int x, int y, int z){ p1 = x; p2 = y; p = z; } };
	static void ans(){ // poj 3415 Common Substrings
		const int N =  200008;
		char s[N];
		int L1, L, k, pm[N], ps[N], lcp[N];
		while( scanf("%d",&k)==1 && k!=0 ){
			scanf("%s",s);
			L1 = strlen(s);
			s[L1] = '$';
			scanf("%s",s+L1+1);
			L = strlen(s);
			Sort( s, L, 200, pm, ps );
			Lcp( s, L, pm, ps, lcp, 2 );

			stack<Node> z;
			__int64 res = 0; int t, p, p1, p2;
			for(int i=0; i<L; i++){
				if( ps[i] < L1 ){ p1 = 1; p2 = 0; }
				else if( ps[i] > L1 ){ p1 = 0; p2 = 1; }
				p = lcp[i]-k+1;
				if( p <= 0 ) p = 0;
				if( z.empty() || z.top().p<p ){ z.push(Node(p1,p2,p)); continue; }
				if( z.top().p == p ){ z.top().p1 += p1; z.top().p2 += p2; continue; }
				while( !z.empty() ){
					t = z.top().p; p1 += z.top().p1; p2 += z.top().p2; z.pop();
					if( z.empty() ){
						res += (__int64)(t-p)*p1*p2;
						z.push(Node(p1,p2,p));
						break;
					}
					if( z.top().p>p ){ res += (__int64)(t-z.top().p)*p1*p2; continue; }
					if( z.top().p == p ){
						res += (__int64)(t-p)*p1*p2;
						z.top().p1 += p1;
						z.top().p2 += p2;
						break;
					}
					if( z.top().p<p ){
						res += (__int64)(t-p)*p1*p2;
						z.push(Node(p1,p2,p));
						break;
					}
				}
			}
			printf("%I64d\n", res );
		}
	}*/
	/*static void ans(){ // poj 2774 Long Long Message
		const int N = 200008;
		int pm[N], ps[N];
		char s[N];
		scanf("%s",s);
		int L1 = strlen(s);
		s[L1] = '$';
		scanf("%s",s+L1+1);
		int L = strlen(s);
		Sort( s, L, 200, pm, ps );

		int res = 0;
		for(int i=0, k=0; i<L; i++){
			if( pm[i] == L-1 ){ k = 0; continue; }
			if( k > 0 ) k--;
			for(int j=ps[pm[i]+1]; s[i+k]==s[j+k]; k++);
			if( (ps[pm[i]]<L1&&ps[pm[i]+1]>L1) || (ps[pm[i]]>L1&&ps[pm[i]+1]<L1) )
				res = max( res, k );
		}
		printf("%d\n", res );
	}*/
	/*struct Node{ int l, r, h; Node( int x, int y, int z){ l = x; r = y; h = z; } };
	static void ans(){ // poj 1743 Musical Theme
		const int N = 20008;
		int n, s[N], a[N], pm[N], ps[N], lcp[N];
		while( scanf("%d",&n)!=EOF && n!=0 ){
			scanf("%d",&a[0]);
			for(int i=1; i<n; i++){
				scanf("%d",&a[i]);
				s[i-1] = a[i]-a[i-1]+88;
			}
			n--;
			Sort( s, n, 200, pm, ps );
			Lcp( s, n, pm, ps, lcp, 2 );

			stack<Node> st;
			int res = 0, len = 0;
			for(int i=0; i<n; i++){
				int l = ps[i], r = ps[i], h = lcp[i];
				while( !st.empty() && st.top().h>h ){
					if( st.top().l<l) l = st.top().l;
					if( st.top().r>r) r = st.top().r;
					if( r-l>=st.top().h && res <= st.top().h ){
						len = max( len, r-l);
						res = st.top().h;
					}
					st.pop();
				}
				if( st.empty() || st.top().h<h ){ st.push( Node(l,r,h)); continue; }
				if( st.top().h==h){
					if( st.top().l>l) st.top().l = l;
					if( st.top().r<r) st.top().r = r;
				}
			}
			if( len == res ) res -= 1;
			if( res <= 3 ) res = -1;
			printf("%d\n",res+1);
		}
	}*/
	/*static void ans(){ // poj 3261 Milk Patterns
		char s[20008];
		int n, len, *pm, *ps, *lcp;
		while( 2==scanf("%d%d",&n,&len) ){
			int i, j;
			for(i=0; i<n; i++){ scanf("%d",&j); s[i] = j; }
			pm = new int[n];
			ps = new int[n];
			Sort( s, n, 1000001, pm, ps );
			lcp = new int[16*n];
			Lcp( s, n, pm, ps, lcp, len );
			
			int res = 0;
			for(j=1; (1<<j)<len; j++);
			j--;
			for(i=0; i+len<=n; i++)
				res = max( res, min( lcp[j*n+i], lcp[j*n+i+len-(1<<j)-1] ) );
			printf("%d\n",res);
			delete []pm;
			delete []ps;
			delete []lcp;
		}
	}*/
};

class SuffixTree{
	struct Node{
		int l, r;
		Node *son, *bro, *suffix;
		Node( int L, int R ){ l = L; r = R; son = NULL; bro = NULL; suffix = NULL; }
	};
public:
	static void Insert( Node *nd, int l, int r, int *s ){
		if( nd->son == NULL ) nd->son = new Node( l, r );
		else{
			Node *t;
			for(t=nd->son; t!=NULL && s[t->l]!=s[l]; t=t->bro);
			if( t == NULL ){
				t = new Node( l, r );
				t->bro = nd->son;
				nd->son = t;
			}
			else{
				int k = 0;
				for(k=t->l; k<=t->r; k++) if( s[k]!=s[l+k-t->l] ) break;
				if( k > t->r ) Insert( t, k, r, s );
				else{
					Node *t1 = new Node( k, t->r );
					t->r = k-1;
					t1->son = t->son;
					t->son = t1;
					Node *t2 = new Node( l+k-t->l, r );
					t1->bro = t2;
				}
			}
		}
	}
	static void DFS( Node *nd, int *s ){
		if( nd->l!=-1 ){
			for(int i=nd->l; i<=nd->r; i++) printf("%c",s[i]+'a'); printf("\n");
		}
		for(Node *t=nd->son; t!=NULL; t=t->bro) DFS( t, s );
	}
	static void ans(){
		char s[10008];
		int t[10008];
		while( scanf("%s",s)==1 ){
			int n = strlen( s );
			for(int i=0; i<n; i++) t[i] = (int)(s[i]-'a');
			Node *root = new Node( -1, -1 );
			root->suffix = root;
			for(int i=0; i<n; i++){
				Insert( root, i, n-1, t );
				DFS( root, t );
				printf("----------------------------------------\n");
			}
		}
	}
};

int main(){
	SuffixArray::ans();
	//SuffixTree::ans();
	return 0;
}