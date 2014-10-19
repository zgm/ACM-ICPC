#include<iostream>
#include<cstdio>
#include<cstring>
#include<algorithm>
#include<stdlib.h>
#include<cmath>
#include<vector>
#include<stack>
#include<time.h>
#include<unordered_map>
#include<unordered_set>
#include<map>
#include<set>
#include<queue>
using namespace std;

#include <iostream>
#include <functional>
#include <algorithm>
#include <vector>
#include <iterator>
using namespace std;
int main()
{
     vector<int> vi;
     vi.push_back(1); // insert elements into vector
     vi.push_back(0);
     vi.push_back(0);
     vi.push_back(9);
     vi.push_back(2);  //.............
     vi.push_back(0);
     vector<int>::iterator new_end = unique(vi.begin(), vi.end());
     //vi.erase(new_end, vi.end());

     vector<int>::iterator it;

     copy(vi.begin(),vi.end(),std::ostream_iterator<int>( std::cout," "));
     cout << endl;

     sort(vi.begin(),vi.end());
     vector<int>::iterator new_end2 = unique(vi.begin(), vi.end());

     copy(vi.begin(),new_end2,std::ostream_iterator<int>( std::cout," ")); cout<<endl;
     copy(vi.begin(),vi.end(),std::ostream_iterator<int>( std::cout," "));

     cout << endl;
	 return 0;
}



/*
struct Node{
	char c;
	unordered_map<char,Node*> mp;
	Node *p, *suf;
	bool flag;
	Node(char C):c(C),suf(NULL),flag(false){ mp.clear(); }
};

void Insert( Node *root, char *s, int n ){
	Node *next = NULL;
	for(int i=0; i<n; i++){
		if( root->mp.find(s[i])==root->mp.end() ){
			next = new Node(s[i]);
			root->mp[s[i]] = next;
			next->p = root;
		}
		root = root->mp[s[i]];
	}
	root->flag = true;
}
Node *Suffix( Node *suf, char c, Node *root ){
	for( ; suf && suf->mp.find(c)==suf->mp.end(); suf=suf->suf);
	if( suf==NULL || suf->mp.find(c)==suf->mp.end() ) return root;
	return suf->mp.find(c)->second;
}
void TrieGraph(Node *root){
	queue<Node*> q;
	q.push(root);
	Node *cur = NULL, *next = NULL;
	unordered_map<char,Node*>::iterator it;
	while( !q.empty() ){
		cur = q.front(); q.pop();
		for(it=cur->mp.begin(); it!=cur->mp.end(); it++){
			it->second->suf = Suffix(cur->suf, it->first, root);
			it->second->flag |= it->second->suf->flag;
			if( !it->second->flag ) q.push(it->second);
		}
	}
}
void show(Node *root){
	printf("%c   %c\n",root->c,root->suf?root->suf->c:'#');
	unordered_map<char,Node*>::iterator it;
	for(it=root->mp.begin(); it!=root->mp.end(); it++){
		show(it->second);
	}
}


int main(){
	Node *root = new Node('$');
	int n; char s[10];
	scanf("%d",&n);
	while( n-- ){
		scanf("%s",s);
		Insert(root,s,strlen(s));
	}
	TrieGraph(root);
	show(root);
	scanf("%s",s);

	
	Node *cur = root;
	for(int i=0; i<strlen(s); i++){
		while( cur && cur->mp.find(s[i])==cur->mp.end() ) cur = cur->suf;
		if( cur==NULL ) cur = root;
		else{
			cur = cur->mp.find(s[i])->second;
			if( cur->flag ){
				Node *p = cur;
				while( p ){
					printf("%c ->",p->c);
					p = p->p;
				}
				printf("\n");
			}
		}
	}
	return 0;
}
*/




