#include<stdlib.h>
#include<stdio.h>
#include<cmath>
#include<math.h>
#include<iostream>
using namespace std;

class KM{
public:
	static void agument( int v, int *aM, int *bM, int *pre ){
		for(int u; v!=-1; v=u){
			u = aM[pre[v]];
			bM[v] = pre[v];
			aM[pre[v]] = v;
		}
	}
	static bool bfs( int n, int *ph, int *a, int *b, int *aM, int *bM, bool *fA, bool *fB, int *slack, int *pre, int &head, int &tail, int *q){
		for(int v,u; head<tail; ){
			v = q[head++], u = v>>1;
			if( v&1 ){
				if( bM[u]==-1 ){ agument( u, aM, bM, pre ); return true; }
				fA[bM[u]] = true; q[tail++] = (bM[u]<<1);
			}
			else{
				for(int i=0,d; i<n; i++){
					if( fB[i] ) continue;
					if( a[u]+b[i] != ph[u*n+i] ){
						d = a[u]+b[i]-ph[n*u+i];
						if( slack[i]>d ){ slack[i] = d; pre[i] = u; }
					}
					else{ fB[i] = true; pre[i] = u; q[tail++] = ((i<<1)|1); }
				}
			}
		}
		return false;
	}
	static int km( int n, int *ph, const int INF=(1<<26) ){
		int *a = new int[6*n], *b = a+n, *aM = a+2*n, *bM = a+3*n;
		int *slack = a+4*n, *pre = a+5*n, *q = new int[2*n], result = 0;

		for(int i=0; i<n; i++){
			a[i] = ph[n*i];
			for(int j=0; j<n; j++) a[i] = max( a[i], ph[n*i+j] );
		}
		memset( b, 0, sizeof(int)*n );
		memset( aM, -1, sizeof(int)*2*n );

		bool agu = true, *fA = new bool[2*n], *fB = fA+n;
		for(int k=0,head,tail; k<n; ){
			if( agu ){
				memset( fA, 0, sizeof(bool)*2*n );
				for(int i=0; i<n; i++) slack[i] = INF;
				head = tail = 0; fA[k] = true; q[tail++] = (k<<1);
			}
			agu = bfs( n, ph, a, b, aM, bM, fA, fB, slack, pre, head, tail, q);
			if( agu ){ k++; continue; }

			int md = INF;
			for(int i=0; i<n; i++) if( !fB[i] ) md = min( md, slack[i] );
			for(int i=0; i<n; i++){
				if( fA[i] ) a[i] -= md;
				if( fB[i] ) b[i] += md;
				slack[i] -= md;
			}
			for(int i=0; i<n; i++){
				if( !fB[i] && slack[i]==0 ){ fB[i]=true; q[tail++]=((i<<1)|1); }
			}
		}
		for(int i=0; i<n; i++) result += ph[i*n+aM[i]];
		delete []a;
		delete []q;
		delete []fA;
		return result;
	}
	static void ans(){ // poj 2195 Going Home
		const int N = 128;
		int hh, ww, h[N], c[N];
		char line[N];
		while( scanf("%d%d",&hh,&ww)==2 && ww!=0 ){
			int n = 0, cn = 0;
			for(int i=0; i<hh; i++){
				scanf("%s",line);
				for(int j=0; j<ww; j++){
					if( line[j] == 'H' ) h[n++] = i*N+j;
					if( line[j] == 'm' ) c[cn++] = i*N+j;
				}
			}
			int *ph = new int[n*n];
			for(int i=0; i<n; i++){
				int cr = c[i]/N, cc = c[i]%N;
				for(int j=0; j<n; j++){
					int hr = h[j]/N, hc = h[j]%N;
					ph[i*n+j] = -abs(cr-hr)-abs(cc-hc);
				}
			}
			printf("%d\n", -km( n, ph ) );
			delete []ph;
		}
	}
};

int main(){
	KM::ans();
	return 0;
}