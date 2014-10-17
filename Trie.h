#include<iostream>
#include<queue>
#include<time.h>
#include<cmath>
#include<math.h>
using namespace std;
const int INF = (1<<28), MaxSon = 4, Mod = 100000;

class Trie{
	struct Node{ int s[MaxSon], p, suf; char c; bool vis; };
public:
	static void Init( int &pn, Node *P, int p, char c ){
		P[pn].p = p;
		P[pn].c = c;
		P[pn].vis = false;
		for(int i=0; i<MaxSon; i++) P[pn].s[i] = -1;
		++pn;
	}
	static void Insert( int *sg, int &pn, Node *P, int n, char *s ){
		int ps = 0;
		for(int i=0,u; i<n; i++){
			u = sg[s[i]];
			if( P[ps].s[u]==-1 ){ P[ps].s[u] = pn; Init( pn, P, ps, s[i] ); }
			ps = P[ps].s[u];
		}
		P[ps].vis = true;
	}
	static int Suffix( int *sg, int ps, Node *P ){
		int t = P[ps].p, u = sg[P[ps].c];
		if( t == 0 ) return 0;
		for(t=P[t].suf; t!=0 && P[t].s[u]==-1; t=P[t].suf);
		return (P[t].s[u]==-1 ? 0:P[t].s[u]);
	}
	static void TrieGraph( int *sg, Node *P ){
		queue<int> q; q.push( 0 );
		P[0].suf = 0;
		while( !q.empty() ){
			int u = q.front(); q.pop();
			for(int i=0; i<MaxSon; i++){
				int v = P[u].s[i];
				if( v == -1 ) continue;
				P[v].suf = Suffix( sg, v, P );
				P[v].vis = (P[v].vis||P[P[v].suf].vis);
				if( !P[v].vis ) q.push( v );
			}
			for(int i=0; i<MaxSon; i++){
				if( P[u].s[i] == -1 ) P[u].s[i] = P[P[u].suf].s[i];
			}
		}
	}
	static void Get_Matrix( int pn, Node *P, int &n, int *mat ){
		queue<int> q;
		int *id = new int[pn];
		memset( id, -1, sizeof(int)*pn );
		bool *vis = new bool[pn];
		memset( vis, 0, sizeof(bool)*pn );
		memset( mat, 0, sizeof(int)*pn*pn );
		for( q.push(0),n=0,id[0]=n++,vis[0]=1; !q.empty(); ){
			int u = q.front(); q.pop();
			for(int i=0; i<MaxSon; i++){
				int v = P[u].s[i];
				if( v == -1 ){ mat[id[u]*pn+0]++; continue; }
				if( P[v].vis ) continue;
				if( id[v] == -1 ) id[v] = n++;
				mat[id[u]*pn+id[v]]++;
				if( !vis[v] ){ q.push( v ); vis[v] = 1; }
			}
		}
		delete []vis;
		delete []id;
	}
	// A = A*B
	static void Mul( int n, int *A, int *B, int *tmp ){
		memset( tmp, 0, sizeof(int)*n*n );
		for(int i=0, *a=A, *t=tmp; i<n; i++, a+=n, t+=n){
			for(int j=0, *b=B; j<n; j++, b+=n){
				if( a[j] == 0 ) continue;
				for(int k=0; k<n; k++) t[k] = (t[k]+(__int64)a[j]*b[k])%Mod;
			}
		}
		memcpy( A, tmp, sizeof(int)*n*n );
	}
	// t = a^p
	static void Power( int n, int *a, int p, int *t, int *tmp ){
		if( p == 1 ) memcpy( t, a, sizeof(int)*n*n );
		else{
			Power( n, a, p>>1, t, tmp );
			Mul( n, t, t, tmp );
			if( p&1 ) Mul( n, t, a, tmp );
		}
	}
	/*static void ans(){ // poj 2778 DNA Sequence
		const int N = 1008;
		int n, m, sg[200], *mat = new int[N*N*4];
		int *a = mat+N*N, *t = mat+2*N*N, *tmp = mat+3*N*N;
		char c[5] = {"AGCT"}, s[12];
		Node P[N];
		for(int i=0; i<4; i++) sg[c[i]] = i;
		while( 2==scanf("%d%d",&n,&m) ){
			int pn = 0;
			Init( pn, P, 0, '$' );
			for(int i=0; i<n; i++){
				scanf("%s",&s);
				Insert( sg, pn, P, strlen(s), s );
			}
			TrieGraph( sg, P );
			Get_Matrix( pn, P, n, mat );
			for(int i=0; i<n*n; i++) a[i] = mat[i/n*pn+i%n];
			Power( n, a, m, t, tmp );

			int res = 0;
			for(int i=0; i<n; i++) res = (res+t[i])%100000;
			printf("%d\n",res);
		}
		delete []mat;
	}*/
	struct Point{
		int ps, lv;
		Point( int x, int y ){ ps = x; lv = y; }
	};
	static void Solve( char *c, int n, char *s, int pn, Node *P, int N, int *dp ){
		for(int i=0; i<pn; i++) for(int j=0; j<N; j++) dp[i*N+j] = INF;
		queue<Point> q;
		for( q.push(Point(0,0)), dp[0]=0; !q.empty(); ){
			int ps = q.front().ps, lv = q.front().lv;
			q.pop();
			if( lv == n ) break;
			for(int i=0; i<MaxSon; i++){
				int u = P[ps].s[i];
				if( u == -1 ) u = 0;
				if( P[u].vis ) continue;
				bool flag = (dp[u*N+lv+1]==INF);
				dp[u*N+lv+1] = min( dp[u*N+lv+1], dp[ps*N+lv]+(s[lv]!=c[i]) );
				if( flag ) q.push( Point( u, lv+1 ) );
			}
		}
		int md = INF;
		for(int i=0; i<pn; i++) md = min( md, dp[i*N+n] );
		if( md == INF ) printf("-1\n");
		else printf("%d\n", md );
	}
	static void ans(){ // poj 3691 DNA repair
		const int N = 1008;
		int n, t = 0, sg[200], *dp = new int[N*N];
		char c[5] = {"AGCT"}, s[N];
		Node P[N];
		for(int i=0; i<4; i++) sg[c[i]] = i;
		while( scanf("%d",&n)!=EOF && n!=0 ){
			int pn = 0;
			Init( pn, P, 0, '$' );
			for(int i=0; i<n; i++){
				scanf("%s",s );
				Insert( sg, pn, P, strlen(s), s );
			}
			TrieGraph( sg, P );

			scanf("%s",s);
			printf("Case %d: ",++t);
			Solve( c, strlen(s), s, pn, P, N, dp );
		}
	}
};

