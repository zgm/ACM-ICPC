#include<iostream>
#include<stack>
using namespace std;

class Expression{
	struct Node{
		char c; double v; int l, r;
		Node(){ c = ' '; l = r = -1; }
		void Set( char C, double V, int L, int R){ c = C; v = V; l = L; r = R; }
	};
public:
	static void Init( int ps, int &pn, Node *p, int l, int r, char *s, int *pair ){
		if( l > r ){ p[ps].c = '0'; p[ps].v = 0; return; }
		if( s[l]=='(' && s[r]==')' && pair[l]==pair[r] ){
			Init( ps, pn, p, l+1, r-1, s, pair );
			return;
		}
		int w = -1;
		for(int i=r; i>=l; i--){
			if( s[i]==')' ){
				for(int k=i--; s[i]!='(' || pair[i]!=pair[k]; i--);
			}
			if( (s[i]=='+'|| s[i]=='-') && (i>0 && s[i-1]!='(' ) ){ w = i; break; }
			if( (s[i]=='*'|| s[i]=='/') && w == -1 ) w = i;
		}
		if( w != -1 ){
			p[ps].Set( s[w], 0, pn, -1 );
			Init( p[ps].l, ++pn, p, l, w-1, s, pair );
			p[ps].r = pn;
			Init( p[ps].r, ++pn, p, w+1, r, s, pair );
		}
		else{
			p[ps].Set( s[l], 0, -1, -1 ); // 单字符表达式, 下面是数值
			if( s[l]!='-' ) p[ps].v = s[l]-'0';
			for(int i=l+1; i<=r; i++) p[ps].v = 10*p[ps].v+s[i]-'0';
			if( s[l]=='-' ) p[ps].v = -p[ps].v;
		}
	}
	static double Value( int ps, Node *p ){
		double l = 0, r = 0;
		if( p[ps].l == -1 && p[ps].r == -1 ) return p[ps].v;
		if( p[ps].l != -1 ) l = Value( p[ps].l, p );
		if( p[ps].r != -1 ) r = Value( p[ps].r, p );
		if( p[ps].c == '+' ) p[ps].v = l+r;
		if( p[ps].c == '-' ) p[ps].v = l-r;
		if( p[ps].c == '*' ) p[ps].v = l*r;
		if( p[ps].c == '/' ) p[ps].v = l/r;
		return p[ps].v;
	}
	static void MinBracket( int ps, Node *p, int k=0 ){
		if( ps == -1 ) return;
		char c = p[ps].c, lc = p[p[ps].l].c, rc = p[p[ps].r].c;
		int kk = k;
		if( c == '*' || c == '/' ){
			if( lc=='+' || lc=='-' ){ printf("("); kk = 0; }
		}
		MinBracket( p[ps].l, p, kk );
		if( c == '*' || c == '/' ){
			if( lc=='+' || lc=='-' ) printf(")");
		}
		if( c=='+' || c=='-' ) if( k==1 ) c = (c=='+' ? '-':'+' );
		if( c=='*' || c=='/' ) if( k==2 ) c = (c=='*' ? '/':'*' );
		printf("%c",c);
		if( c == '*' || c == '/' ){
			if( rc=='+' || rc=='-' ) printf("(");
		}
		k = 0;
		if( c=='-' && (rc =='+'||rc=='-') ) k = 1;
		if( c=='/' && (rc =='*'||rc=='/') ) k = 2;
		MinBracket( p[ps].r, p, k );
		if( c == '*' || c == '/' ){
			if( rc=='+' || rc=='-' ) printf(")");
		}
	}
	static void Show( int ps, Node *p, int deep ){
		if( ps == -1 ) return;
		Show( p[ps].r, p, deep+1 );
		for(int i=0; i<deep; i++) printf(" ");
		if( p[ps].l == -1 && p[ps].r == -1 ) printf("%lf\n", p[ps].v );
		else printf("%c\n", p[ps].c );
		Show( p[ps].l, p, deep+1 );
	}
	static void GetPair( int n, int *pair, char *s ){
		stack<int> st;
		memset( pair, 0, sizeof(int)*n );
		for(int i=0, k=0; i<n; i++){
			if( s[i]=='(' ){ pair[i] = ++k; st.push( k ); }
			else if( s[i]==')' ){ pair[i] = st.top(); st.pop(); }
		}
	}
	/*static void ans(){ // 表达式求值
		const int N = 1008;
		char s[N];
		int pair[N];
		Node p[10*N];
		while( scanf("%s",s)==1 ){
			int n = strlen(s), pn = 0;
			GetPair( n, pair, s );
			Init( 0, ++pn, p, 0, n-1, s, pair );
			Show( 0, p, 1 );
			printf("%.6lf\n", Value( 0, p ) );
		}
	}*/
	static void ans(){ // poj 2792 Brackets Removal
		const int N = 1008;
		char s[N];
		int pair[N]; // 记录括号匹配
		Node p[10*N];
		while( scanf("%s",s)==1 ){
			int n = strlen(s), pn = 0;
			GetPair( n, pair, s );
			Init( 0, ++pn, p, 0, n-1, s, pair );
			//Show( 0, p, 1 );
			MinBracket( 0, p );
			printf("\n");
		}
	}
};

int main(){
	Expression::ans();
	return 0;
}