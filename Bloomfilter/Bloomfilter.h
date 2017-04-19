#include "hashfunction.h"
#include<math.h>
#include<string>
#include<iostream>
#include<memory.h>
#define CBF 0
#define SHBF 1
#define LHBF 2 
using namespace std; 

unsigned int (*hfunc[14])(const unsigned char*, unsigned int)=
{
	BOB1,
	PJW,
	DJB,
	CRC32,
	OCaml,
	SML,
	STL,
	FNV32,
	BOB2,
	BOB3,
	BOB4,
	Hsieh,
	RSHash	
};

class BF
{
		public:
		BF(int w_t, int k_t, short type_t);
		~BF(){
			delete []filter;
		};
		void Insert(const unsigned char* str, unsigned int len);
		int Query(const unsigned char* str, unsigned int len);
		
		private:
			int w;
			int k;
			short type;
			unsigned short* filter;
			
};

BF::BF(int w_t, int k_t, short type_t)
{
	w = w_t;
	k = k_t;
	type = type_t;
	filter = new unsigned short[(w/16)+1];
	memset(filter, 0, sizeof(short)*(w_t/16+1)); 
}

void BF::Insert(const unsigned char* str, unsigned int len)
{
	
	if(type==1)
	{
		unsigned int hash = (*hfunc[0])(str, len);
		unsigned int v1 = hash;
		unsigned int v2 = hash >>16;
		v1=v1%w;
		v2=v2%w;
		int h_value, p1, p2,tmp;
		for (int i =0; i<k; i++)
		{
			h_value = (v1 ^ v2)%w; 
			p1 = h_value/16;
			p2 = h_value%16;
			tmp = 1<<(16-p2);
			if(p2 == 0)
				filter[p1-1] = filter[p1-1] | 1;
			else 
				filter[p1] = filter[p1] | tmp;
			v1=v1<<1;
		}
		return;
	}
	if(type==2)
	{
		int v1 = ((*hfunc[10])(str, len))%w;
		int v2 = ((*hfunc[12])(str,len))%w;
		int h_value, p1, p2,tmp; 
		for (int i =0; i<k; i++)
		{
			h_value = (v1+v2*i)%w; 
			p1 = h_value/16;
			p2 = h_value%16;
			tmp = 1<<(16-p2);
			if(p2 == 0)
				filter[p1-1] = filter[p1-1] | 1;
			else 
				filter[p1] = filter[p1] | tmp;
		}
		return;
	}
	 
	if(type==0)
	{
		int h_value, p1, p2,tmp;
		for(int i=0; i<k; i++)
		{
			h_value = ((*hfunc[i])(str, len))%w;
			p1 = h_value/16;
			p2 = h_value%16;
			tmp = 1<<(16-p2);
			if(p2 == 0)
				filter[p1-1] = filter[p1-1] | 1;
			else 
				filter[p1] = filter[p1] | tmp;
		}
			return;
	}
}

int BF::Query(const unsigned char* str, unsigned int len)
{
	int ans=1;
		if(type==1)
	{
		unsigned int hash = ((*hfunc[0])(str, len));
		unsigned int v1 = hash;
		unsigned int v2 = hash >>16;
		v1=v1%w;
		v2=v2%w;
		int h_value, p1, p2,tmp;
		for (int i =0; i<k; i++)
		{
			h_value = (v1 ^ v2)%w; 
			p1 = h_value/16;
			p2 = h_value%16;
			if(p2 == 0)
				tmp = filter[p1-1] & 1;
			else 
				tmp = (filter[p1]>>(16-p2)) & 1;
			if(tmp == 0)
			{
				ans = 0;
				break;
			}
			v1=v1<<1;
		}
		return ans;
	}
			if(type==2)
	{
		int h_value, p1, p2,tmp;
		int v1 = ((*hfunc[10])(str, len))%w;
		int v2 = ((*hfunc[12])(str,len))%w; 
		for (int i =0; i<k; i++)
		{
			h_value = (v1+v2*i)%w; 
			p1 = h_value/16;
			p2 = h_value%16;
			if(p2 == 0)
				tmp = filter[p1-1] & 1;
			else 
				tmp = (filter[p1]>>(16-p2)) & 1;
			if(tmp == 0)
			{
				ans = 0;
				break;
			}
		}
		return ans;
	}
	if(type==0)
	{
		int h_value, p1, p2,tmp;
		for(int i=0; i<k; i++)
		{
			h_value = ((*hfunc[i])(str, len))%w;
			p1 = h_value/16;
			p2 = h_value%16;
			if(p2 == 0)
				tmp = filter[p1-1] & 1;
			else 
				tmp = (filter[p1]>>(16-p2)) & 1;
			if(tmp == 0)
			{
				ans = 0;
				break;
			}
		}
		return ans;
	}
}
