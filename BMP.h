#include<iostream>
#include<stdlib.h>
#include<stdio.h>
#include<cstring>
#include<string.h>
#include<algorithm>
#include<math.h>
#include<cmath>
#include<time.h>
#include<assert.h>
#include<stdarg.h>
using namespace std;


void write8(FILE *f, int x){
	unsigned char z = (unsigned char) x;
	fwrite(&z,1,1,f);
}
void writefv(FILE *f, char *fmt, va_list v){
	unsigned char x;  unsigned short y;  unsigned int z;
	while( *fmt ){
		switch ( *fmt++ ){
		case ' ': break;
		case '1': x = va_arg(v, int); write8(f,x); break;
		case '2': y = va_arg(v, int); write8(f,y); write8(f,y>>8); break;
		case '4': z = va_arg(v, int); write8(f,z);  write8(f,z>>8);
			write8(f,z>>16);  write8(f,z>>24);  break;
		default: assert( 0 ); va_end( v ); return;
      }
   }
}
void writef(FILE *f, char *fmt, ...){
	va_list v;
	va_start( v, fmt );
	writefv( f, fmt,v );
	va_end( v );
}
void write_pixels(FILE *f, int rgb_dir, int vdir, int x, int y, int comp, void *data,
				  int write_alpha, int scanline_pad ){
	unsigned char bg[3] = { 255, 0, 255}, px[3];
	unsigned int zero = 0;
	int i,j,k, j_end;

	if( vdir < 0 )  j_end = -1, j = y-1;
	else  j_end =  y, j = 0;
	
	for( ;j!=j_end; j+=vdir){
		for( i=0; i<x; ++i){
			unsigned char *d = (unsigned char *) data + (j*x+i)*comp;
			if( write_alpha < 0 ) fwrite( &d[comp-1], 1, 1, f );
			switch( comp ){
				case 1:
				case 2: writef(f, "111", d[0],d[0],d[0]);  break;
				case 4:
					if( !write_alpha ){
						for (k=0; k < 3; ++k)
							px[k] = bg[k] + ((d[k] - bg[k]) * d[3])/255;
						writef(f, "111", px[1-rgb_dir],px[1],px[1+rgb_dir]);
						break;
					}
					/* FALLTHROUGH */
				case 3:  writef(f, "111", d[1-rgb_dir],d[1],d[1+rgb_dir]);  break;
			}
			if( write_alpha > 0 ) fwrite( &d[comp-1], 1, 1, f );
		}
		fwrite(&zero,scanline_pad,1,f);
	}
}
bool outfile(char *filename, int rgb_dir, int vdir, int x, int y, int comp, void *data,
			 int alpha, int pad, char *fmt, ...){
	FILE *f = fopen( filename, "wb" );
	if( f ){
		va_list v;
		va_start( v, fmt );
		writefv( f, fmt, v );
		va_end( v );
		write_pixels( f, rgb_dir, vdir, x, y, comp, data, alpha, pad );
		fclose( f );
   }
	return f!=NULL;
}

bool WriteBMP( char *filename, int x, int y, int comp, void *data ){
	int pad = (-x*3) & 3;
	return outfile(filename,-1,-1,x,y,comp,data,0,pad,
		"11 4 22 4" "4 44 22 444444",
		'B', 'M', 14+40+(x*3+pad)*y, 0,0, 14+40,  // file header
		40, x,y, 1,24, 0,0,0,0,0,0);             // bitmap header
}






const double PI = 3.1415926535897932384626433832795;

const int Width = 240, Height = 180, N = Width*Height;

double x[N], z[N], r[N], u[N], w[N];
double X[N], Z[N], R[N], U[N], W[N];
int Iter = 0, Left[N], Right[N], Up[N], Down[N];

double T = 0.005, Dxy = 0.5*0.5, low = 0, up = 0.2;  //  ²ÎÊý
double Dx = 0.1/Dxy, Dz = 0.1/Dxy, Dr = 0.1/Dxy, Du = 5/Dxy, Dw = 100/Dxy;
double e1 = 0.14, f1 = 1.6, e2 = 0.4, f2 = 1.1, q1 = 0.01, q2 = 0.01;
double det1 = 2*e1, det2 = 2*e2;