/*
struct Node{
	int t, v; bool flag;
	Node(int T,int V,bool Flag):t(T),v(V),flag(Flag){}
};
bool cmp(Node s, Node t){ return s.t<t.t; }

int main(){
	int n, s, t, v;
	scanf("%d",&n);
	vector<Node> a;
	//vector<int> vol;
	map<int,int> st;
	map<int,int>::iterator it;
	while( n-- ){
		scanf("%d%d%d",&s,&t,&v);
		a.push_back(Node(s,v,true));
		a.push_back(Node(t,v,false));
	}
	sort(a.begin(),a.end(),cmp);
	int pre_t = a[0].t, pre_mx = -1;
	for(int i=0; i<a.size(); i++){
		if( st.begin()==st.end() ){ 
			pre_t = a[i].t; 
			pre_mx = a[i].v; 
			st.insert(make_pair(a[i].v,1)); 
			continue;
		}
		it = st.end(); --it;
		if( a[i].t!=a[i-1].t && it->first!=pre_mx ){
			printf("%d %d %d\n",pre_t,a[i-1].t,pre_mx);
			pre_t = a[i-1].t;
			pre_mx = it->first;
		}
		it = st.find(a[i].v);
		if( a[i].flag ){
			if( it!=st.end() ) it->second++;
			else st.insert(make_pair(a[i].v,1));
		}else{
			if( it->second>1 ) it->second--;
			else st.erase(it);// st.erase(st.find(a[i].v));
		}
		//for(it=st.begin(); it!=st.end(); it++) printf("%d ",*it); printf("   end\n");
	}
	printf("%d %d %d\n",pre_t,a[a.size()-1].t,pre_mx);
	return 0;
}
*/



/*
struct ListNode{
	int val;
	ListNode *pre, *next;
	ListNode(int x=0):val(x),pre(NULL),next(NULL){}
};
void show(ListNode *head){
	printf("List = ");
	while( head ){
		printf("%d ",head->val);
		head = head->next;
	}
	printf("\n");
}

class LRU{
	int size, MaxSize;
	ListNode *head, *tail;
	unordered_map<int,ListNode* > mp;
public:
	LRU(){ size = 0; MaxSize = 4; head = tail = NULL; }
	~LRU(){ }
	ListNode* use(int x){
		if( mp.find(x)==mp.end() ){
			ListNode *cur = new ListNode(x);
			if( head==NULL ) head = tail = cur;
			else{ cur->next = head; head->pre = cur; head = cur; }
			size++;
			mp[x] = cur;

			if( size>MaxSize ){
				cout<<"delete "<<tail->val<<endl;
				mp.erase(mp.find(tail->val));
				if( tail->pre ){
					tail->pre->next = NULL;
					ListNode *pre = tail->pre;
					delete tail;
					tail = pre;
				}else{
				}
			}
		}
		else refresh(x);
		return head;
	}
	void refresh(int x){
		ListNode *cur = mp[x];
		if( head==cur ) return;
		if( tail==cur ) tail = tail->pre;
		ListNode *pre = cur->pre;
		cur->pre->next = cur->next;
		if( cur->next ) cur->next->pre = cur->pre;
		cur->next = head;
		head->pre = cur;
		head = cur;
	}
	void show(){
		printf("List = ");
		ListNode *cur = head;
		while( cur ){
			printf("%d ",cur->val);
			cur = cur->next;
		}
		printf("\n");
	}
};
	

int main(){
	LRU test;
	int x;
	while( cin>>x ){
		test.use(x);
		test.show();
	}
	return 0;
}*/







/*
struct ListNode{
	int val;
	ListNode *next;
	ListNode(int x):val(x), next(NULL){}
};

void show(ListNode *head){
	while( head ){
		cout<<" "<<head->val;
		head = head->next;
	}
	cout<<"  ok"<<endl;
}

class Solution {
public:
    static ListNode *reverse( ListNode *head ){
        ListNode *pre = NULL, *cur = head, *next = NULL;
        while( cur ){
            next = cur->next;
            cur->next = pre;
            pre = cur; cur = next;
        }
        return pre;
    }
    static void reorderList(ListNode *head) {
        // IMPORTANT: Please reset any member data you declared, as
        // the same Solution instance will be reused for each test case.
        if( head==NULL || head->next==NULL ) return;
        ListNode *first = head, *second = head, *next = NULL;
        while( second->next && second->next->next ){
            first = first->next;
            second = second->next->next;
        }
        second = first->next;
        first->next = NULL;
        second = reverse(second);
		show(second); show(head);
        
        first = NULL;
        while( head || second ){
            if( head ){
				if( first ) first->next = head;
				first = head;
                head = head->next;
            }
            if( second ){
                first->next = second;
                first = second;
                second = second->next;
            }
        }
        first->next = NULL;
    }
};
int main(){
	int n, val;
	while(cin>>n){
		ListNode *head = NULL, *cur = NULL;
		for(int i=0; i<n; i++){
			cin>>val;
			ListNode *tmp = new ListNode(val);
			if( head==NULL ){
				head = tmp;
				cur = tmp;
			}
			else{
				cur->next = tmp;
				cur = tmp;
			}
		}
		show(head);
		Solution::reorderList(head);
		show(head);
	}
	return 0;
}*/


