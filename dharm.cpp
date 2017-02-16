#include<bits/stdc++.h>
#define gcd __gcd
#define pb(x) push_back(x)
#define ll long long
#define in(x) scanf("%d",&x)
#define mod 1000000007
#define sz(x) x.size()
#define mst(x,a) memset(x,a,sizeof(x))
#define pii pair<int,int>
#define F first
#define S second
#define m_p make_pair
#define all(v) (v.begin(),v.end())
#define red 0
#define black 1
using namespace std;
bool empti=0;
struct node
{
    pair<int,int> p;//contains the range
    int maxi;//maximum high of all subtree routed at node
    bool clr;//color of the node
    node *lft,*ryt,*parent;//the lft child, ryt child and the parent nodes
};
void correct(node *rt)
{
    if(rt==NULL)
        return;
    correct(rt->lft);
    correct(rt->ryt);
    if(rt->lft!=NULL&&rt->ryt!=NULL)
        rt->maxi=max(rt->maxi,max(rt->lft->maxi,rt->ryt->maxi));
    else if(rt->lft!=NULL)
        rt->maxi=max(rt->maxi,rt->lft->maxi);
    else if(rt->ryt!=NULL)
        rt->maxi=max(rt->maxi,rt->ryt->maxi);
    return;
}
node *insert_node(node *rt,node *curr)//inserting the node
{
    if(rt==NULL)
        return curr;

    rt->maxi=max(rt->maxi,curr->maxi);
    if(curr->p.F<rt->p.F)
    {
        rt->lft=insert_node(rt->lft,curr);
        rt->lft->parent=rt;
    }
    else
    {
        rt->ryt=insert_node(rt->ryt,curr);
        rt->ryt->parent=rt;
    }
    return rt;

}
void rlft(node *&curr,node *&rt)//lft rotation
{
    node *ryt=curr->ryt;
    curr->ryt=ryt->lft;

    if(curr->ryt!=NULL)
        curr->ryt->parent=curr;
    ryt->parent=curr->parent;
    if(curr->parent==NULL)
        rt=ryt;
    else if(curr==curr->parent->lft)
        curr->parent->lft=ryt;
    else
        curr->parent->ryt=ryt;
    ryt->lft=curr;
    curr->parent=ryt;
}
void rryt(node *&curr,node *&rt)//ryt rotation
{
    node *lft=curr->lft;
    curr->lft=lft->ryt;
    if(curr->lft!=NULL)
        curr->lft->parent=curr;
    lft->parent=curr->parent;
    if(curr->parent==NULL)
        rt=lft;
    else if(curr==curr->parent->lft)
        curr->parent->lft=lft;
    else
        curr->parent->ryt=lft;
    lft->ryt=curr;
    curr->parent=lft;
}
void refresh(node *&rt,node *&curr)
{
    node *parent=NULL;
    node *gparent=NULL;
    while(curr!=rt&&curr->clr==red&&curr->parent->clr==red)
    {
        parent=curr->parent;
        gparent=curr->parent->parent;
        if(gparent->lft==parent)
        {
            node *uncle=gparent->ryt;
            if(uncle!=NULL&&uncle->clr==red)
            {
                gparent->clr=red;
                parent->clr=black;
                uncle->clr=black;
                curr=gparent;
            }
            else
            {
                if(curr==parent->ryt)
                {
                    rlft(curr,rt);
                    curr=parent;
                    parent=curr->parent;
                }
                rryt(gparent,rt);
                swap(parent->clr,gparent->clr);
                curr=parent;
            }
        }
        else
        {
            node *uncle=gparent->lft;
            if(uncle!=NULL&&uncle->clr!=black)
            {
                gparent->clr=red;
                parent->clr=black;
                uncle->clr=black;
                curr=gparent;
            }
            else
            {
                if(curr==parent->lft)
                {
                    rryt(parent,rt);
                    curr=parent;
                    parent=curr->parent;
                }
                rlft(gparent,rt);
                swap(gparent->clr,parent->clr);
                curr=parent;
            }
        }
    }
    rt->clr=black;

}
void search_interval_tree(node *rt,pair<int,int> p)//searching a range
{
    if(rt==NULL)
        return;
    int l=max(rt->p.F,p.F),r=min(rt->p.S,p.S);
    if(l<=r)
    {
        cout<<"the overlapping range is ("<<rt->p.F<<" "<<rt->p.S<<")"<<endl;
        empti=1;
        return;
    }
    if(rt->lft!=NULL&&rt->lft->maxi>=p.F)
        search_interval_tree(rt->lft,p);
    else
        search_interval_tree(rt->ryt,p);
}
void print(node *rt)//printing the in-order traversal
{
    if(rt==NULL)
        return;
    print(rt->lft);
    cout<<rt->p.F<<" "<<rt->p.S<<" "<<rt->maxi<<" "<<rt->clr<<endl;
    print(rt->ryt);
}
node *next_node(node* cur)
{
    node* temp;
    if(cur->ryt!=NULL)
    {
        while(cur->lft)
            cur=cur->lft;
        return cur;
    }
    temp=cur->parent;
    while(temp!=NULL && cur==temp->ryt)
    {
        cur=temp;
        temp=temp->parent;
    }
    return temp;
}
node *exact_find(node *rt,pair<int,int> p)
{
    if(p.F==rt->p.F&&p.S==rt->p.S)
        return rt;
    if(rt->lft!=NULL&&rt->lft->maxi>=p.F)
        return exact_find(rt->lft,p);
    else
        return exact_find(rt->ryt,p);
}

