#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<stack>
using namespace std;


struct Node{
    int val;
    Node *left, *right;
    Node(int v){ val=v; left=NULL; right=NULL; }
};


class Tree{
    Node *root;
public:
    Tree(){ root = NULL; }
    //~Tree(){ delete root; }
    
    static void insert(Node *&root, int val){
        if(root==NULL){ root = new Node(val); return; }
        if(val<root->val) insert(root->left,val);
        else insert(root->right,val);
    }
    static void print(Node *root, int deep=0){
        if(root==NULL) return;
        print(root->left,deep+1);
        for(int i=0; i<deep; i++) printf("  ");
        printf("%d\n",root->val);
        print(root->right,deep+1);
    }
    void insert(int val){ insert(root,val); }
    void print(int deep=0){ print(root,deep); }
    
    
    class PreIterator{
        stack<Node*> s;
    public:
        PreIterator(Node *root=NULL){ if(root!=NULL) s.push(root); }
        PreIterator(Tree t){ if(t.root!=NULL) s.push(t.root); }
        
        Node *hasNext(){
            if( s.empty() ) return NULL;
            Node *cur = s.top(); s.pop();
            if(cur->right) s.push(cur->right);
            if(cur->left) s.push(cur->left);
            return cur;
        }
    };
    
    class InorderIterator{
        stack<Node*> s;
        void pushNext(Node *root){
            while(root!=NULL){ s.push(root); root=root->left; }
        }
    public:
        InorderIterator(Node *root=NULL){ pushNext(root); }
        InorderIterator(Tree t){ pushNext(t.root); }
        
        Node *hasNext(){
            if( s.empty() ) return NULL;
            Node *cur = s.top(); s.pop();
            pushNext(cur->right);
            return cur;
        }
    };
    
    class PostIterator{
        stack<Node*> s;
        void pushNext(Node *root){
            while(root!=NULL){
                s.push(root);
                if(root->left) root = root->left;
                else root = root->right;
            }
        }
    public:
        PostIterator(Node *root=NULL){ pushNext(root); }
        PostIterator(Tree t){ pushNext(t.root); }
        
        Node *hasNext(){
            if( s.empty() ) return NULL;
            Node *cur = s.top(); s.pop();
            if(!s.empty()){
                if(s.top()->left==cur) pushNext(s.top()->right);
            }
            return cur;
        }
    };
    
};


int main(){
    
    Tree t = Tree();
    int a[10] = {3,6,2,5,9,1,0,4,7,8};
    for(int i=0; i<10; i++) t.insert(a[i]);
    t.print();
    
    
    Tree::PreIterator it = Tree::PreIterator(t);
    Node *cur = NULL;
    while( (cur=it.hasNext())!=NULL ){
        printf("%d ",cur->val);
    }
    printf("\n");
    
    
    Tree::InorderIterator iit = Tree::InorderIterator(t);
    cur = NULL;
    while( (cur=iit.hasNext())!=NULL ){
        printf("%d ",cur->val);
    }
    printf("\n");
    
    Tree::PostIterator pit = Tree::PostIterator(t);
    cur = NULL;
    while( (cur=pit.hasNext())!=NULL ){
        printf("%d ",cur->val);
    }
    printf("\n");

    return 0;
}

