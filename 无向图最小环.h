#include<iostream>
#include<stack>
using namespace std;
const int INF = (1<<28);
const int N = 101;

int main(){ // poj 1734 Sightseeing trip
	int n, m, u, v, c, d[N][N], w[N][N], pre[N][N];
	while( scanf("%d%d",&n,&m)==2 ){
		for(int i=1; i<=n; i++){
			for(int j=1; j<=n; j++) d[i][j] = INF;
			d[i][i] = 0;
		}
		memset( pre, -1, sizeof(pre) );
		for(int i=0; i<m; i++){
			scanf("%d%d%d",&u,&v,&c);
			if( d[u][v]>c ){
				d[u][v] = d[v][u] = c;
				pre[u][v] = v; pre[v][u] = u;
			}
		}
		int res = INF;
		for(int i=1; i<=n; i++){
			for(int j=1; j<=n; j++) w[i][j] = d[i][j];
		}
		int ans[N], size = 0, p;
		for(int k=1; k<=n; k++){
			for(int i=1; i<k; i++){
				for(int j=i+1; j<k; j++){
					if( res > w[i][j]+d[i][k]+d[k][j] ){
						res = w[i][j]+d[i][k]+d[k][j];
						size = 0; ans[0] = k; p = i;
						while( p!=-1 ){ ans[++size] = p; p = pre[p][j]; }
					}
				}
			}
			for(int i=1; i<=n; i++){
				for(int j=1; j<=n; j++){
					if( w[i][j] > w[i][k]+w[k][j] ){
						w[i][j] = w[i][k]+w[k][j];
						pre[i][j] = pre[i][k];
					}
				}
			}
		}
		if( res == INF ) printf("No solution.\n");
		else{
			for(int i=0; i<size; i++) printf("%d ",ans[i]);
			printf("%d\n", ans[size]);
		}
	}
	return 0;
}