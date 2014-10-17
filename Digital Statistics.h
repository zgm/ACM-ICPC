#include<iostream>
using namespace std;

class Amount{
public:
	// 统计[0...x]内二进制表示含k个1的数的个数
	static int Calculate( int x, int k, int f[32][32] ){
		if( x < 0 ) return 0;
		int n = 0, res = 0;
		for(int i=31; i>=0; i--){
			if( x&(1<<i) ){
				if( i>=k-n ) res += f[i][k-n];
				if( ++n > k ) break;
				x ^= (1<<i);
			}
		}
		if( n+x == k ) ++res;
		return res;
	}
	/*static void ans(){ // ural 1057: Amount of degrees
		int f[32][32];
		for(int i=0; i<32; i++){
			f[i][0] = f[i][i] = 1;
			for(int j=1; j<i; j++) f[i][j] = f[i-1][j]+f[i-1][j-1];
		}
		int x, y, k, b, s[32];
		while( scanf("%d%d%d%d",&x,&y,&k,&b)==4 ){
			--x;
			for(int i=0; i<32; i++){ s[i] = x%b; x /= b; }
			for(int i=31; i>=0; i--){
				if( s[i]>0 ) x |= (1<<i);
				if( s[i]>1 ){ x += (1<<i)-1; break; }
			}
			for(int i=0; i<32; i++){ s[i] = y%b; y /= b; }
			for(int i=31; i>=0; i--){
				if( s[i]>0) y |= (1<<i);
				if( s[i]>1 ){ y += (1<<i)-1; break; }
			}
			//printf("%d %d\n",x,y);
			printf("%d\n", Calculate( y, k, f )-Calculate( x, k, f ) );
		}
	}*/
	static void ans(){
		int f[32][32];
		for(int i=0; i<32; i++){
			f[i][0] = f[i][i] = 1;
			for(int j=1; j<i; j++) f[i][j] = f[i-1][j]+f[i-1][j-1];
		}
		int m, n, k;
		while( scanf("%d%d%d",&m,&n,&k)==3 ){
			//for(int i=31; i>=0; i--) printf("%d",(n&(1<<i))!=0); printf("\n");
			//for(int i=31; i>=0; i--) printf("%d",(m&(1<<i))!=0); printf("\n");
			if( n > 0 ){
				int i = 0, l = m, r = n, mid, res;
				for( ; ; i++){
					int num = Calculate( n, i, f )-Calculate( m-1, i, f );
					if( k > num ) k -= num;
					else break;
				}
				k += Calculate( m-1, i, f );
				while( l<=r ){
					mid = (l+r)/2;
					if( Calculate( mid, i, f )>=k ){ res = mid; r = mid-1; }
					else l = mid+1;
				}
				printf("%d\n", res);
			}
			else{
			}
		}
	}
};

int main(){
	Amount::ans();
}