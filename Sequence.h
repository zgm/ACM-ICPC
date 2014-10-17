#include<stdlib.h>
#include<stdio.h>
#include<iostream>
#include<queue>
#include<time.h>
using namespace std;

class Sequence{
public:
	/*struct Point{ int v, id; };
	static bool cmp( Point s, Point t ){ return s.v<t.v; }
	static void NewSort( int n, int *a, Point *p ){ // 将序列a[1...n]中的数变换为1...n
		for(int i=0; i<n; i++){ p[i].v = a[i+1]; p[i].id = i; }
		sort( p, p+n, cmp );
		for(int i=0; i<n; i++) a[p[i].id+1] = i+1;
	}
	// 给定序列a[n],每次交换两个数,交换费用为两数之和,求最小费用使序列按升序排列
	static void ans(){ // nkoj 1835 Cow Sorting
		int n, a[10008];
		bool vis[10008];
		Point p[10008];
		while( scanf("%d",&n)==1 ){
			int res = 0;
			for(int i=1; i<=n; i++){ scanf("%d",&a[i]); res+=a[i]; }
			NewSort( n, a, p );
			memset( vis, 0, sizeof(bool)*(n+1) );
			for(int i=1; i<=n; i++){
				if( vis[i] ) continue;
				int t = a[i], k = 1, m = p[i-1].v;
				while( t!=i ){
					vis[t] = true;
					k++;
					m = min( m, p[a[i]-1].v );
					t = a[t];
				}
				res += min( (k-2)*m, m+(k+1)*p[0].v );
			}
			printf("%d\n",res);
		}
	}*/
	// 给定数列a[n],在区间[l...r]内求最长不重复序列的长度
	/*static void ans(){ // poj 3419 Difference Is Beautiful
		const int N = 200008;
		int n, m, a[N], res[N], next[N], q[N];
		bool flag[10*N];
		while( scanf("%d%d",&n,&m)==2 ){
			for(int i=0; i<n; i++){ scanf("%d",&a[i]); a[i] += 1000000; }
			for(int i=0; i<10*N; i++) flag[i] = 0;
			for(int i=0; i<n; i++) next[i] = 0;
			int head = 0, tail = 0;
			for(int i=0; i<=n; i++){
				if( i==n || flag[a[i]] ){
					while( head < tail ){
						int u = q[head++];
						res[u] = i-u;
						if( i<n && a[u]==a[i] ){ next[u] = -1; break; }
						flag[a[u]] = 0;
					}
					q[tail++] = i;
				}
				else{ flag[a[i]] = 1; q[tail++] = i; }
			}
			for(int i=n-1; i>=0; i--){
				if( next[i]==-1 ){
					next[i] = i;
					int u = i;
					while( i>=0 && next[--i]==0) next[i] = u;
					i++;
				}
			}
			int l, r;
			for(int i=0; i<m; i++){
				scanf("%d%d",&l,&r);
				int k = 0;
				while( 1 ){
					if( r-l < k ) break;
					if( l+res[l]-1>=r ){ k = max( k, r-l+1 ); break; }
					k = max( k, res[l] );
					l = next[l]+1;
				}
				printf("%d\n",k);
			}
		}
	}*/

	// 将数列a[1...n]分成连续的几份,使得每份的和不大于M,怎样的分法使得每份最大值之和最小
	/*static void ans(){ // poj 3017 Cut the Sequence
		int n, a[100008], index[100008], opt[100008], q[100008];
		__int64 m;
		while( scanf("%d%I64d",&n,&m)==2 ){
			for(int i=1; i<=n; i++) scanf("%d",&a[i]);
			__int64 tp = 0;
			for(int i=1; i<=n; i++) tp = (tp>a[i] ? tp:a[i]);
			if( tp > m ){ printf("-1\n"); continue; }
			tp = 0;
			for(int i=1, k=1; i<=n; i++){
				tp += a[i];
				while( tp > m ) tp -= a[k++];
				index[i] = k-1;
			}

			const int INF = 1<<28;
			opt[0] = 0;
			int q1 = 0; q[q1++] = 0; a[0] = INF;
			for(int i=1; i<=n; i++){
				int k = 0;
				for(int j=0; j<q1; j++){
					int id = q[j];
					if( id>=index[i] && (id==0||a[id]>=a[i]) ) q[k++] = id;
				}
				q[k++] = i;
				opt[i] = opt[index[i]]+a[q[0]];
				for(int j=0; j<k-1; j++){
					opt[i] = min( opt[i], opt[q[j]]+a[q[j+1]] );
				}
				q1 = k;
			}
			printf("%d\n",opt[n]);
		}
	}*/

