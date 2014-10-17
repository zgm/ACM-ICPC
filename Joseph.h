#include<iostream>
using namespace std;

class Joseph{
public:
	// 从1开始的约瑟夫问题求解  
	static void next( int n, int m ){
		for(int i=0,p; ++i<=n; ){
			p = i*m;
			while( p>n ) p = p-n+(p-n-1)/(m-1);
			printf("%d ", p);
		}printf("\n");
	}
};

int main(){
	int n, m;
	while( scanf("%d%d",&n,&m)==2 ){
		Joseph::next( n, m );
	}
	return 0;
}