#include<iostream>
#include<fstream>
#include<string>
#include<time.h>
#include "Bloomfilter.h"
#include "partion-bf.h"
#define CBF 0
#define SHBF 1
#define LHBF 2 
using namespace std;
#define width 200000
//#define k_num 10
int main() {
	string s[80000];
	int n=40000;
	ofstream fout1("npBF-fp.dat");
	ofstream fout2("pBF-fp.dat");
	ofstream fout3("npBF-sp.dat");
	ofstream fout4("pBF-sp.dat");
	clock_t start,finish;
	int tmp; 
	ifstream fin_s("data_for_bf/uniqueelement.txt");
	for(int i=0;i<80000;i++)
	{
		fin_s>>s[i];
	}
	fin_s.close();
//	for(int width =100000; width<=300000; width+=20000)
//{
	for(int k_num=2; k_num<11;k_num++)
{
//	fout<< width <<' '<<k_num<<endl;
//	for(n=5000;n<=40000;n+=5000)
//	{
	ifstream fin("data_for_bf/uniqueelement.txt"); 
	BF* cl_BF = new BF(width, k_num, 0);
	BF* sh_BF = new BF(width, k_num, 1);
	BF* lh_BF = new BF(width, k_num, 2);
	PBF* ps_BF = new PBF(width/k_num,k_num,SHBF);
	PBF* pl_BF = new PBF(width/k_num,k_num,LHBF);
	PBF* pc_BF = new PBF(width/k_num,k_num,CBF);
	int c1p=0; 
	int c2p=0;
	int c3p=0;
	int c1n=0;
	int c2n=0;
	int c3n=0;
	for(int i=0; i<n;i++)
	{
		cl_BF->Insert((const unsigned char*)s[i].c_str(), s[i].length());
		sh_BF->Insert((const unsigned char*)s[i].c_str(), s[i].length());
		lh_BF->Insert((const unsigned char*)s[i].c_str(), s[i].length());
		ps_BF->Insert((const unsigned char*)s[i].c_str(), s[i].length());
		pl_BF->Insert((const unsigned char*)s[i].c_str(), s[i].length());
		pc_BF->Insert((const unsigned char*)s[i].c_str(), s[i].length());	
	}
	    int N=80000;
		for(int i=n;i<N;i++)
		{
		tmp = cl_BF->Query((const unsigned char*)s[i].c_str(), s[i].length());
		if(tmp==0)
		c1p++;
		tmp = lh_BF->Query((const unsigned char*)s[i].c_str(), s[i].length());
		if(tmp==0)
		c2p++;
		tmp = sh_BF->Query((const unsigned char*)s[i].c_str(), s[i].length());
		if(tmp==0)
		c3p++;
		tmp = pc_BF->Query((const unsigned char*)s[i].c_str(), s[i].length());
		if(tmp==0)
		c1n++;
		tmp = pl_BF->Query((const unsigned char*)s[i].c_str(), s[i].length());
		if(tmp==0)
		c2n++;
		tmp = ps_BF->Query((const unsigned char*)s[i].c_str(), s[i].length());
		if(tmp==0)
		c2n++;
		}
    fout1<<k_num<<' '<<1-float(c1p)/(80000-n)<<' '<<1-float(c2p)/(80000-n)<<' '<<1-float(c3p)/(80000-n)<<endl;   
	fout2<<k_num<<' '<<1-float(c1n)/(80000-n)<<' '<<1-float(c2p)/(80000-n)<<' '<<1-float(c3p)/(80000-n)<<endl;
		start = clock();
		for(int i=0;i<N;i++)
		{
		for(int j=0;j<100;j++)
		tmp = cl_BF->Query((const unsigned char*)s[i].c_str(), s[i].length());
	    }
	finish = clock();
	double d1 = double(finish-start)/CLOCKS_PER_SEC;
	start = clock();
	for(int i=0;i<N;i++)
	{
	for(int j=0;j<100;j++)
		tmp = lh_BF->Query((const unsigned char*)s[i].c_str(), s[i].length());
	}
	finish = clock();
	double d2 = double(finish-start)/CLOCKS_PER_SEC;
	start = clock();
	for(int i=0;i<N;i++)
	{
		for(int j=0;j<100;j++)
		tmp = sh_BF->Query((const unsigned char*)s[i].c_str(), s[i].length());
	}
	finish = clock();
	double d3 = double(finish-start)/CLOCKS_PER_SEC;
	fout3<<k_num<<' '<<float(N)/(d1*10000)<<' '<<float(N)/(d2*10000)<<' '<<float(N)/(d3*10000)<<endl;
//}
}
//}
	return 0;
}
