#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<algorithm>
#include<iostream>
using namespace std;


class Sort{
public:
	static void SelectSort( int n, int *a ){
		for(int i=0; i<n; i++){
			int s = n-1;
			for(int j=n-2; j>=i; j--) if( a[j]<a[s] ) s = j;
			swap( a[i], a[s] );
		}
	}
	static void InsertSort( int n, int *a ){
		for(int i=1; i<n; i++){
			for(int j=i; j>0 && a[j-1]>a[j]; j--) swap( a[j-1], a[j] );
		}
	}
	static void BubbleSort( int n, int *a ){
		for(int i=0; i<n; i++){
			for(int j=n-1; j>=i+1; j--)
				if( a[j]<a[j-1] ) swap( a[j], a[j-1] );
		}
	}
	static void ShellSort( int n, int *a ){
	}
	static void MergeSort( int s, int t, int *a ){
		if( s<t ){
			int mid = (s+t)/2;
			MergeSort( s, mid, a );
			MergeSort( mid+1, t, a );
			//Merge( s, mid, a, mid+1, t, a );
		}
	}

	static void Down( int t, int n, int *a ){
		for(int i=2*(t+1)-1; i<n; i=2*(i+1)-1){
			if( i<=n-2 && a[i]<a[i+1] ) i++;
			if( a[i]>a[(i+1)/2-1] ) swap( a[i], a[(i+1)/2-1] );
			else break;
		}
	}
	static void HeapSort( int n, int *a ){
		for(int i=n/2; i>=0; i--) Down( i, n, a );
		for(int i=n-1; i>=1; i--){
			swap( a[0], a[i] );
			Down( 0, i, a );
		}
	}

	static int Partition( int p, int r, int *a ){
		int x = a[r], i = p-1;
		for(int j=p; j<r; j++){
			if( a[j]<=x ){ i++; swap( a[i], a[j] ); }
		}
		swap( a[i+1], a[r] );
		return i+1;
	}
	static void QuickSort( int p, int r, int *a ){
		if( p<r ){
			int q = Partition( p, r, a );
			QuickSort( p, q-1, a );
			QuickSort( q+1, r, a );
		}
	}

	static void Show( int n, int *a ){
		for(int i=0; i<n; i++) printf("%d ",a[i]); printf("\n");
	}
	static void ans(){
		int n, a[10000], b[10000];
		srand((unsigned)time(NULL));
		while( scanf("%d",&n)==1 ){
			for(int i=0; i<n; i++){ a[i] = b[i] = rand(); }
			Show( n, a );
			//InsertSort( n, a );
			HeapSort( n, a );
			//QuickSort( 0, n-1, a );
			Show( n, a );
			sort( b, b+n ); Show( n, b );
			bool ok = true;
			for(int i=0; i<n; i++) if( a[i]!=b[i] ) ok = false;
			if( !ok ) printf("Wrong!\n");
			else printf("ok!\n");
		}
	}
};

/*int main(){
	Sort::ans();
	return 0;
}*/