/*
int main(){
	int n, k;
	while( cin>>n>>k ){
        // IMPORTANT: Please reset any member data you declared, as
        // the same Solution instance will be reused for each test case.
        vector<int> x; x.push_back(1);
        for(int i=1,s=1; i<=n; i++){ s*=i; x.push_back(s); }
        unordered_set<int> st;
        string result = "";
        for(int i=n-1; i>=0; i--){
            int t = k/x[i];
			cout<<t<<" "<<k<<" "<<x[i]<<endl;
			k -= (t-1)*x[i];
            for(int j=1; j<=n; j++){
                if( st.find(j)==st.end() ){
                    t--;
                    if( t==0 ){ cout<<"=== "<<j<<endl; st.insert(j); result += ('0'+j); break; }
                }
            }
        }
        cout<<result<<endl;
    }
	return 0;
}*/



/*
vector<int> v;
unordered_map<int,int> mp;
unordered_map<int,int>::iterator it;

int main(){
	int a[100], n = 0, t, x;
	srand((unsigned)time(NULL));
	while( cin>>t>>x ){
		if( t==-2 ){
			if( v.size()>0 ){
				int id = rand()%v.size();
				mp.erase(v[id]);
				if( id!=v.size()-1 ){
					mp[v[v.size()-1]] = id;
					swap(v[id],v[v.size()-1]);
				}
				v.pop_back();
			}
		}else if( t==-1 ){
			if( (it=mp.find(x))!=mp.end() ){
				int id = it->second;
				mp.erase(x);
				if( id!=v.size()-1 ){
					mp[v[v.size()-1]] = id;
					swap(v[id],v[v.size()-1]);
				}
				v.pop_back();
			}else printf("no find this number %d\n",x);
		}else{
			v.push_back(x);
			mp[x] = v.size()-1;
		}
		for(int i=0; i<v.size(); i++) printf("%d ",v[i]); printf("\n");
		//a[n++] = t;
		//make_heap(a,a+n);
		//push_heap(a,a+n,greater<int>() );
		//for(int i=0; i<n; i++) cout<<a[i]<<" "; cout<<endl;
		//cout<<a[0]<<endl;
	}
	return 0;
}
*/

/*class Solution {
public:
    static string minWindow(string S, string T) {
        // Note: The Solution object is instantiated only once and is reused by each test case.
        int n = S.length(), m = T.length(), st = -1, end = -1;
        unordered_map<char,int> mp;
        unordered_map<char,int>::iterator it;
        for(int i=m-1; i>=0; i--){
	    it = mp.find(T[i]);
	    if( it==mp.end() ) mp[T[i]] = 1;
	    else{ it->second++; m--; }
            //mp.insert( make_pair(T[i],1) );
        }
        for(int i=0,ii=0; i<n; i++){
            it = mp.find(S[i]);
            if( it==mp.end() ) continue;
            it->second--;
            if( it->second==0 ) m--;
            if( m>0 ) continue;
            
            for(; m==0 && ii<=i; ii++){
		cout<<ii<<" "<<i<<" "<<m<<endl;
		if( st==-1 || end-st>i-ii ){ st=ii; end=i; }
                it = mp.find(S[ii]);
		if( it==mp.end() ) continue;
                it->second++;
                if( it->second>0 ) m++;
            }
        }
	if(st==-1) return "";
	cout<<st<<" "<<end<<endl;
        return S.substr(st,end-st+1);
    }
};

int main(){
    string s, t;
    while(cin>>s>>t){
	cout<<Solution::minWindow(s,t)<<endl;
    }
    return 0;
}*/

/*
int main(){
    int n, a;
    int v[100], vn = 0;
    scanf("%d",&n);
    while(n--){ scanf("%d",&a); v[vn++] = a; }
    make_heap(v,v+vn,greater<int>());
    for(int i=0; i<vn; i++) printf("%d ",v[i]); printf("\n");
    
    scanf("%d",&n);
    while( n-- ){
        //v.push_back(n);
        cout<<v[0]<<" ";
        pop_heap(v,v+vn,greater<int>());
        vn--;
        //push_heap(v.begin(),v.end(),greater<int>());
        //for(int i=0; i<v.size(); i++) printf("%d ",v[i]); printf("\n");
    }
    return 0;
}
*/
