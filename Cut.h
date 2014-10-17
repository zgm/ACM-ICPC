#include<iostream>
#include<stack>
using namespace std;

struct Link{
	int v, next; bool vis;
	void set( int V = -1, int Next = -1 ){ v = V; next = Next; vis = false; }
};
class List{
public:
	int end; Link *V;
	List( int size = 1024 ){ V = new Link[size]; }
	~List(){ delete []V; }
	void init( int n ){ end = (n&1 ? n+1:n); for(int i=0; i<n; i++) V[i].next = -1; }
	void insert( int u, int v ){
		V[end].set( v, V[u].next );
		V[u].next = end++;
		V[end].set( u, V[v].next );
		V[v].next = end++;
	}
	void clear(){
		for(int i=0; i<end; i++) V[i].vis = false;
	}
};

class Cut{
public:
	static void CutPoint( int v, int &sign, bool *vis, int *dfn, int *low, Link *V, int *num, int forbid=-1 ){
        dfn[v] = low[v] = (++sign);
        vis[v] = true;
        for(int i=V[v].next; i!=-1; i=V[i].next){
			if( V[i].v == forbid ) continue;
            int u = V[i].v;
			if( V[i^1].vis ) continue;
            if( !vis[u] ){
				V[i].vis = true;
                CutPoint( u, sign, vis, dfn, low, V, num, forbid );
                low[v] = min( low[v], low[u] );
                if( low[u] >= dfn[v] ) num[v]++;
            }
            else low[v] = min( low[v], dfn[u] );
        }
    }
    static void CutEdge( int v, int &sign, bool *vis, int *dfn, int *low, Link *V, int &num ){ // warning: No Test
        dfn[v] = low[v] = (++sign);
        vis[v] = true;
        for(int i=V[v].next; i!=-1; i=V[i].next){
            int u = V[i].v;
			if( V[i].vis ) continue;
            if( !vis[u] ){
				V[i].vis = true;
                CutEdge( u, sign, vis, dfn, low, V, num );
                low[v] = min( low[v], low[u] );
                if( low[u] > dfn[v] ) num++;
            }
            else low[v] = min( low[v], dfn[u] );
        }
    }
    static void Block( int v, int &sign, bool *vis, int *dfn, int *low, Link *V, int &num, stack<int>&s, int *id ){
        dfn[v] = low[v] = (++sign);
        vis[v] = true;
        s.push( v );
        for(int i=V[v].next; i!=-1; i=V[i].next){
            int u = V[i].v;
			if( V[i^1].vis ) continue;
            if( !vis[u] ){
				V[i].vis = true;
                Block( u, sign, vis, dfn, low, V, num, s, id );
                low[v] = min( low[v], low[u] );
            }
            else low[v] = min( low[v], dfn[u] );
        }
        if( low[v] == dfn[v] ){
			while( !s.empty() ){
				id[s.top()] = num;
				if( s.top() == v ){ s.pop(); break; }
				s.pop();
			}
            num++;
        }
    }
	static void ans(){ // 2008 hefei  Necklace
		const int N = 10001, M = 1000001;
		int cs = 0, n, m, dfn[N], low[N], id[N];
		bool vis[N];
		List s( N+2*M );
		while( scanf("%d%d",&n,&m)==2 && n+m ){
			int u, v;
			s.init( n );
			for(int i=0; i<m; i++){
				scanf("%d%d",&u,&v);
				s.insert( u-1, v-1 );
			}
			scanf("%d%d",&u,&v);
			u--; v--;
			int sign = 0, num = 0;
			memset( vis, 0, sizeof(bool)*n );
			memset( id, -1, sizeof(int)*n );
			stack<int> st;
			Block( u, sign, vis, dfn, low, s.V, num, st, id );
			printf("Case %d: ",++cs);
			if( id[u]==id[v] ) printf("YES\n");
			else printf("NO\n");
		}
	}
	/*static void ans(){ // poj 3713 Transferring Sylla
		const int N = 508, M = 20008;
		int n, m, dfn[N], low[N], num[N];
		bool vis[N];
		List s( N+2*M );
		while( scanf("%d%d",&n,&m)==2 && n+m!=0 ){
			s.init( n );
			for(int i=0, u, v; i<m; i++){
				scanf("%d%d",&u,&v);
				s.insert( u, v );
			}
			bool ok = true;
			for(int i=0; i<n; i++){
				int sign = 0, res = 0, root = 0;
				memset( num, 0, sizeof(int)*n );
				memset( vis, 0, sizeof(bool)*n );
				if( i==0 ) root =1;
				num[root]--;
				CutPoint( root, sign, vis, dfn, low, s.V, num, i );
				s.clear();
				for(int j=0; j<n; j++) if( j!=i && (!vis[j] || num[j]>0) ) res++;
				if( res != 0 ){ printf("NO\n"); break; }
				else if( i == n-1 ) printf("YES\n");
			}
		}
	}*/
    /*static void ans(){ // poj 1144 Network
        const int N = 108;
        int dfn[N], low[N], num[N], n;
        bool vis[N];
        while( scanf("%d",&n)!=EOF && n!=0 ){
            List s( n*n );
            s.init( n );
            memset( num, 0, sizeof(int)*n );
            memset( vis, 0, sizeof(bool)*n );
            int i, j, k;
            while( scanf("%d",&j)!=EOF && j!=0 ){
                while( getchar()!='\n' ){
                    scanf("%d",&k);
                    s.insert( j-1, k-1 );
                }
            }
            int sign = 0, res = 0;
            CutPoint( 0, sign, vis, dfn, low, s.V, num );
            if( num[0] > 0 ) num[0] --;
            for(i=0; i<n; i++) if( num[i] > 0 ) res++;
            printf("%d\n",res);
        }
    }*/
	/*static void ans(){ // poj 3177 Redundant Paths
        const int N = 5008;
        int dfn[N], low[N], n, m, id[N], sum[N];
        bool vis[N];
        stack<int> st;
        while( scanf("%d%d",&n,&m)!=EOF && n!=0 ){
            List s( n+2*m+8 );
            s.init( n );
            int i, j, k;
            memset( vis, 0, sizeof(bool)*n );
            memset( sum, 0, sizeof(int)*n );
            for(i=0; i<m; i++){
                scanf("%d%d",&j,&k);
                s.insert( j-1, k-1 );
            }
            int sign = 0, num = 0, res = 0;
            while( !st.empty() ) st.pop();
            Block( 0, sign, vis, dfn, low, s.V, num, st, id );
            if( num == 1 ){ printf("0\n"); continue; }
            for(i=0; i<n; i++){
                for(j=s.V[i].next; j!=-1; j=s.V[j].next){
                    k = s.V[j].v;
                    if( id[i] == id[k] ) continue;
                    k = max( id[i], id[k] );
                    sum[k]++;
                }
            }
            for(i=0; i<num; i++) if( sum[i] == 0 ) res++;
            if( sum[id[0]] == 2 ) res++;
            printf("%d\n",(res+1)/2);
        }
    }*/
};

int main(){
	Cut::ans();
    return 0;
}