#include"hashfunction.h"
#include<math.h>
#include<string>
#include<map>
#include<iostream>
#include<memory.h>
#define CS 0
#define SHS 1
#define LHS 2
using namespace std;
unsigned int (*hfunc[14])(const unsigned char*, unsigned int)=
{
    BOB1,
    BOB2,
    BOB3,
    BOB4,
    PJW,
    DJB,
    CRC32,
    OCaml,
    SML,
    STL,
    FNV32,
    Hsieh,
    RSHash	
};
class node
{
public:
    string key;
    int value;
    int depth;
    node* next;
};
class query_result
{
public:
    int hashtimes;
    int depth;
};
class Dleft_Hash_Table
{
private:
    int LengthOfSection;
    int SumOfSection;
    int type;
    node **Hash_Table;
public:
    Dleft_Hash_Table(int l, int s, int t);
    ~Dleft_Hash_Table()
    {
        delete [] Hash_Table;
    }
    void insert(string s, int len);
    int query(string s, int len);
    int CountUsedChain();
    int CountUsedChainLength();
};
int Dleft_Hash_Table::CountUsedChain()
{
    int TotalLength = LengthOfSection*SumOfSection;
    int SumOfBucket =0;
    for (int i=0; i<TotalLength; ++i)
    {
        if (Hash_Table[i]!=NULL&&Hash_Table[i]->depth!=1)
        
            SumOfBucket++;
    }
    return SumOfBucket;
}
int Dleft_Hash_Table::CountUsedChainLength()
{
    int TotalLength = LengthOfSection*SumOfSection;
    int LengthOfChain =0;
    for (int i=0; i<TotalLength; ++i)
    {
        if (Hash_Table[i]!=NULL&&Hash_Table[i]->depth!=1)
            LengthOfChain+=Hash_Table[i]->depth-1;
    }
    return LengthOfChain;
}

Dleft_Hash_Table::Dleft_Hash_Table(int l, int s, int t)
{
    LengthOfSection=l;
    SumOfSection=s;
    type=t;
    int hashsize = LengthOfSection * SumOfSection;
    Hash_Table = new node*[hashsize];
    for(int i=0;i<hashsize;i++)
    	Hash_Table[i]=NULL;
}
void Dleft_Hash_Table::insert(string s, int len)
{
    switch(type)
    {
        case (CS):
        {
            int IdOfMinLoadDepth=0;
            int Minload=INT_MAX;
            unsigned int hash_M=0;
            for(int i =0;i<SumOfSection;i++)
            {
                unsigned int hash = ((*hfunc[i])((const unsigned char*)s.c_str(),len))%LengthOfSection;
                int load = 0;
                if(Hash_Table[i*LengthOfSection+hash]==NULL)
                    load = 0;
                else
                    load = Hash_Table[i*LengthOfSection+hash]->depth;
                if(load<Minload)
                {
                    IdOfMinLoadDepth = i;
                    Minload =load;
                    hash_M=hash;
                }
            }
            unsigned int hash = hash_M;
            node* newnode = new node;
            newnode->key =  s;
            newnode->value =len;
            newnode->depth =Minload+1;
            newnode->next =Hash_Table[IdOfMinLoadDepth*LengthOfSection+hash];
            Hash_Table[IdOfMinLoadDepth*LengthOfSection+hash] = newnode;
            break;
        }
        case (LHS):
        {
            unsigned int h1 = ((*hfunc[0])((const unsigned char*)s.c_str(),len))%LengthOfSection;
            unsigned int h2 = ((*hfunc[1])((const unsigned char*)s.c_str(),len))%LengthOfSection;
            int IdOfMinLoadDepth=0;
            int Minload=INT_MAX;
            unsigned int hash_M=0;
            for(int i=0;i<SumOfSection;i++)
            {
                int hash=(h1+i*h2)%LengthOfSection;
                int load = 0;
                if(Hash_Table[i*LengthOfSection+hash]==NULL)
                    load = 0;
                else
                    load = Hash_Table[i*LengthOfSection+hash]->depth;
                if(load<Minload)
                {
                    IdOfMinLoadDepth = i;
                    Minload =load;
                    hash_M = hash;
                }
            }
            unsigned int hash=hash_M;
            node* newnode = new node;
            newnode->key =s;
            newnode->value =len;
            newnode->depth =Minload+1;
            newnode->next =Hash_Table[IdOfMinLoadDepth*LengthOfSection+hash];
            Hash_Table[IdOfMinLoadDepth*LengthOfSection+hash] =newnode;
            break;
        }
        case (SHS):
        {
            unsigned int h_value = (*hfunc[0])((const unsigned char*)s.c_str(),len);
            unsigned int h1 = h_value;
            unsigned int h2 = (h_value>>16);
            int IdOfMinLoadDepth=0;
            unsigned int hash_M=0;
            int Minload=INT_MAX;
            for(int i=0;i<SumOfSection;i++)
            {
                int hash=(h1^h2)%LengthOfSection;
                int load = 0;
                if(Hash_Table[i*LengthOfSection+hash]==NULL)
                    load = 0;
                else
                    load = Hash_Table[i*LengthOfSection+hash]->depth;
                if(load<Minload)
                {
                    IdOfMinLoadDepth = i;
                    Minload =load;
                    hash_M = hash;
                }
                h1=h1<<1;
            }
             unsigned int hash = hash_M;
            node* newnode = new node;
            newnode->key =s;
            newnode->value =len;
            newnode->depth =Minload+1;
            newnode->next =Hash_Table[IdOfMinLoadDepth*LengthOfSection+hash];
            Hash_Table[IdOfMinLoadDepth*LengthOfSection+hash] =newnode;
            break;
        }
        default: break;
    }
    return;
}

