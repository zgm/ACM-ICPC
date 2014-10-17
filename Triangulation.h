#include<algorithm>
#include<stdlib.h>
#include<stdio.h>
#include<cmath>
#include<cstring>
#include<string.h>
#include<time.h>
#include<vector>
#include<queue>
#include<list>
#include<stack>
#include<set>
#include<map>
#include<iostream>
using namespace std;

 
#define Oi(e) ((e)->oi)  
#define Dt(e) ((e)->dt)
#define On(e) ((e)->on)
#define Op(e) ((e)->op)
#define Dn(e) ((e)->dn)
#define Dp(e) ((e)->dp)
#define Other(e, p) ((e)->oi == p ? (e)->dt : (e)->oi)
#define Next(e, p) ((e)->oi == p ? (e)->on : (e)->dn)
#define Prev(e, p) ((e)->oi == p ? (e)->op : (e)->dp)
#define V(p1, p2, u, v) (u = p2->x - p1->x, v = p2->y - p1->y)
#define C2(u1, v1, u2, v2) (u1 * v2 - v1 * u2)
#define C3(p1, p2, p3) ((p2->x - p1->x) * (p3->y - p1->y) - (p2->y - p1->y) * (p3->x - p1->x))
#define Dot(u1, v1, u2, v2) (u1 * u2 + v1 * v2)
 
const int MAXN = 1000001;
 
struct point {
    double x, y;
    struct edge *in;
    bool operator<(const point &p)const{ return x<p.x || (x==p.x && y<p.y); }
};
struct edge {
    point *oi, *dt;
    edge *on, *op, *dn, *dp;
};
int nfree; edge mem[3 * MAXN];

struct gEdge {
    int u, v; double w;
    bool operator<(const gEdge &e)const{ return w<e.w; }
}E[3*MAXN], MST[MAXN];