double inve1 = 1/e1, inve2 = 1/e2, invdet1 = 1/det1, invdet2 = 1/det2;


char filename[20] = "00.bmp";

unsigned char data[3*N];


double Rand(){ return (rand()%12345/12345.0)*0.1+0.05; }

void Init(){
	memset( data, 0, sizeof(data) );

	for(int i=0; i<N; i++){
		int j = i%Width, k = i/Width;
		Left[i] = ( j==0 ? i:i-1 );
		Right[i] = ( j==Width-1 ? i:i+1 );
		Up[i] = ( k==Height-1 ? i:i+Width );
		Down[i] = ( k==0 ? i:i-Width );
	}

	srand((unsigned)time(NULL));
	for(int i=0; i<N; i++){
		x[i] = Rand();
		z[i] = Rand();
		r[i] = Rand();
		u[i] = Rand();
		w[i] = Rand();
	}
}

void ScreenShot( char *filename, int width, int height ){
	for(int i=0,k=0; i<N; i++,k+=3){
		data[k+1] = (unsigned char)( (x[i]-low)/(up-low)*255 );
	}
	WriteBMP( filename, width, height, 3, data );
}


double F( double x, double z, double inve, double f, double q ){
	return inve*( x-x*x-f*z*(x-q)/(x+q) );
}
double constrain( double v, double low, double up ){
	return min( max( v, low ), up );
}
void next(){
	if( Iter--==0 ){
		Iter = 50;
		ScreenShot( filename, Width, Height );
		if( filename[1]=='9' ){ filename[0]+=1; filename[1]='0'; }
		else filename[1] += 1;
		if( filename[0]=='9' && filename[1]=='9' ) exit( 0 );
	}

	for(int iter=0; iter<100; iter++){
		double DT = 0, dx, dz, dr, du, dw;
		for(int i=0; i<N; i++){
			int L = Left[i], RR = Right[i], UU = Up[i], D = Down[i];

			dx = x[L]+x[RR]+x[UU]+x[D]-4*x[i];
			dz = z[L]+z[RR]+z[UU]+z[D]-4*z[i];
			dr = r[L]+r[RR]+r[UU]+r[D]-4*r[i];
			du = u[L]+u[RR]+u[UU]+u[D]-4*u[i];
			dw = w[L]+w[RR]+w[UU]+w[D]-4*w[i];

			X[i] = Dx*dx+F(x[i],z[i],inve1,f1,q1)-invdet1*(x[i]-r[i]);
			Z[i] = Dz*dz+x[i]-z[i];
			R[i] = Dr*dr+invdet1*(x[i]-r[i])+invdet2*(u[i]-r[i]);
			U[i] = Du*du+F(u[i],w[i],inve2,f2,q2)-invdet2*(u[i]-r[i]);
			W[i] = Dw*dw+u[i]-w[i];

			DT = max( DT, fabs(X[i]) );
			DT = max( DT, fabs(Z[i]) );
			DT = max( DT, fabs(R[i]) );
			DT = max( DT, fabs(U[i]) );
			DT = max( DT, fabs(W[i]) );
		}
		//T += 0.00001;
		DT = T/DT;

		low = 1e20; up = 0;
		for(int i=0; i<N; i++){
			x[i] = constrain( x[i]+X[i]*DT, 0, 0.2 );
			z[i] = constrain( z[i]+Z[i]*DT, 0, 0.2 );
			r[i] = constrain( r[i]+R[i]*DT, 0, 0.2 );
			u[i] = constrain( u[i]+U[i]*DT, 0, 0.2 );
			w[i] = constrain( w[i]+W[i]*DT, 0, 0.2 );
			low = min( low, x[i] );
			up  = max( up, x[i] );
		}
	}
}



int main(){
	Init();
	while( true ){
		next();
	}
	return 0;
}