int  Dleft_Hash_Table::query(string s,int len)
{
    switch(type)
    {
        case (CS):{
            for(int i =0;i<SumOfSection;i++)
            {
                unsigned int hash = ((*hfunc[i])((const unsigned char*)s.c_str(),len))%LengthOfSection;
                node *np =   Hash_Table[i*LengthOfSection+hash];
                while(np!=NULL)
                {
                    if (s.compare(np->key)==0) {
//                        query_result q;
//                        q.hashtimes = i+1;
//                        q.depth = LengthOfChain-(np->depth);
                        return np->value;
                    }
                    np=np->next;
                }
            }
            break;
        }
        case (LHS):{
            unsigned int h1 = ((*hfunc[0])((const unsigned char*)s.c_str(),len))%LengthOfSection;
            unsigned int h2 = ((*hfunc[1])((const unsigned char*)s.c_str(),len))%LengthOfSection;
            for(int i =0;i<SumOfSection;i++)
            {
                int hash = (h1+i*h2)%LengthOfSection;
                node *n =   Hash_Table[i*LengthOfSection+hash];
                while(n!=NULL)
                {
                    if (s.compare(n->key)==0)
                    {
//                        query_result q;
//                        q.hashtimes =i+1;
//                        q.depth =LengthOfChain-(n->depth);
                 return n->value;
                    }
                    n=n->next;
                }
            }
            break;
        }
        case (SHS):{
            unsigned int h_value = (*hfunc[0])((const unsigned char*)s.c_str(),len);
            unsigned int h1 = h_value;
            unsigned int h2 = (h_value>>16);
            for(int i =0;i<SumOfSection;i++)
            {
                int hash=(h1^h2)%LengthOfSection;
                node *n =   Hash_Table[i*LengthOfSection+hash];
                while(n!=NULL)
                {
                    if (s.compare(n->key)==0)
                    {
//                        query_result q;
//                        q.hashtimes =i+1;
//                        q.depth = LengthOfChain-(n->depth);
//                        return q;
                     return n->value;
                    }
                    n=n->next;
                }
                h1=h1<<1;
            }
            break;
        }
        default: break;
    }
    return 0;
}