	static int HalfSearch( int n, int *a, int val ){
		int l = 0, r = n-1, mid, s = n;
		while( l<=r ){
			mid = (l+r)/2;
			if( a[mid]<=val ) l = mid+1; // 不可以相等需去掉等号
			else{ s = mid; r = mid-1; }
		}
		return s;
	}
	// longest increasing subsequence
	static int LongestSubsequence( int n, int *a, int *s = NULL ){
		int *v = new int[n], *id = new int[n], len = 0;
		for(int i=0; i<n; i++){
			id[i] = HalfSearch( len, v, a[i] );
			if( id[i] == len ) v[len++] = a[i];
			else v[id[i]] = min( v[id[i]], a[i] );
		}
		if( s != NULL ){
			for(int i=n-1, j=len; i>=0 && j>0; i--)
				if( id[i]==j-1 && (j==len||a[i]<=s[j]) ) s[--j] = a[i];
		}
		delete []v;
		delete []id;
		return len;
	}
	/*static void ans(){ // poj 3670 Eating Together
		int n, a[30008];
		while( scanf("%d",&n)==1 ){
			for(int i=0; i<n; i++) scanf("%d",&a[i]);
			int s1 = LongestSubsequence( n, a );
			for(int i=0; i<n/2; i++) swap( a[i], a[n-1-i] );
			s1 = max( s1, LongestSubsequence( n, a ) );
			printf("%d\n", n-s1 );
		}
	}*/
	/*static void ans(){ // poj 2533 Longest Ordered Subsequence
		int n, a[1008];
		while( scanf("%d",&n)!=EOF ){
			for(int i=0; i<n; i++) scanf("%d",&a[i]);
			printf("%d\n",LongestSubsequence( n, a ) );
		}
	}*/
	/*static void ans(){ // poj 1887 Testing the CATCHER
		int t = 0, n, a[33000];
		while( true ){
			for(n=0, scanf("%d",&a[0]); a[n]!=-1; n++) scanf("%d",&a[n+1]);
			if( n == 0 ) break;
			for(int i=0; i<n/2; i++) swap( a[i], a[n-1-i] );
			printf("Test #%d:\n",++t);
			printf("  maximum possible interceptions: %d\n\n", LongestSubsequence( n, a ));
		}
	}*/
	/*static void ans(){ // poj 1631 Bridging signals
		int t, n, a[40008];
		scanf("%d",&t);
		while( t-- ){
			scanf("%d",&n);
			for(int i=0; i<n; i++) scanf("%d",&a[i]);
			printf("%d\n",LongestSubsequence( n, a ) );
		}
	}*/
	static void ans(){ // poj 3709 K-Anonymous Sequence
		const int N = 500008;
		int t, n, k, *q = new int[N];
		long long *a = new long long[N], *s = new long long[N], *d = new long long[N];
		scanf("%d",&t);
		while( t-- ){
			scanf("%d%d", &n, &k);
			s[0] = 0;
			for(int i=1; i<=n; i++){ scanf("%lld",&a[i]); s[i]=s[i-1]+a[i]; }
			for(int i=k; i<2*k; i++) d[i] = s[i]-a[1]*i;
			int head = 0, tail = 0; q[tail++] = k;
			for(int i=2*k; i<=n; i++){
				while( head<=tail-2 ){
					int x = q[head], y = q[head+1];
					if( d[x]-s[x]-a[x+1]*(i-x)>=d[y]-s[y]-a[y+1]*(i-y) ) head++;
					else break;
				}
				int x = q[head];
				d[i] = d[x]+s[i]-s[x]-a[x+1]*(i-x);
				if( i==n ) continue;

				q[tail++] = i-k+1;
				if( q[head] == i-2*k+1 ) head++;
				while( head<=tail-3 ){
					int x = q[tail-3], y = q[tail-2], z = q[tail-1];
					int t1 = 0, t2 = 0;
					if( a[x+1]==a[y+1] ) t1 = ( d[x]>=d[y] ? y+k:n );
					else{
						t1 = (d[y]-d[x]+s[x]-s[y]+a[y+1]*y-a[x+1]*x + a[y+1]-a[x+1])/(a[y+1]-a[x+1]);
						if( t1<y+k ) t1 = y+k;
					}
					if( a[y+1]==a[z+1] ) t2 = ( d[y]>=d[z] ? z+k:n );
					else{
						t2 = (d[z]-d[y]+s[y]-s[z]+a[z+1]*z-a[y+1]*y + a[z+1]-a[y+1])/(a[z+1]-a[y+1]);
						if( t2<z+k ) t2 = z+k;
					}
					if( t1>=t2 ){ q[tail-2] = q[tail-1]; tail--; }
					else break;
				}
			}
			printf("%lld\n", d[n]);
		}
	}
	/*static void ans(){ // poj 2082 Terrible Sets
		int n, a[50008], h[50008], q[50008];
		while( scanf("%d", &n)==1 && n!=-1 ){
			for(int i=0; i<n; i++) scanf("%d%d", &a[i], &h[i]);
			a[n] = h[n] = 0;
			int res = 0;
			for(int i=0, tail=0; i<=n; i++){
				if( tail==0 || h[i]>=h[i-1] ){ q[tail++] = i; continue; }
				int w = 0, t = 0;
				while( tail>0 && h[q[tail-1]]>h[i] ){
					t = q[--tail];
					w += a[t];
					res = max( res, w*h[t] );
				}
				q[tail++] = i; a[i] += w;
			}
			printf("%d\n", res );
		}
	}*/
	/*static int LCIS( int n, int *a, int m, int *b, int *d, int *s, int *r ){
		memset( d, 0, sizeof(int)*m );
		memset( s, 0, sizeof(int)*m );
		for(int i=0; i<n; i++){
			if( i>0 ) memcpy( d+i*m, d+(i-1)*m, sizeof(int)*m );
			int k = -1;
			for(int j=0; j<m; j++){
				if( a[i]>b[j] && (k==-1||s[j]>s[k]) ) k = j;
				if( a[i]==b[j] && (k==-1||s[k]+1>s[j]) ){
					s[j] = (k==-1 ? 1:s[k]+1);
					d[i*m+j] = i*m+k;
				}
			}
		}
		int mx = 0;
		for(int i=0; i<m; i++) if( s[i]>s[mx] ) mx = i;
		for(int i=(n-1)*m+mx,j=s[mx]-1; j>=0; i=d[i],j--) r[j] = b[i%m];
		return s[mx];
	}
	static void ans(){ // poj 2127 Greatest Common Increasing Subsequence
		const int N = 508;
		int n, m, a[N], b[N], d[N*N], s[N], r[N];
		while( scanf("%d",&n)==1 ){
			for(int i=0; i<n; i++) scanf("%d",&a[i]);
			scanf("%d",&m);
			for(int i=0; i<m; i++) scanf("%d",&b[i]);
			int len = LCIS( n, a, m, b, d, s, r );
			printf("%d\n",len);
			if( len>0 ) printf("%d",r[0]);
			for(int i=1; i<len; i++) printf(" %d",r[i]);
			if( len>0 ) printf("\n");
		}
	}*/