int UFfind( int u, int *p ){
	if( p[u]<0 ) return u;
	else{ p[u] = UFfind( p[u], p ); return p[u]; }
}
void UFunion( int u, int v, int *p ){
	int pu = UFfind( u, p ), pv = UFfind( v, p );
	if( pu != pv ){
		int tmp = p[pu]+p[pv];
		if( p[pu] > p[pv] ){ p[pu] = pv; p[pv] = tmp; }
		else{ p[pv] = pu; p[pu] = tmp; }
	}
}
void Kruskal( int N, int M, gEdge *E, int k, int *p ){
    if( k>N ) k = N;
    sort(E, E + M );
    memset( p, -1, sizeof(int)*N );
    for(int i=0, kk=0; i<M && kk<N-1; i++)
        if( UFfind(E[i].u, p)!=UFfind(E[i].v, p) ){
            UFunion( E[i].u, E[i].v, p);
            MST[kk++] = E[i];
        }
    printf("%d\n", int(ceil(MST[N-k-1].w) + 0.00000001));
}

 
void Splice( edge *a, edge *b, point *v ) {
    edge *next;
    if( Oi(a)==v ) next = On(a), On(a) = b;
    else next = Dn(a), Dn(a) = b;
    if( Oi(next)==v ) Op(next) = b;
    else Dp(next) = b;
    if( Oi(b)==v ) On(b) = next, Op(b) = a;
    else Dn(b) = next, Dp(b) = a;
}
edge *Make_edge( point *u, point *v ) {
    edge *e = &mem[--nfree];
    e->on = e->op = e->dn = e->dp = e; e->oi = u; e->dt = v;
    if( u->in==NULL ) u->in = e;
	if( v->in==NULL ) v->in = e;
    return e;
}
edge *Join( edge *a, point *u, edge *b, point *v, int side ) {
    edge *e = Make_edge(u, v);
    if(side == 1) {
        if( Oi(a)==u ) Splice( Op(a), e, u );
        else Splice( Dp(a), e, u );
        Splice( b, e, v );
    }
    else {
        Splice( a, e, u );
        if( Oi(b)==v ) Splice( Op(b), e, v );
        else Splice( Dp(b), e, v );
    }
    return e;
}
void Remove( edge *e ) {
    point *u = Oi(e), *v = Dt(e);
    if( u->in==e ) u->in = e->on;
	if( v->in==e ) v->in = e->dn;
    if( Oi(e->on) == u ) e->on->op = e->op;
    else e->on->dp = e->op;
    if( Oi(e->op) == u ) e->op->on = e->on;
    else e->op->dn = e->on;
    if( Oi(e->dn) == v ) e->dn->op = e->dp;
    else e->dn->dp = e->dp;
    if( Oi(e->dp) == v ) e->dp->on = e->dn;
    else e->dp->dn = e->dn;
}
void Make_Graph( int N, int &M, gEdge *E, point *p ){ // print edge
    for(int i = 0; i < N; ++i) {
        point *u = &p[i];
        edge *start = u->in, *e = u->in;
        do{
            point *v = Other(e, u);
            if(u < v) {
                E[M].u = u - p, E[M].v = v - p;
                E[M++].w = hypot(u->x - v->x, u->y - v->y);
            }
            e = Next(e, u);
        }while( e != start );
    }
}
void Print_Triangle( int N, point *p ){ // print triangle
	for(int i=0; i<N; i++){
		point *u = &p[i];
		edge *start = u->in, *e = u->in;
		do{
			point *v = Other( e, u );
			if( u<v ){
				edge *next = Next( e, u );
				point *w = Other( next, u );
				if( u<w && Next(next,w)==Prev(e,v) ){
					if( v>w ) swap( v, w );
					printf("%d %d %d\n",u-p,v-p,w-p);
				}
			}
			e = Next( e, u );
		}while( e!=start );
	}
}
void Low_tangent(edge *e_l, point *o_l, edge *e_r, point *o_r, edge **l_low, point **OL, edge **r_low, point **OR) {
    point *d_l = Other(e_l, o_l), *d_r = Other(e_r, o_r);
    while(true) {
        if( C3(o_l, o_r, d_l) < 0.0 ) {
            e_l = Prev(e_l, d_l);
            o_l = d_l; d_l = Other(e_l, o_l);
        }
        else if( C3(o_l, o_r, d_r) < 0.0 ) {
            e_r = Next(e_r, d_r);
            o_r = d_r; d_r = Other(e_r, o_r);
        }
        else break;
    }
    *OL = o_l, *OR = o_r;
    *l_low = e_l, *r_low = e_r;
}
void Merge( edge *lr, point *s, edge *rl, point *u, edge **tangent ) {
    double l1, l2, l3, l4, r1, r2, r3, r4, cot_L, cot_R;
	double u1, v1, u2, v2, N1, cot_N, P1, cot_P;
    point *O, *D, *OR, *OL;
    edge *B, *L, *R;
    Low_tangent(lr, s, rl, u, &L, &OL, &R, &OR);
    *tangent = B = Join(L, OL, R, OR, 0);
    O = OL, D = OR;
    do{
        edge *El = Next(B, O), *Er = Prev(B, D), *next, *prev;
        point *l = Other(El, O), *r = Other(Er, D);
        V(l, O, l1, l2); V(l, D, l3, l4); V(r, O, r1, r2); V(r, D, r3, r4);
        double cl = C2(l1, l2, l3, l4), cr = C2(r1, r2, r3, r4);
        bool BL = cl > 0.0, BR = cr > 0.0;
        if(!BL && !BR) break;
        if(BL) {
            double dl = Dot(l1, l2, l3, l4);
            cot_L = dl / cl;
            do {
                next = Next(El, O);
                V(Other(next, O), O, u1, v1); V(Other(next, O), D, u2, v2);
                N1 = C2(u1, v1, u2, v2);
                if(!(N1 > 0.0)) break;
                cot_N = Dot(u1, v1, u2, v2) / N1;
                if(cot_N > cot_L) break;
                Remove(El);
                El = next;
                cot_L = cot_N;
            } while(true);
        }
        if(BR) {
            double dr = Dot(r1, r2, r3, r4);
            cot_R = dr / cr;
            do {
                prev = Prev(Er, D);
                V(Other(prev, D), O, u1, v1); V(Other(prev, D), D, u2, v2);
                P1 = C2(u1, v1, u2, v2);
                if(!(P1 > 0.0)) break;
                cot_P = Dot(u1, v1, u2, v2) / P1;
                if(cot_P > cot_R) break;
                Remove(Er);
                Er = prev;
                cot_R = cot_P;
            } while(true);
        }
        l = Other(El, O); r = Other(Er, D);
        if(!BL || (BL && BR && cot_R < cot_L)) { B = Join(B, O, Er, r, 0); D = r; }
        else { B = Join(El, l, B, D, 0); O = l; }
    } while(true);
}
void Divide( int s, int t, edge **L, edge **R, point *p ) {
    edge *a, *b, *c, *ll, *lr, *rl, *rr, *tangent;
    int n = t - s + 1;
    if(n == 2) *L = *R = Make_edge( &p[s], &p[t] );
    else if(n == 3) {
        a = Make_edge( &p[s], &p[s+1] ), b = Make_edge( &p[s+1], &p[t] );
        Splice( a, b, &p[s+1] );
        double v = C3( (&p[s]), (&p[s+1]), (&p[t]) );
        if( v > 0.0 ){
            c = Join( a, &p[s], b, &p[t], 0 );
            *L = a; *R = b;
        }
        else if( v < 0.0 ){
            c = Join( a, &p[s], b, &p[t], 1 );
            *L = c; *R = c;
        }
        else { *L = a; *R = b; }
    }
    else if(n > 3) {
        int split = (s+t)/2;
        Divide( s, split, &ll, &lr, p );
		Divide( split+1, t, &rl, &rr, p );
        Merge( lr, &p[split], rl, &p[split+1], &tangent );
        if( Oi(tangent) == &p[s] ) ll = tangent;
        if( Dt(tangent) == &p[t] ) rr = tangent;
        *L = ll; *R = rr;
    }
}
/*
	问题是求平面欧几里得最小生成树的第n-k小边。平面欧几里德最小生成树是经典问题，
	可以做到O(nlogn)。具体做法是先对平面点进行三角剖分，时间复杂度是O(nlogn)，三角
	剖分的边就是可能的在最小生成树的边。因为是平面图，所以有O(n)条边，在其上应用
	Kruscal 算法即可。
*/
point p[MAXN];

int main(){ // poj 3391
	int cs, k, N, M, f[MAXN];
	scanf("%d",&cs);
	while( cs-- ){
		scanf("%d", &k);
		nfree = 3*MAXN;
		N = 0;
		for(int t; true; N++ ){
			scanf("%d",&t);
			if( t==-1 ) break;
			p[N].x = t;
			scanf("%d",&t);
			p[N].y = t;
			p[N].in = NULL;
		}
		if( N==1) { printf("0\n"); continue; }
        sort( p, p + N );
        edge *L, *R;
        Divide( 0, N - 1, &L, &R, p );
        M = 0;
        Make_Graph( N, M, E, p );
        Kruskal( N, M, E, k, f );
    }
    return 0;
}