void delete_node(node *rt,pair<int,int> p)//deletes a node
{
    node *curr=exact_find(rt,p);
    node *temp1=new node,*temp2=new node;
    int gh=0;
    //cout<<gh++<<endl;
    if(curr->lft==NULL || curr->ryt==NULL)
        temp1=curr;
    else
        temp1=next_node(curr);
    //cout<<gh++<<endl;

    if(temp1->lft!=NULL)
        temp2=temp1->lft;
    else
        temp2=temp1->ryt;


    temp2->parent=temp1->parent;
    if(temp1->parent==NULL)
        rt=temp2;
    else if(temp1==temp1->parent->lft)
    {
        temp1->parent->lft=temp2;
        //cout<<gh++<<endl;
    }
    else
        temp1->parent->ryt=temp2;
    //cout<<gh++<<endl;
    if(temp1!=curr)
    {
        curr->p.F=temp1->p.F;
    }
    //cout<<gh++<<endl;
    if(temp1->clr==black)
        refresh(rt,temp2);
    //cout<<gh++<<endl;
}

int main()
{
    ios::sync_with_stdio(0);
    int n;
    cout<<"input the number of nodes\n";
    cin>>n;
    cout<<"input the node\n";
    pair<int,int>a;
    node *rt=NULL;
    for(int i=0;i<n;i++)
    {
        cin>>a.F>>a.S;
        node *curr=new node;
        curr->clr=red;
        curr->p=a;
        curr->maxi=a.S;
        curr->lft=curr->ryt=curr->parent=NULL;
        rt=insert_node(rt,curr);
        refresh(rt,curr);
        correct(rt);
    }
    cout<<"dfs on the nodes\n";
    print(rt);
    int q;
    cout<<"input the number of queries\n";
    cin>>q;
    cout<<"enter the queries\n";
    pair<int,int>p;
    while(q--)
    {
        cin>>p.F>>p.S;
        empti=0;
        search_interval_tree(rt,p);
        if(!empti)
            cout<<"interval not found"<<endl;
    }
//    cout<<"enter the node to delete\n";
//    cin>>a.F>>a.S;
//    delete_node(rt,a);
//    correct(rt);
//    cout<<"node deleted\n";
    return 0;
}
