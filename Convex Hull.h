#include<iostream>
#include<cstdio>
#include<vector>
#include<map>
#include<cmath>
#include<algorithm>
using namespace std;

const int N = 512;
const double eps = 1e-6;

struct Point{
    double x, y, z;
    Point(double X=0, double Y=0, double Z=0): x(X),y(Y),z(Z){}
    Point operator - (Point p){ return Point(x-p.x,y-p.y,z-p.z); }
    Point operator * (Point p){ return Point(y*p.z-z*p.y,z*p.x-x*p.z,x*p.y-y*p.x); }
    double operator ^ (Point p){ return x*p.x+y*p.y+z*p.z; }
}P[N];

double vlen( Point p ){ return sqrt(p.x*p.x+p.y*p.y+p.z*p.z); }
double area( Point a, Point b, Point c ){ return vlen((b-a)*(c-a))/2; }

struct Plane{
    vector<int> id;  // 记录凸包面上点的编号
    bool ok; // 表示该面是否属于最终凸包中的面
}add, F[4*N];

int n, cnt; map<pair<int,int>,int> mp;


double side(Point p, Plane f){ // 正：点在面同向   负：点在面反向
    Point m = P[f.id[1]]-P[f.id[0]], n = P[f.id[2]]-P[f.id[0]];
    return (m*n)^(p-P[f.id[0]]);
}

void dfs(int,Plane&);
void deal(int p, int a, int b){
    int f = mp[make_pair(a,b)];
    if( F[f].ok ){
        if( side(P[p],F[f]) > eps ) dfs( p, F[f] );
        else{
            add.id[0] = b; add.id[1] = a; add.id[2] = p; add.ok = true;
            mp[make_pair(p,b)] = mp[make_pair(a,p)] = mp[make_pair(b,a)] = cnt;
            F[cnt++] = add;
        }
    }
}

void dfs(int p, Plane &f){
    f.ok = false;
    deal( p, f.id[1], f.id[0] );
    deal( p, f.id[0], f.id[2] );
    deal( p, f.id[2], f.id[1] );
}

double Solve(){
    if( n<4 ) return 0;
    cnt = 0;
    add.id.clear();
    add.id.push_back( 0 ); add.id.push_back( 0 ); add.id.push_back( 0 );
    for(int i=0; i<4; i++){
        add.id[0] = (i+1)%4;
        add.id[1] = (i+2)%4;
        add.id[2] = (i+3)%4;
        add.ok = true;
        if( side(P[i],add) > 0 ) swap( add.id[1], add.id[2] );
        mp[make_pair(add.id[0],add.id[1])] = cnt;
        mp[make_pair(add.id[1],add.id[2])] = cnt;
        mp[make_pair(add.id[2],add.id[0])] = cnt;
        F[cnt++] = add;
    }
    for(int i=4; i<n; i++){
        for(int j=0; j<cnt; j++){
            if( F[j].ok && side(P[i],F[j]) > eps ){
                dfs( i, F[j] ); break;
            }
        }
    }

    double result = 0;
    for(int i=0; i<cnt; i++){
        if( F[i].ok ) result += area( P[F[i].id[0]], P[F[i].id[1]], P[F[i].id[2]] );
    }
    return result;
}

/*int main(){
    while( scanf("%d",&n)==1 ){
        for(int i=0; i<n; i++) scanf("%lf%lf%lf",&P[i].x,&P[i].y,&P[i].z);
        random_shuffle( &P[0], &P[n] );
        printf("%.3f\n",Solve());
    }
    return 0;
}*/
