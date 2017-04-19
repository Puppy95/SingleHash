#include<iostream>
#include"dleft-hash table.h"
#include<fstream>
#include<map>
#include <vector>
#include <ctime>
#define CS 0
#define SHS 1
#define LHS 2
using namespace std;
int main()
{
    
    int LengthOfSection; int SumOfSection;
    ofstream result;
    result.open("data/NoL.txt");
    ofstream timeresult;
    timeresult.open("data/speed.txt");
    ifstream fcheck("data/uniqueelement.txt");
    ofstream result2("data/LoL.txt");
    vector<string> check;
    string s;
    while(fcheck>>s)
    {
            check.push_back(s);
    }
    fcheck.close();
    for (int i=2; i<=10; i++)
    {
        LengthOfSection = 85000/i;
        SumOfSection = i;
    Dleft_Hash_Table* Cs = new Dleft_Hash_Table(LengthOfSection, SumOfSection,CS);
    Dleft_Hash_Table* SHs = new Dleft_Hash_Table(LengthOfSection, SumOfSection,SHS);
    Dleft_Hash_Table* LHs = new Dleft_Hash_Table(LengthOfSection, SumOfSection, LHS);
    ifstream fin("data/uniqueelement.txt");
    while(fin>>s)
    {
        Cs->insert(s,s.length());
        SHs->insert(s,s.length());
        LHs->insert(s,s.length());
    }
    int d1=0,d2=0,d3=0;
    int maxd1 =0 ,maxd2 =0 ,maxd3 =0;
    int n=0;
    int totalsize = check.size();
    clock_t start,finish;
    double cstotal=0,SHstotal=0,LHstotal=0;
    for(int l=0;l<10;l++)
{
    for(int i=0;i<totalsize;i++)
    {   int querytimes =10;
        int r1=0,r2=0,r3=0;
        start = clock();
        while (querytimes--)
        r1 = Cs->query(check[i],check[i].length());
        finish = clock();
        cstotal+=double(finish-start)/CLOCKS_PER_SEC;
        
        querytimes =10;
        start = clock();
        while (querytimes--)
        r2 = LHs->query(check[i],check[i].length());
        finish = clock();
        LHstotal+=double(finish-start)/CLOCKS_PER_SEC;
        
        querytimes =10;
        start = clock();
        while (querytimes--)
        r3 = SHs->query(check[i],check[i].length());
        finish = clock();
        SHstotal+=double(finish-start)/CLOCKS_PER_SEC;
    }
}
    timeresult<<SumOfSection<<" "<<totalsize/(cstotal*10000)<<" "<<totalsize/(LHstotal*10000)<<' '<<totalsize/(SHstotal*10000)<<endl;
    int CsSumOfUsedChain = Cs->CountUsedChain();
    int CsLengthOfUsedChain = Cs->CountUsedChainLength();
    int SHsSumOfUsedChain = SHs->CountUsedChain();
    int SHsLengthOfUsedChain = SHs->CountUsedChainLength();
    int LHsSumOfUsedChain = LHs->CountUsedChain();
    int LHsLengthOfUsedChain = LHs->CountUsedChainLength();
    result<<SumOfSection<<' '<<CsSumOfUsedChain<<" "<<LHsSumOfUsedChain<<" "<<SHsSumOfUsedChain<<endl;
	result2<<SumOfSection<<" "<<float(CsLengthOfUsedChain)/CsSumOfUsedChain<<" "<<float(LHsLengthOfUsedChain)/SHsSumOfUsedChain<<" "<<float(SHsLengthOfUsedChain)/LHsSumOfUsedChain<<endl;
    }

    return 0;
} 
