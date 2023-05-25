//input: g++ buddy.cc
//this is done by creating binary tree
#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;
#define N 64

struct NODE{
    int key[2];
    struct NODE* chi[2];
    struct NODE* parent;
    int num;
    bool free;
    bool leaf;
    int filled;

};

NODE*
find_child(NODE* s){
    if (s->chi[0]==NULL) return s;
    while(s->chi[0]!=NULL){
        s=s->chi[0];
    }
    //printf("num: %i, start: %i, end: %i leaf:%d block: %i\n", s->num,s->key[0], s->key[1],s->leaf,s->filled);
    return s;
}


NODE*
alloc_node(){
        NODE *node;
        node = (NODE *)calloc(1, sizeof(NODE));

    return node;
}

NODE*
defi(NODE* s, int start,int end,NODE* start_chi,NODE* end_chi,NODE* parent,int num,bool free,bool leaf, int filled=0){
    s->key[0]=start;
    s->key[1]=end;
    s->chi[0]=start_chi;
    s->chi[1]=end_chi;
    s->parent=parent;
    s->num=num;
    s->free=free;
    s->leaf=leaf;
    s->filled=filled;
    return s;
}

void
print_leaf(NODE* s){
    while(true){

        printf("num: %i, start: %i, end: %i leaf:%d isfilled: %d\n", s->num,s->key[0], s->key[1],s->leaf,s->free);

        if(s->chi[1]==NULL){
            break;
        }else{
        s=s->chi[1];}
    }

}

NODE*
init(NODE* s){
    //if lim is 4, then min split block is 4, if 3 then min split block is 8
    int i=0,num, lim=3;
    NODE* mem=NULL, *temp=NULL;
    //head is intialization
    NODE* head;
    head=alloc_node();
    head=s;
    while(i<lim){
        NODE* a,*b;
        num=s->num/2;
        cout<<"(splitting 0/"<<s->num<<")"<<endl;

        a=alloc_node();
        a=defi((NODE*)a,0,num-1,NULL,NULL,(NODE*)s,num,true,false);
        s->chi[0]=(NODE*)a;

        b=alloc_node();
        b=defi((NODE*)b,num,s->key[1],NULL,(NODE*)mem,(NODE*)s,num,true,true);
        mem=(NODE*)b;
        s->chi[1]=(NODE*)b;

        if (i!=0){
            temp->chi[0]=(NODE*)b;
        }
        temp=(NODE*)b;
        s=(NODE*)a;
        i++;
    }
    s->leaf=true;
    s->chi[1]=(NODE*)mem;
    mem->chi[0]=(NODE*)s;
    cout<<"(splitting 0/"<<s->num<<")"<<endl;

    //To check if the binary tree is working correctly
    //head=head->chi[1];
    //print_leaf(s);

    return (NODE*)s;
}

NODE*
split(NODE* s, int block){
    NODE* a,*b;
    a=alloc_node();
    b=alloc_node();
    int i=2;
    while(i<=s->num){
        if (i>=block){
            if ((s->num-i)<2){
                s->free=false;
                s->filled=block;
                printf("Blockss %i-%i allocated:\n",s->key[0],s->key[0]+s->filled);
                return s;
            }
            else{
                //NODE* s, int start,int end,NODE* start_chi,NODE* end_chi,NODE* parent,int num,bool free,bool leaf, int filled=0
                a=defi((NODE*)a,s->key[0],i-1,(NODE*)s->chi[0],NULL,(NODE*)s,i,false,true,block);
                b=defi((NODE*)b,i,s->key[1],(NODE*) a,(NODE*)s->chi[1],(NODE*)s,s->num-i,true,true,0);
                a->chi[1]=(NODE*)b;
                s->leaf=false;
                if (s->chi[0]!=NULL){
                    s->chi[0]->chi[1]=(NODE*)a;
                }
                if(s->chi[1]!=NULL){
                    s->chi[1]->chi[0]=(NODE*)b;
                }
                printf("Blocks %i-%i allocated:\n",a->key[0],a->key[1]);
                //print_leaf(s);
                return a;
            }
        }
        i=i*2;
    }
    cout<<"Block out fo range, can not allocate"<<endl;
    //printf("num: %i, start: %i, end: %i leaf:%d block: %i\n", s->num,s->key[0], s->key[1],s->leaf,block);
    return s;
}

NODE*
alloc_block(NODE* s,int block){

    while (true){
        if (s->num>=block && s->free==true){
            return split(s,block);
        }
        if (s->chi[1]==NULL){
            cout<<"WARNING: block is full"<<endl;
            return s;
        }
        s=s->chi[1];
    }
}