class Palindromes{
	struct Node{
		int ch[26], t, s; char c;
		void set( char C ){ memset( ch, -1, sizeof(int)*26 ); t = s = 0; c = C; }
	};
public:
	static void Insert( int ps, int &pn, Node *p, int n, char *s, bool *ok ){
		for(int i=0; i<n; i++){
			int v = s[i]-'a';
			if( p[ps].ch[v]==-1 ){ p[++pn].set( s[i] ); p[ps].ch[v] = pn; }
			ps = p[ps].ch[v];
			if( i<n-1 && ok[i+1] ) p[ps].s++;
		}
		p[ps].t++;
	}
	// a[i] = { the longest common prefix of "p and p(i,m)" }
	static void EK( int m, char *p, int *a ){
		int j = 0, k = 1;
		while( j+1<m && p[j]==p[j+1] ) ++j;
		a[0] = m;
		if( m > 1 ) a[1] = j;
		for(int i=2; i<m; i++){
			int Len = k+a[k]-1, L = a[i-k];
			if( L < Len-i+1 ) a[i] = L;
			else{
				j = max( 0, Len-i+1 );
				while( i+j<m && p[i+j]==p[j] ) ++j;
				a[i] = j; k = i;
			}
		}
	}
	// b[i] = { the longest common prefix of "p and t(i,n)" }
	static void EK( int n, char *t, int m, char *p, int *a, int *b ){
		EK( m, p, a );
		int j = 0, k = 0;
		while( j<n && j<m && p[j]==t[j] ) ++j;
		b[0] = j;
		for(int i=1; i<n; i++){
			int Len = k+b[k]-1, L = a[i-k];
			if( L < Len-i+1 ) b[i] = L;
			else{
				j = max( 0, Len-i+1 );
				while( j<m && i+j<n && p[j]==t[i+j] ) ++j;
				b[i] = j; k = i;
			}
		}
	}
	// judge whether t(i,n-1)(0<=i<n) is a palindrome, return ok[]
	static void Palindrome( int n, char *t, bool *ok, char *s, int *a, int *b ){
		for(int i=0; i<n; i++) s[i] = t[n-1-i];
		EK( n, t, n, s, a, b );
		memset( ok, false, sizeof(bool)*n );
		for(int i=0; i<n; i++) if( b[i]==n-i ) ok[i] = true;
	}
	static __int64 Num( int ps, Node *p, int n, char *t, bool *ok ){
		__int64 s = 0;
		for(int i=0; i<n; i++){
			ps = p[ps].ch[t[i]-'a'];
			if( ps == -1 ) break;
			if( i<n-1 && ok[i+1] ) s += p[ps].t;
		}
		if( ps != -1 ) s += p[ps].t+p[ps].s;
		return s;
	}
	static void ans(){ // poj 3376 Finding Palindromes
		const int N = 2000008;
		Node *p = new Node[N];
		char *s = new char[2*N], *t = s+N;
		int n, *m = new int[3*N], *a = m+N, *b = m+2*N;
		bool *ok = new bool[N];
		while( scanf("%d",&n)==1 ){
			p[0].set( '$' );
			m[0] = 0;
			for(int i=0, pn=0; i<n; i++){
				scanf("%d",&m[i+1]);
				scanf("%s",s+m[i]);
				Palindrome( m[i+1], s+m[i], ok, t, a, b );
				Insert( 0, pn, p, m[i+1], s+m[i], ok );
				m[i+1] += m[i];
			}
			__int64 num = 0;
			for(int i=0; i<n; i++){
				std::reverse( s+m[i], s+m[i+1] );
				//for(int j=0,k=(m[i+1]-m[i])/2; j<k; j++) swap( s[m[i]+j], s[m[i+1]-1-j] );
				Palindrome( m[i+1]-m[i], s+m[i], ok, t, a, b );
				num += Num( 0, p, m[i+1]-m[i], s+m[i], ok );
			}
			printf("%I64d\n", num );
		}
		delete []p;
		delete []s;
		delete []m;
		delete []ok;
	}
};

