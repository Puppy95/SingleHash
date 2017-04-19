#include"hashfunction.h"
#include<math.h>
#include<string>
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

class CM_sketch
{
	private:
		int width;
		int depth;
		int type;
		unsigned short* Counters;
	public:
		CM_sketch(int w, int d, int t);
		~CM_sketch()
		{
			delete []Counters;
		}
		void insert(const unsigned char* s, int len);
		int query(const unsigned char* s, int len); 
};

CM_sketch::CM_sketch(int w, int d, int t)
{
	width=w;
	depth=d;
	type=t;
	Counters=new unsigned short[w*d];
	for(int i=0;i<d*w;i++)
	Counters[i]=0;
}
void CM_sketch::insert(const unsigned char*s, int len)
{
	 switch(type)
	 	{
	 		case (CS):
	 			{
	 				for(int i =0;i<depth;i++)
	 				{
	 					int hash = ((*hfunc[i])(s,len))%width;
						Counters[i*width+hash]+=1;
					 }
					 break;
				 }
			case (LHS):
			{
				int h1 = ((*hfunc[0])(s,len))%width;
				int h2 = ((*hfunc[1])(s,len))%width;
				for(int i=0;i<depth;i++)
				{
					int hash=(h1+i*h2)%width;
					Counters[i*width+hash]+=1;
				}
				break;
			 }
			case (SHS):
			{ 
				unsigned int h_value = (*hfunc[0])(s,len);
				unsigned int h1 = h_value;
				unsigned int h2 = (h_value>>16);
				h1 = h1%width;
				h2 = h2%width;
				for(int i=0;i<depth;i++)
				{
					int hash=(h1^h2)%width;
					Counters[i*width+hash]+=1;
					h1=h1<<1;
				 }
				 break;
			}
			default: break;
		 }
	return;
}

int CM_sketch::query(const unsigned char* s,int len)
{
	int ans=65535;
	switch(type)
	{
		case (CS):{
				for(int i =0;i<depth;i++)
	 			{
	 				int hash = ((*hfunc[i])(s,len))%width;
					int p = i*width+hash;
					if(Counters[p]<ans)
						ans = Counters[p];
					if(ans == 0) break;
				}
			break;
		}
				case (LHS):{
				int h1 = ((*hfunc[0])(s,len))%width;
				int h2 = ((*hfunc[1])(s,len))%width;
				for(int i =0;i<depth;i++)
	 			{
	 				int hash = (h1+i*h2)%width;
					int p = i*width+hash;
					if(Counters[p]<ans)
						ans = Counters[p];
					if(ans == 0) break;
				}
			break;
		}
				case (SHS):{
				unsigned int h_value = (*hfunc[0])(s,len);
				unsigned int h1 = h_value%width;
				unsigned int h2 = (h_value>>16);
				h1 = h1%width;
				h2 = h2%width;
				for(int i =0;i<depth;i++)
	 			{
					int hash=(h1^h2)%width;
					int p = i*width+hash;
					if(Counters[p]<ans)
						ans = Counters[p];
					if(ans == 0) break;
					h1=h1<<1;
				}
			break;
		}
		default: break;
	}
	return ans;
}
