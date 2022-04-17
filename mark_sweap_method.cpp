#include<iostream>
using namespace std;
struct node
{
    int data;
    bool mark;
    struct node* adj1;
    struct node* adj2;
    struct node* adj3;
    node(int n)
    {
        data = n;
        adj1=NULL;
        adj2=NULL;
        adj3=NULL;
        mark=false;
    }
}; //Atmost 3 connections are present
typedef struct root_tag
{
    node* pointer;
}root;

node* heap[8];

void garbage_collector(const root& r1,const root& r2,node** hp);
void mark(const root& value);
void sweep(node** arr);
void mark_node(node* ptr);
void print_useful_heap(const root& value);
void print_heap(node** arr);
void print_node(const node* node);
void initialize(root& root1,root& root2);

int main()
{
    root A,B;
    initialize(A,B);
    cout<<"This is the Simulation for mark sweep garbage collector\n";
    cout<<"Key to navigate the printed table: - indicates connection and [] indicate all the elements connected to the element\n";
    cout<<"the full heap is: "<<endl;
    print_heap(heap);
    cout<<"***************"<<endl;
    cout<<"Heap connected to the roots is: "<<endl;
    print_useful_heap(A);
    print_useful_heap(B);
    cout<<"****************"<<endl;
    garbage_collector(A,B,heap);
    cout<<"the garbage collector was called\n";
    print_heap(heap);
    cout<<"does it match heap connected or not to roots printed below\n";
    print_useful_heap(A);
    print_useful_heap(B);
}

void initialize(root& root1,root& root2)
{                                                   
    node* temp = new node(5);
    heap[0]=temp;
    temp = new node(1);
    heap[1]=temp;
    temp = new node(2);
    heap[2]=temp;
    temp = new node(9);
    heap[3]=temp;
    temp = new node(10);
    heap[4]=temp;
    temp = new node(7);
    heap[5]=temp;
    temp = new node(8);
    heap[6]=temp;
    temp = new node(3);
    heap[7]=temp;
    temp = NULL;
    
    root1.pointer=heap[0];//root1->5
    heap[0]->adj1=heap[1];//5->1
    root2.pointer=heap[1];//root2->1
    heap[1]->adj1=heap[2];//1->2
    heap[1]->adj2=heap[3];//1->9
    heap[1]->adj3=heap[4];//1->10
    heap[5]->adj1=heap[1];//7->1
    heap[5]->adj2=heap[6];//7->8
    heap[6]->adj1=heap[4];//8->10
    heap[7]->adj1=heap[6];//3->8
    heap[7]->adj2=heap[4];//3->10
   
}
void print_node(const node* node)
{
    if(node==NULL)
    {
        return;
    }
    cout<<" "<<node->data;
    if (node->adj1!=NULL || node->adj2!=NULL || node->adj3!=NULL)
    {
        cout<<"-[";
        print_node(node->adj1);
        print_node(node->adj2);
        print_node(node->adj3);
        cout<<" ]";
    }
}
void print_heap(node** arr)
{
    for(int i=0;i<8;i++)
    {
        if(arr[i]!=NULL)
        {
            print_node(arr[i]);
            cout<<endl;
        }
    }
}
void print_useful_heap(const root& value)
{
    cout<<"root->";
    print_node(value.pointer);
    cout<<endl;
}

void mark_node(node* ptr)
{
    node* head=ptr;
    node* tail=NULL;
    node* middle=NULL;
    int flag=1;
    while(head !=NULL)
    {
        if(!head->mark) //if node we are on is unmarked, mark it
        {
            head->mark=true;
        }
        if(head->adj1!=NULL && !head->adj1->mark) //if adjacent node to this is unmarked, move to it
        {
            tail=middle;
            middle=head;
            head=head->adj1;
        }
        else if(head->adj2!=NULL && !head->adj2->mark)//only move along one adjacent path every iteration
        {
            tail=middle;
            middle=head;
            head=head->adj2;   
        }
        else if(head->adj3!=NULL && !head->adj3->mark)
        {
            tail=middle;
            middle=head;
            head=head->adj3;   
        }
        else
        {
            head=middle;
            middle=tail;
            tail=NULL;
        }

    }
}
void mark(const root& value)
{
    mark_node(value.pointer);
}
void sweep(node** arr)
{
    for(int i=0;i<8;i++)
    {
        if(arr[i]!=NULL)
        {
            if(!arr[i]->mark)
            {
                //disconnect the node
                arr[i]->adj1=NULL;
                arr[i]->adj2=NULL;
                arr[i]->adj3=NULL;
                delete arr[i];
                arr[i]=NULL;
            }
        }
    }
}
void garbage_collector(const root& r1,const root& r2,node** hp)
{
    cout<<"Mark phase started."<<endl;
    mark(r1);
    mark(r2);
    cout<<"Marking done"<<endl;
    cout<<"Sweep phase started."<<endl;
    sweep(hp);
}