NODE*
merge(NODE* s){
    NODE* a;
    a=alloc_node();
    //if the left node is free
    if (s->chi[0]!=NULL && s->chi[0]->free==true){
        printf("Merging to the left node\n");
        a=s->chi[0];
        //if a is the a node, merge s to a
        if (a->parent->chi[0]==(NODE*)a){
            printf("mergering (%i/%i) and (%i/%i) 00\n",a->key[0],a->num,s->key[0],s->num);
            s->parent->chi[1]=NULL;
            a->key[0]=s->key[0];
            a->chi[1]=s->chi[1];
            if (s->chi[1]!=NULL)s->chi[1]->chi[0]=(NODE*)a;
            a->num=a->num+s->num;
            a->filled=0;
            a->leaf=true;
        }
        //if a is the b node, merge a to s
        else if(a->parent->chi[1]==(NODE*)a){
            printf("mergering (%i/%i) and (%i/%i) 01\n",a->key[0],a->num,s->key[0],s->num);
            a->parent->chi[1]=NULL;
            s->key[0]=a->key[0];
            s->chi[0]=a->chi[0];
            if (a->chi[0]!=NULL)a->chi[0]->chi[1]=(NODE*) s;
            s->num=s->num+a->num;
            s->filled=0;
            s->leaf=true;
        }
    }
    //if the right node is free
    if (s->chi[1]!=NULL && s->chi[1]->free==true){
        printf("Merging to the right node\n");
        a=s->chi[1];
        //if a is the a node, merge s to a
        if (a->parent->chi[0]==(NODE*)a){
            printf("mergering (%i/%i) and (%i/%i) 10\n",s->key[0],s->num,a->key[0],a->num);
            s->parent->chi[1]=NULL;
            a->key[1]=s->key[1];
            a->chi[0]=s->chi[0];
            if(s->chi[0]!=NULL)s->chi[0]->chi[1]=(NODE*)a;
            a->num=a->num+s->num;
            a->filled=0;
            a->leaf=true;
            return a;
        }
        //if a is the b node, merge a to s
        else if(a->parent->chi[1]==(NODE*)a){
            //print_leaf(s);
            printf("mergering (%i/%i) and (%i/%i) 11\n",s->key[0],s->num,a->key[0],a->num);
            a->parent->chi[1]=NULL;
            s->key[1]=a->key[1];
            s->chi[1]=a->chi[1];
            if (a->chi[1]!=NULL)a->chi[1]->chi[0]=(NODE*)s;
            s->num=s->num+a->num;
            s->filled=0;
            s->leaf=true;
        }
        return s;

    }
    //printf("No near empty block");
    return s;
}

NODE*
free_block(NODE* s,int block){
    //print_leaf(s);
    while(s->chi[1]!=NULL){
        //printf("the key[0] is :%i and it is free%d\n",s->key[0],s->free);
        if (s->key[0]==block && s->free==false){
            s->filled=0;
            s->free=true;
            merge(s);
            return s;
        }
        s=s->chi[1];
    }
    if (s->key[0]==block && s->free==false){
        s->filled=0;
        s->free=true;
        merge(s);
        return s;
    }
    cout<<"No allocated block that starts with "<<block<<" therefore could not free any block"<<endl;
    return s;
}

void
print_letter(int total, int covered){
    int i;
    for(i=0; i<total;i++){
        if(covered>i){
            cout<<'#';
        }
        else{
            cout<<'-';
        }
    }
}

void
print_tree(NODE* s){
    while(s->chi[1]!=NULL){
        print_letter(s->num, s->filled);
        cout<<'|';
        s=s->chi[1];
    }
    print_letter(s->num,s->filled);
    cout<<"|\n";
}

int
main(int argc, char* argv[]){
    //define the first node
    NODE* s;
    s=alloc_node();
    s=defi(s,0,N,NULL,NULL,NULL,N,true,false);
    s=init(s);

    //infinite loop till q is pressed
    while(true){
        //get the command line
        vector<string> v;
        char command;
        int block=0;
        cout<<"How many blocks do you want to allocate/fee? \n";
        string line;
        getline(cin,line);
        //if q exit
        if(line.front()=='q'){
            return 0;
        }
        //if other is pressed, continue
        if (line.front()!='a' && line.front()!='f' && line.front()!='s'){
            cout<<"the first argument must be 'q' or 'a' or 'f'"<<endl;
            continue;
        }
        stringstream line2{line};
        while(getline(line2,line,' ')){
            v.push_back(line);
        }
        command=v[0].front();
        block=stoi(v[1]);
        //if block out of range
        if (block>N){
            cout<<"block out of range"<<endl;
            return 0;
        }

        //if a allocate, if f free
        s=find_child(s);
        if (command=='a'){
            s=alloc_block(s,block);
        }else if (command=='f'){
            free_block(s,block);
        }else if (command=='s'){
            print_leaf(s);
        }
        s=find_child(s);
        print_tree(s);
    }
    return 0;
}