	/*struct Point{
		int s, num;
		Point(int a, int b){ s = a; num = b; }
	};
	static bool cmp( Point s, Point t ){ return ( s.s<t.s || (s.s==t.s&&s.num<t.num) ); }
	static void ans(){ // poj 3063 Sherlock Holmes
		const int N = 10008;
		int n, num, w[N], b[N];
		vector<Point> q, p;
		while( scanf("%d",&n)==1 ){
			scanf("%d", &num);
			int W = 0, B = 0;
			for(int i=0; i<n; i++){
				scanf("%d%d",&w[i],&b[i]);
				W += w[i];
				B += b[i];
			}
			if( n==1 || (B<=W&&W<=B+1) || (W<=B&&B<=W+1) ){
				printf("No solution\n");
				continue;
			}
			bool inv = false;
			if( W < B ){
				inv = true;
				swap( W, B );
				for(int i=0; i<n; i++) swap( w[i], b[i] );
			}
			for(int i=0; i<n; i++) w[i] -= b[i];

			q.clear();
			q.push_back( Point(0,0) );
			int best = -1;
			for(int i=0; i<n; i++){
				p.clear();
				int pn = p.size();
				for(int j=q.size()-1; j>=0; j--){
					int s = q[j].s+w[i];
					if( q[j].num+1==n/2 && 0<s && s<=(W-B)/2 && min(s,W-B-s)>min(best,W-B-best) ) best = s;
					
					if( q[j].num+n-1-i>=n/2 ){
						if( q[j].num+1<n/2 ) p.push_back( Point(s,q[j].num+1) );
						p.push_back( q[j] );
					}
				}
				if( p.size()<1 ) break;
				sort( p.begin(), p.end(), cmp );

				q.clear();
				q.push_back( p[0] );
				for(int j=1,k=p.size(); j<k; j++){
					if( p[j].s!=p[j-1].s || p[j].num!=p[j-1].num ) q.push_back( p[j] );
				}
			}
			if( best==-1 ){ printf("No solution\n"); continue; }
			if( inv ) printf("B ");
			else printf("W ");
			printf("%.2f\n", 100.0*(num*n/2+best)/(num*n) );
		}
		//printf("No solution\n");
	}*/
	/*static void ans(){ // hdu 3480  Division
		const int INF = (1<<28);
		int T, n, m, a[10008], d[2][10008];
		int head, tail, q[10008];
		scanf("%d",&T);
		for(int tt=1; tt<=T; tt++){
			scanf("%d%d",&n,&m);
			for(int i=0; i<n; i++) scanf("%d",&a[i]);
			sort( a, a+n );
			int nn = 0, result = INF;
			for(int i=1; i<n; i++) if( a[i]!=a[nn] ) a[++nn] = a[i];
			if( nn+1<=m ){ printf("Case %d: 0\n",tt); continue; }
			n = nn+1;
			for(int i=1,r=1; i<=m; i++,r=1-r){
				head = tail = 0;
				if( i>1 ) q[tail++] = i-2;
				for(int j=i-1; j<n; j++){
					if( i==1 ){ d[r][j] = (a[j]-a[0])*(a[j]-a[0]); continue; }
					while( head<=tail-2 ){
						int x = q[head], y = q[head+1];
						if( d[1-r][x]+(a[y+1]-a[x+1])*(2*a[j]-a[x+1]-a[y+1])>=d[1-r][y] ) head++;
						else break;
					}
					int x = q[head];
					d[r][j] = d[1-r][x]+(a[j]-a[x+1])*(a[j]-a[x+1]);
					if( j==n-1 ) continue;
					
					q[tail++] = j;
					while( head<=tail-3 ){
						int x = q[tail-3], y = q[tail-2], z = q[tail-1];
						int yx = a[y+1]-a[x+1], zy = a[z+1]-a[y+1];
						if( (d[1-r][y]-d[1-r][x])*zy>=(d[1-r][z]-d[1-r][y])*yx
							+(a[z+1]-a[x+1])*yx*zy ){
							q[tail-2] = q[tail-1]; tail--;
						}
						else break;
					}
				}
				if( i==m ) result = d[r][n-1];
			}
			printf("Case %d: %d\n",tt,result);
		}
	}*/
};

int main(){
	Sequence::ans();
	return 0;
}
