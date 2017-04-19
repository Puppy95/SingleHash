//#include "hashfunction.h"
#include<math.h>
#include<string>
#include<iostream>
#include<memory.h>
#define CBF 0
#define SHBF 1
#define LHBF 2 
using namespace std; 

class PBF
{
		public:
		PBF(int p_t, int k_t,int type);
		~PBF(){
			delete []filter;
		};
		void Insert(const unsigned char* str, unsigned int len);
		int Query(const unsigned char* str, unsigned int len);
		
		private:
			int p;
			int k;
			int t; 
			unsigned short* filter;
			
};

PBF::PBF(int p_t, int k_t,int type)
{
	p = p_t;
	k = k_t;
	t = type;
	filter = new unsigned short[((p*k)/16)+1];
	memset(filter, 0, sizeof(short)*((p*k)/16+1)); 
}

void PBF::Insert(const unsigned char* str, unsigned int len)
{
	
		if(t==SHBF)
		{
		unsigned int hash = (*hfunc[0])(str, len);
		unsigned int v1 = hash%p;
		unsigned int v2 = hash >>16;
		v2=v2%p;
		int h_value, p1, p2,tmp;
		for (int i =0; i<k; i++)
		{
			h_value = (v1 ^ v2)%p; 
			p1 = (i*p+h_value)/16;
			p2 = (i*p+h_value)%16;
			tmp = 1<<(16-p2);
			if(p2 == 0)
				filter[p1-1] = filter[p1-1] | 1;
			else 
				filter[p1] = filter[p1] | tmp;
			v1=v1<<1;
		}
		return;
		}
		else if(t==LHBF)
		{
		int h_value, p1, p2,tmp;
		int v1 = ((*hfunc[10])(str, len))%p;
		int v2 = ((*hfunc[12])(str,len))%p; 
		for (int i =0; i<k; i++)
		{
			h_value = (v1 + i*v2)%p; 
			p1 = (i*p+h_value)/16;
			p2 = (i*p+h_value)%16;
			tmp = 1<<(16-p2);
			if(p2 == 0)
				filter[p1-1] = filter[p1-1] | 1;
			else 
				filter[p1] = filter[p1] | tmp;
		}
		return;	
		}
		else if(t==CBF)
		{
		int h_value, p1, p2,tmp;
		for (int i =0; i<k; i++)
		{
			h_value = ((*hfunc[i])(str, len))%p;
			p1 = (i*p+h_value)/16;
			p2 = (i*p+h_value)%16;
			tmp = 1<<(16-p2);
			if(p2 == 0)
				filter[p1-1] = filter[p1-1] | 1;
			else 
				filter[p1] = filter[p1] | tmp;
		}
		return;	
		}
}

int PBF::Query(const unsigned char* str, unsigned int len)
{
	int ans=1;
	if(t==SHBF)
	{
		unsigned int hash = ((*hfunc[0])(str, len));
		unsigned int v1 = (hash)%p;
		unsigned int v2 = hash >>16;
		v2=v2%p;
		int h_value, p1, p2,tmp;
		for (int i =0; i<k; i++)
		{
			h_value = (v1 ^ v2)%p; 
			p1 = (i*p+h_value)/16;
			p2 = (i*p+h_value)%16;
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
	else if(t==LHBF)
	{
		int h_value, p1, p2,tmp;
		int v1 = ((*hfunc[10])(str, len))%p;
		int v2 = ((*hfunc[12])(str,len))%p; 
		for (int i =0; i<k; i++)
		{
			h_value = (v1 + i*v2)%p; 
			p1 = (i*p+h_value)/16;
			p2 = (i*p+h_value)%16;
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
	else if(t==CBF)
	{
		int h_value, p1, p2,tmp;
		for (int i =0; i<k; i++)
		{
			h_value = ((*hfunc[i])(str, len))%p;
			p1 = (i*p+h_value)/16;
			p2 = (i*p+h_value)%16;
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