class Test{
	struct Node{ int s[26], p, num, id; };
public:
	static void Init( int &n, Node *P, int p ){
		memset( P[n].s, -1, sizeof(int)*26 );
		P[n].p = p; P[n].num = 0; P[n++].id = -1;
	}
	static void Insert( int &n, Node *P, int m, char *s ){
		for(int i=0,t=0; i<m; i++){
			int u = s[i]-'a';
			if( P[t].s[u]==-1 ){ P[t].s[u] = n; Init( n, P, t ); }
			t = P[t].s[u];
			P[t].num++;
		}
	}
	static void Up( int ps, Node *P ){
		if( ps!=-1 && P[ps].id==-1 ){ P[ps].id = 0; Up( P[ps].p, P ); }
	}
	static void Query( int t, Node *P, int n, char *s, int k ){
		if( P[t].id == 1 ) return;
		if( n == 0 ){ P[t].id = 1; Up( P[t].p, P ); return; }
		int r = s[0]-'a';
		if( P[t].s[r]!=-1 ) Query( P[t].s[r], P, n-1, s+1, k );
		if( k>0 ){
			Query( t, P, n-1, s+1, k-1 );
			for(int i=0; i<26; i++){
				if( P[t].s[i]==-1 || i==r ) continue;
				Query( P[t].s[i], P, n, s, k-1 );
				Query( P[t].s[i], P, n-1, s+1, k-1 );
			}
		}
	}
	static int Num( int ps, Node *P ){
		if( P[ps].id == -1 ) return 0;
		if( P[ps].id == 1 ) return P[ps].num;
		int num = 0;
		for(int i=0; i<26; i++)
			if( P[ps].s[i]!=-1 ) num += Num( P[ps].s[i], P );
		return num;
	}
	static void Resume( int ps, Node *P ){
		if( P[ps].id!=-1 ){
			P[ps].id = -1;		
			for(int i=0; i<26; i++)
				if( P[ps].s[i]!=-1 ) Resume( P[ps].s[i], P );
		}
	}
	static void ans(){ // HIT 2888  Fuzzy Google Suggest
		const int N = 300000;
		Node *P = new Node[3*N];
		int n, k; char s[20];
		while( scanf("%d",&n)==1 ){
			int pn = 0;
			Init( pn, P, -1 );
			for(int i=0; i<n; i++){
				scanf("%s",s);
				Insert( pn, P, strlen(s), s );
			}
			scanf("%d",&n);
			while( n-- ){
				scanf("%s%d",s,&k);
				Query( 0, P, strlen(s), s, k );
				printf("%d\n",Num( 0, P ) );
				Resume( 0, P );
			}
		}
		delete []P;
	}
};

int main(){
	//Trie::ans();
	//Palindromes::ans();
	Test::ans();
	return 0;
}