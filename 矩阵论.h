#include<iostream>
#include<cmath>
#include<math.h>
using namespace std;

const double eps = 1e-8;

class Plural{
private:
	double a, b;
public:
	Plural(double A=0, double B=0){ a = A; b = B; }
	Plural( const Plural &t ){ a = t.a; b = t.b; }
	
	Plural Add( Plural s ){ return Plural( a+s.a, b+s.b ); }
	Plural Sub( Plural s ){ return Plural( a-s.a, b-s.b ); }
	Plural Mul( Plural s ){ return Plural( a*s.a-b*s.b, b*s.a+a*s.b ); }
	Plural Div( Plural s ){
		double d = s.a*s.a+s.b*s.b;
		return Plural(  (s.a*a+s.b*b)/d, (s.a*b-s.b*a)/d);
	}
	double Modulus(){ return sqrt(a*a+b*b); }
	double Modulus2(){ return a*a+b*b; }
	void Conjugate(){ b = -b; }
	
	void Input(){ scanf("%lf%lf",&a,&b); }
	void Output(){ printf("%.4f %.4f\n",a,b); }
};

class Matrix{
public:
	// B = A*B
    static void Mul( int n, double *A, double *B, double *tmp ){
		memset( tmp, 0, sizeof(double)*n*n );
		double *a = A, *t = tmp; 
		for(int i=0; i<n; i++, a+=n, t+=n){
            double *b = B;
			for(int j=0; j<n; j++, b+=n){
				if( a[j] == 0 ) continue;
				for(int k=0; k<n; k++) t[k] = t[k]+a[j]*b[k];
			}
		}
		memcpy( B, tmp, sizeof(double)*n*n );
	}
	// p存放a的特征多项式的系数
    static void Poly( int n, double *a, double *p, double *inv=NULL ){
        double *b = new double[2*n*n];
        memcpy( b, a, sizeof(double)*n*n );
        p[0] = -1; p[1] = 0;
        for(int i=0; i<n; i++) p[1] += b[i*n+i];
        for(int i=2; i<=n; i++){
            for(int j=0; j<n; j++) b[j*n+j] -= p[i-1];
            if( i==n-1 && inv!=NULL ){
                memcpy( inv, b, sizeof(double)*n*n );
            }
            Mul( n, a, b, b+n*n );
            p[i] = 0;
            for(int j=0; j<n; j++) p[i] += b[j*n+j]/i;
            if( i==n-1 && inv!=NULL ){
                for(int j=0; j<n*n; j++) inv[j] /= p[i];
            }
        }
        for(int i=0; i<=n; i++) p[i] = -p[i];
        delete []b;
    }
	static bool Equal( double a, double b ){ return fabs(a-b)<eps; }
	// retur k1*a==k2*b
	static bool Equal( int n, int m, double k1, double *a, double k2, double *b ){
		for(int i=n*m-1; i>=0; i--) if( !Equal(a[i],b[i]) ) return false;
		return true;
	}
	// get a vector u where u'x = 0
	static void ZeroVector( int n, double *x, double *u ){
		memset( u, 0, sizeof(double)*n );
		u[0] = 1;
		double s = u[0]*x[0];
		for(int i=1; i<n && !Equal(s,0); i++){
			if( !Equal(x[i],0) ){ u[i] = s/x[i]; return; }
		}
		if( !Equal(s,0) ){ u[0] = 0; u[n-1] = 1; }
	}
	static double Norm2( int n, double *x ){
		double s = 0;
		for(int i=0; i<n; i++) s += x[i]*x[i];
		return sqrt(s);
	}
	// return H where Hx = az while |a| = ||x||2
	static void Householder( int n, double *x, double *z, double *H ){
		double a = Norm2( n, x );
		memset( H, 0, sizeof(double)*n*n );
		for(int i=0; i<n; i++) H[i*n+i] = 1;
		if( fabs(a)<eps ) H[0] = -1;
		else{
			double *u = new double[n];
			// x = az
			if( Equal(n,1,1,x,a,z) ) ZeroVector( n, x, u );
			else for(int i=0; i<n; i++) u[i] = x[i]-a*z[i];
			a = Norm2( n, u );
			for(int i=0; i<n; i++) u[i] /= a;
			for(int i=0; i<n; i++){
				for(int j=0; j<n; j++) H[i*n+j] -= 2*u[i]*u[j];
			}
			delete []u;
		}
	}
	// [Q,R] = QR( A )
	static void QR( int n, double *A, double *Q, double *R ){
		double *H = new double[2*n*n+2*n];
		double *x = H+n*n, *e = x+n, *tmp = e+n;
		memcpy( R, A, sizeof(double)*n*n );
		memset( e, 0, sizeof(double)*n );
		e[0] = 1;
		for(int i=0; i<n-1; i++){
			for(int j=i; j<n; j++) x[j] = R[j*n+i];
			Householder( n-i, x+i, e, H );
			for(int j=n-1-i; j>=0; j--){
				for(int k=n-1-i; k>=0; k--) H[(j+i)*n+k+i] = H[j*(n-i)+k];
			}
			for(int j=0; j<i; j++){
				memset( H+j*n, 0, sizeof(double)*n );
				for(int k=i; k<n; k++) H[k*n+j] = 0;
			}
			for(int j=0; j<i; j++) H[j*n+j] = 1;
			Mul( n, H, R, tmp );
			if( i!=0 ) Mul( n, Q, H, tmp );
			memcpy( Q, H, sizeof(double)*n*n );
		}
		delete []H;
	}
	// p存放a的特征值
	static void Eig( int n, double *a, double *p ){
		double *t = new double[3*n*n+n];
		double *Q = t+n*n, *R = Q+n*n, *f = R+n*n;
		const int MaxIter = 10000;
		for(int i=0; i<MaxIter; i++){
			QR( n, a, Q, R );
			Mul( n, R, Q, t );
			if( i!=0 ){
				double s = 0, sum = 0;
				for(int j=0; j<n; j++){
					s += (p[j]-Q[j*n+j])*(p[j]-Q[j*n+j]);
					sum += p[j]*p[j];
				}
				if( s/sum<1e-6 ) break;
			}
			for(int j=0; j<n; j++) p[j] = Q[j*n+j];
			memcpy( a, Q, sizeof(double)*n*n );
			Show( n, n, a );
		}
		delete []t;
	}
	static void Show( int n, int m, double *a ){
		for(int i=0; i<n; i++){
            for(int j=0; j<m; j++) printf("%.4f ",a[i*m+j]);
            printf("\n");
        }printf("\n");
	}
	
