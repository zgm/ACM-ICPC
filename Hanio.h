#include<iostream>
#include<algorithm>
#include<cmath>
#include<math.h>
using namespace std;
const double eps = 1e-8;

struct Node{ char c; double x, y; }first;

class Hanio{
public:
	static bool cmp( Node s, Node t ){ return (t.x-first.x)*s.y-(s.x-first.x)*t.y<0; }
	static void ans(){
		int n, res[10008]; char s[108];
		double x[10008]; Node p[108];
		while( scanf("%d",&n)==1 ){
			scanf("%s",s);
			for(int i=0; i<n; i++){
				getchar();			
				scanf("%c%lf%lf",&p[i].c,&p[i].x,&p[i].y);
			}
			int k = 0, t = 0;
			for(int i=0; i<n; i++){
				for(int j=i+1; j<n; j++){
					if( p[i].y == p[j].y ) continue;
					if( p[i].x == p[j].x ) x[k++] = p[i].x;
					else x[k++] = p[i].x-p[i].y/((p[j].y-p[i].y)/(p[j].x-p[i].x));
				}
			}
			x[k++] = -1e10; x[k++] = 1e10;
			sort( x, x+k );
			first.y = 0;
			for(int i=1, j; i<k; i++){
				if( fabs(x[i]-x[i-1])<eps ) continue;
				if( i==1 ) first.x = x[i-1];
				else first.x = (x[i]+x[i-1])/2;
				sort( p, p+n, cmp );
				for(j=0; j<n; j++) if( s[j]!=p[n-1-j].c ) break;
				if( j>=n ) res[t++] = i;
			}
			printf("%d\n",t);
			for(int i=0; i<t; i++){
				if( i!=0 ) printf(" ");
				if( x[res[i]-1]<-1e8 ) printf("* ");
				else printf("%.9lf ",x[res[i]-1]);
				if( x[res[i]]>1e8 ) printf("*");
				else printf("%.9lf",x[res[i]]);
			}
			if( t!=0 ) printf("\n");
		}
	}
};

int main(){
	Hanio::ans();
	return 0;
}