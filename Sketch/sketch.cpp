#include<iostream>
#include"CMsketch.h"
#include<fstream>
#include<map>
#include<vector>
#define CS 0
#define SHS 1
#define LHS 2
#include<time.h>
using namespace std;
map<string,int> kv;
int main()
{
	int width; int depth;
	ofstream fout1("data_for_sketch/limitedmemory/Cr.txt");
	ofstream fout2("data_for_sketch/limitedmemory/Are.txt");
	ofstream fout3("data_for_sketch/limitedmemory/speed.txt");
	ifstream fin_map("data_for_sketch/dataforsketch.dat");
	ifstream fin_vec("data_for_sketch/uniqueelement.txt");
	width=500000;
	string s;
	map<string,int>::iterator itr;
	vector<string> v; 
	while(fin_map>>s)
	{
		itr = kv.find(s);
		if(itr!=kv.end())
			itr->second++;
		else
			kv[s]=1; 
	}
	fin_map.close();
	while(fin_vec>>s)
	{
		v.push_back(s);
	}
	fin_vec.close();
		for(depth=2;depth<10;depth++)
	{
	ifstream fin("data_for_sketch/dataforsketch.dat");
	ifstream fcheck("data_for_sketch/uniqueelement.txt");
	CM_sketch* Cs = new CM_sketch(width/depth, depth,CS);
	CM_sketch* SHs = new CM_sketch(width/depth, depth,SHS);
	CM_sketch* LHs = new CM_sketch(width/depth, depth, LHS);
	while(fin>>s)
	{
		Cs->insert((const unsigned char*)s.c_str(),s.length());
		SHs->insert((const unsigned char*)s.c_str(),s.length());
		LHs->insert((const unsigned char*)s.c_str(),s.length());
	}
	int n1=0,n2=0,n3=0;
	float c1=0,c2=0,c3=0;
	int n=0;
	while(fcheck>>s)
	{
		 int value=kv[s];
		int r1 = Cs->query((const unsigned char*)s.c_str(),s.length());
		c1+=float(r1-value)/value;
		if(value==r1)
		n1++;
		int r2 = LHs->query((const unsigned char*)s.c_str(),s.length());
		c2+=float(r2-value)/value;
		if(value==r2)
		n2++;
		int r3 = SHs->query((const unsigned char*)s.c_str(),s.length());
		c3+=float(r3-value)/value;
		if(value==r3)
		n3++;
		n++;		
	}
	fout1<<depth<<' '<<float(n1)/n<<' '<<float(n2)/n<<' '<<float(n3)/n<<endl;
	fout2<<depth<<' '<<c1/n<<' '<<c2/n<<' '<<c3/n<<endl;
	clock_t start,finish;
	start=clock();
	for(int i=0;i<n;i++)
	{
		for(int j=1;j<10;j++)
		Cs->query((const unsigned char*)v[i].c_str(),v[i].length());
	}
	finish=clock();
	double d1= double(finish-start)/CLOCKS_PER_SEC;
	start=clock();
	for(int i=0;i<n;i++)
	{
		for(int j=1;j<10;j++)
		LHs->query((const unsigned char*)v[i].c_str(),v[i].length());
	}
	finish=clock();
	double d2= double(finish-start)/CLOCKS_PER_SEC;
	start=clock();
		for(int i=0;i<n;i++)
	{
		for(int j=1;j<10;j++)
		SHs->query((const unsigned char*)v[i].c_str(),v[i].length());
	}
	finish=clock();
	double d3= double(finish-start)/CLOCKS_PER_SEC;
	fout3<<depth<<' '<<float(n)/(d1*100000)<<' '<<float(n)/(d2*100000)<<' '<<float(n)/(d3*100000)<<endl;
	fin.close();
	fcheck.close();
	delete Cs;
	delete LHs;
	delete SHs;
}
	return 0;
} 
