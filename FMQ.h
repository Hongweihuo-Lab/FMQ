#include "stdafx.h"
#include <stdlib.h>
#include <string>
#include <stack>
#include "FM.h"
#include "InArray.h"
#include "perfectHash.h"
using namespace std;

class FMQ
{
public:
	FMQ(string T,string S, int Tlen, int Slen,int q);
	void getDiff();
	void findDiff(int * * M, int l, int i, int j, int t);
	string T;
	string S;
	int Tlen;
	int Slen;
	int * BV;
	char * *C;
	int * F;                   //差异类型
	InArray * SC;                   //the compresses of BV
	InArray * Brank;
	InArray * SBrank;
	InArray * Header;         //the flag of block  all0 is 0 else is 1
	InArray * V;                  //the offset of position in BV and to-be-compressed sequence  
	FM * CC;                //上下文FM， 压缩完Context后释放             the real stored C
	InArray * FF;          //真正存储的差异类型， 赋值结束后释放F      the type of difference (sub is 0 ins is 1 del is 2)
	char * Context;     //差异连成的串
	stack<char *> tempC;              //the temp stack of C 
	stack<int> tempF;                   //the temp stack of F
	int q;
	int diffnum;
	FM * REF;
	void compress();
	void buildContext();
	int blockSize;   //块长度          the length of block
	int superBlockSize;  //超块长度     the length of superblock
	int select(int index);       //第i个差异对应的待压缩序列上的位置
	void compReference(FM * ref);
	unsigned char * extract(unsigned char * posT, int pos, int len);
	bool access(int k);
	int getnums(int bindex, int sbindex);
	int getRefPos(int pos);
};