	// 复数向量的1范数
	static double Norm1( int n, Plural *a ){
		double s = 0;
		for(int i=0; i<n; i++) s += a[i].Modulus();
		return s;
	}
	// 复数向量的2范数
	static double Norm2( int n, Plural *a ){
		double s = 0;
		for(int i=0; i<n; i++) s += a[i].Modulus2();
		return sqrt( s );
	}
	// 复数向量的无穷范数
	static double NormINF( int n, Plural *a ){
		double s = 0;
		for(int i=0; i<n; i++) s = max( s, a[i].Modulus() );
		return s;
	}

	// 复数矩阵的m1范数
	static double Norm_m1( int n, int m, Plural *a ){
		double s = 0;
		for(int i=n*m-1; i>=0; i--) s += a[i].Modulus();
		return s;
	}
	// 复数矩阵的F范数
	static double Norm_F( int n, int m, Plural *a ){
		double s = 0;
		for(int i=n*m-1; i>=0; i--) s += a[i].Modulus2();
		return sqrt( s );
	}
	// 复数矩阵的m无穷范数
	static double Norm_mINF( int n, Plural *a ){
		double s = 0;
		for(int i=n*n-1; i>=0; i--) s = max( s, a[i].Modulus() );
		return n*s;
	}
	// 复数矩阵的1范数
	static double Norm_1( int n, int m, Plural *a ){
		double s = 0;
		for(int j=0; j<m; j++){
			double t = 0;
			for(int i=0; i<n; i++) t += a[i*m+j].Modulus();
			s = max( s, t );
		}
		return s;
	}
	// 复数矩阵的无穷范数
	static double Norm_INF( int n, int m, Plural *a ){
		double s = 0;
		for(int i=0; i<n; i++){
			double t = 0;
			for(int j=0; j<m; j++) t += a[i*m+j].Modulus();
			s = max( s, t );
		}
		return s;
	}
};

/*int main(){
	const int N = 10000;
    int n, m;
	Plural a[N];
	while( scanf("%d%d",&n,&m)==2 ){
		for(int i=0; i<n*m; i++) a[i].Input();
		printf("%.6f\n", Matrix::Norm_m1( n, m, a ) );
		printf("%.6f\n", Matrix::Norm_F( n, m, a ) );
		printf("%.6f\n", Matrix::Norm_mINF( n, a ) );
		printf("%.6f\n", Matrix::Norm_1( n, m, a ) );
		printf("%.6f\n", Matrix::Norm_INF( n, m, a ) );
	}
    return 0;
}*/
