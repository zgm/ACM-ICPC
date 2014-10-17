#include<stdlib.h>
#include<stdio.h>
#include<iostream>
#include<map>
#include<set>
using namespace std;

struct Link{
	int v, next;
	void set( int V=-1, int Next=-1){ v=V; next=Next; }
};
class List{
public:
	int end; Link *V;
	List( int size = 1024 ){ V = new Link[size]; }
	~List(){ delete []V; }	
	void init( int n ){ end = n; for(int i=0; i<n; i++) V[i].next = -1; }
	void insert( int u, int v ){ V[end].set( v, V[u].next ); V[u].next = end++; }
};

struct Node{
	int num, id;
	Node(int Num, int Id ){ num = Num; id = Id; }
	bool operator<(const Node& t)const{ return ( num>t.num || (num==t.num && id<t.id) ); }
};
void Cardy( int n, int *label, int *inv, Link *V, bool *vis, int *num ){
	set<Node> st;
	set<Node>::iterator it;
	memset( vis, 0, sizeof(bool)*n );
	memset( num, 0, sizeof(int)*n );
	st.insert( Node(0,0) );
	vis[0] = true;
	for(int i=n-1; i>=0; i--){
		if( st.size()==0 ){
			for(int j=0; j<n; j++) if( !vis[j] ) st.insert( Node(0,j) );
		}
		it = st.begin();
		int v = it->id;
		label[v] = i; inv[i] = v; vis[v] = true;
		st.erase( it );
		for(int j=V[v].next; j!=-1; j=V[j].next){
			v = V[j].v;
			if( vis[v] ) continue;
			it = st.find( Node(num[v],v) );
			if( it!=st.end() ) st.erase( it );
			num[v] += 1;
			st.insert( Node(num[v],v) );
		}
	}
}
void Cardy( int n, int *label, int *inv, Link *V, bool *vis ){ // 居然比上一个更快
	map<int,int> mp;
	map<int,int>::iterator it;
	mp.insert( make_pair(0,0) );
	memset( vis, 0, sizeof(bool)*n );
	for(int i=n-1; i>=0; i--){
		int v = -1, num = -1;
		for(it=mp.begin(); it!=mp.end(); ++it)
			if( it->second>num ){ v = it->first; num = it->second; }
		
		label[v] = i; inv[i] = v; vis[v] = true;
		mp.erase( mp.find(v) );
		for(int j=V[v].next; j!=-1; j=V[j].next){
			v = V[j].v;
			if( vis[v] ) continue;
			it = mp.find( v );
			if( it==mp.end() ) mp.insert( make_pair(v,1) );
			else it->second += 1;
		}
	}
}
bool PerfectOrder( int n, int *label, int *inv, Link *V, int *id ){
	memset( id, -1, sizeof(int)*n );
	for(int i=0; i<=n-2; i++){
		int v = inv[i], w, m = n;
		for(int j=V[v].next; j!=-1; j=V[j].next){
			w = V[j].v;
			if( label[w]>label[v] && label[w]<m ) m = label[w];
		}
		m = inv[m];
		for(int j=V[m].next; j!=-1; j=V[j].next) id[V[j].v] = i;
		for(int j=V[v].next; j!=-1; j=V[j].next){
			v = V[j].v;
			if( label[m]<label[v] && id[v]!=i ) return false;
		}
	}
	return true;
}

int main(){ // honi  1006  神奇的国度
	const int N = 10008;
	int n, m, label[N], inv[N];
	bool vis[N]; int num[N], color[N], hash[N];
	List s( 3000000 );
	while( scanf("%d%d",&n,&m)==2 ){
		s.init( n );
		for(int i=0,u,v; i<m; i++){
			scanf("%d%d",&u,&v);
			s.insert( u-1, v-1 );
			s.insert( v-1, u-1 );
		}
		Cardy( n, label, inv, s.V, vis, num );
		
		int result = 1;
		memset( color, 0, sizeof(int)*n );
		memset( hash, 0, sizeof(int)*n );
		color[inv[n-1]] = 1;
		for(int i=n-2,idx=0; i>=0; i--){
			idx++;
			int v = inv[i];
			for(int j=s.V[v].next; j!=-1; j=s.V[j].next)
				hash[color[s.V[j].v]] = idx;
			for(int j=1; ;j++){
				if( hash[j]!=idx ){
					color[v] = j;
					if( j>result ) result = j;
					break;
				}
			}
		}
		printf("%d\n",result);
	}
	return 0;
}

/*int main(){  // zoj 1015  Fishing Net
	const int N = 1008;
	int n, m, label[N], inv[N];
	bool vis[N]; int num[N];
	List s( 1000000 );
	while( scanf("%d%d",&n,&m)==2 && n+m ){
		s.init( n );
		for(int i=0,u,v; i<m; i++){
			scanf("%d%d",&u,&v);
			s.insert( u-1, v-1 );
			s.insert( v-1, u-1 );
		}
		Cardy( n, label, inv, s.V, vis, num );
		if( PerfectOrder(n,label,inv,s.V,num) ) printf("Perfect\n");
		else printf("Imperfect\n");
		printf("\n");
	}
	return 0;
}*/
